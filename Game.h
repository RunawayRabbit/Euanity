#pragma once

#include <memory>
#include <SDL_events.h>

#include "State.h"
#include "InputHandler.h"

class Game
{
public:
	Game();

	virtual void Simulate(float deltaTime) = 0;

	inline virtual void Quit() { isRunning = false; };
	inline bool IsRunning() { return isRunning; };

	void ProcessSDLInput();

	std::shared_ptr<RenderState> GetCurrentState() { return nextState; };

protected:
	InputHandler _input;

	std::shared_ptr<RenderState> oldState;
	std::shared_ptr<RenderState> nextState;

	std::shared_ptr<RenderState> InitState();

private:
	bool isRunning = true;
};


