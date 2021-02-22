
#include "TouhouGame.h"

// @TODO: All of this is input business.
#include <SDL_keycode.h>
#include "inputContext.h"

void callbackTest(const InputBuffer& inputs)
{
	std::cout << "Callback!" << std::endl;
}

TouhouGame::TouhouGame()
{
	// @TODO: This shouldn't be the game's business. TouhouInput class needs to exist.
	//  Also, who has ownership of inputContext now? Should be the Input system, right?
	InputContext* inputContext = new InputContext();

	inputContext->AddOneShot(SDLK_LCTRL, InputOneShot::Bomb);

	inputContext->AddToggle(SDLK_w, InputToggle::MoveUp);
	inputContext->AddToggle(SDLK_a, InputToggle::MoveLeft);
	inputContext->AddToggle(SDLK_s, InputToggle::MoveDown);
	inputContext->AddToggle(SDLK_d, InputToggle::MoveRight);

	inputContext->AddToggle(SDLK_SPACE, InputToggle::Shoot);

	_input.AddContext("Tha Context", inputContext);


	// Init render buffers

	currentState = std::make_unique<TouhouState>();
	previousState = std::make_unique<TouhouState>();

	/*
	InputCallback callback = [](const InputBuffer& inputs){std::cout << "Boop"; };
	_input.AddCallback(callback, 1);*/
}



/*
TouhouGame::~TouhouGame()
{

}*/


void TouhouGame::Simulate(float deltaTime)
{
	// flip render buffer
	std::swap(currentState, previousState);

	InputBuffer inputBuffer = _input.GetBuffer();
	_input.Dispatch();
	_input.Clear();

	//std::cout << "MousePos: (" << inputBuffer.mouseX << ", " << inputBuffer.mouseY << ")\n";
	currentState->picX = inputBuffer.mouseX;
	currentState->picY = inputBuffer.mouseY;

	for (auto& oneShot : inputBuffer.oneShots)
	{
		std::cout << (int)oneShot << std::endl;
	}

	for (auto& toggle : inputBuffer.toggles)
	{
		std::cout << (int)toggle << std::endl;
	}

}

TouhouState* TouhouGame::GetRenderBuffer()
{ 
	return currentState.get();
}