#define WIN32_LEAN_AND_MEAN // SDL doesn't seem to define this?

#include <SDL.h>
#include <iostream>
#include <math.h>

#include "TouhouRenderer.h"
#include "TouhouGame.h"
#include "State.h"


// ECS Managers
#include "EntityManager.h"
#include "TransformManager.h"

const int target_render_fps = 60;
const int target_physics_fps = 50;
const float target_frame_milliseconds = (1000.0f / target_render_fps);
const float physics_step_miliseconds = (1000.0f / target_physics_fps);

inline float GetSecondsElapsed(uint64_t start, uint64_t end)
{
	const float perfFreqMs = (float) SDL_GetPerformanceFrequency() / 1000.0f;
	return (float)(end - start) / perfFreqMs;
}

int main(int argc, char* args[])
{
	// ECS TESTS
#if 0
	EntityManager entities = EntityManager();
	TransformManager transforms = TransformManager(1024);

	Entity ent1 = entities.Create();
	Entity ent2 = entities.Create();
	Entity ent3 = entities.Create();

	transforms.Add(ent1, M3x3::Identity());
	transforms.Add(ent2, M3x3::Identity());
	transforms.Add(ent3, M3x3::Identity());

	transforms.GarbageCollect(entities);
	entities.Destroy(ent2);
	transforms.GarbageCollect(entities);
#endif

	TouhouRenderer renderer("Tha Window", 1024, 768);
	TouhouGame game;

	int debugFrameCounter = 0;
	int debugPhysicsUpdates = 0;
	uint64_t debugBegin = SDL_GetPerformanceCounter();

	uint64_t physicsTickAccumulator = 0;
	const uint64_t ticksPerPhysicaUpdate = SDL_GetPerformanceFrequency() / target_physics_fps;
	const uint64_t ticksPerRenderUpdate = SDL_GetPerformanceFrequency() / target_render_fps;

	float renderTime = 0; // Stores time taken to render the previous frame (in ms)

	while (game.IsRunning())
	{
		uint64_t frameBegin = SDL_GetPerformanceCounter();

		// Input
		game.ProcessSDLInput();

		// PHYSICS UPDATE LOOP.
		// @NOTE: I do it this way to guarantee the same dTime for every physics update.
		while (physicsTickAccumulator >= ticksPerPhysicaUpdate)
		{
			physicsTickAccumulator -= ticksPerPhysicaUpdate;
			
			game.Simulate(physics_step_miliseconds);

			debugPhysicsUpdates++;
		}

		// SLEEP
		// This is to cap the framerate at some upper bound.
		uint64_t sleepBegin = SDL_GetPerformanceCounter();
		float frameTime = GetSecondsElapsed(frameBegin, sleepBegin);

		// renderTime = the time taken to render the *last* frame. Should be a stable-ish
		// estimation of the renderTime for the upcoming frame. -1 because we'd rather be
		// slightly over than slightly under, and sleep is on average 1ms longer than our
		// requested time under testing.
		int32_t sleepTime = (int32_t)floor(target_frame_milliseconds - renderTime - frameTime) -1;
		if(sleepTime > 1)
		{
			SDL_Delay(sleepTime);
		}

		//spin for the rest
		while (GetSecondsElapsed(frameBegin, SDL_GetPerformanceCounter()) < target_frame_milliseconds - renderTime);

		uint64_t renderBegin = SDL_GetPerformanceCounter();
		// std::cout << "Requested Sleep: " << sleepTime << "ms | Actual Sleep: " << GetSecondsElapsed(sleepBegin, renderBegin) << "\n";
		

		// Render
		renderer.Render(game.GetRenderBuffer());



		uint64_t frameEnd = SDL_GetPerformanceCounter();
		renderTime = GetSecondsElapsed(renderBegin, frameEnd);

		if (debugFrameCounter++ % target_render_fps == 0)
		{
			float debugDisplayTime = GetSecondsElapsed(debugBegin, frameEnd);
			float fps = 1.0f / debugDisplayTime * 1000.0f * target_render_fps;
			std::cout << "Average Framerate:" << fps << ". Physics updates/frame: " << (float)debugPhysicsUpdates / target_render_fps << "." << std::endl;
			debugBegin = frameEnd;
			debugPhysicsUpdates = 0;
		}

		physicsTickAccumulator += frameEnd - frameBegin;
	}

	// Shutdown

	SDL_Quit();
	return EXIT_SUCCESS;
}