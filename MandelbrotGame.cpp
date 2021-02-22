#include <memory>
#include <iostream>

#include "Game.h"

MandelbrotGame::MandelbrotGame()
{
	oldState = InitState();
	nextState = InitState();

	_sinTime = 0;
}

std::shared_ptr<State> MandelbrotGame::InitState()
{
	auto state = std::make_shared<State>();
	state->top = 1;
	state->bottom = -1;
	state->left = -2;
	state->right = 1;

	state->backgroundColor.value = 0xaaaaaaFF;
	state->fractalColor.value = 0x40559AFF;

	return state;
}

void MandelbrotGame::Update(float deltaTime)
{
	_sinTime += deltaTime / COLOR_CHANGE_SPEED / TAU;

	// flip double buffer state
	oldState.swap(nextState);

	Color newFractal;
	newFractal.value = 0x40559AFF;
	for (size_t i = 0; i < 3; i++)
	{
		uint8_t sinColorMod = static_cast<uint8_t>(sin(_sinTime) * 32.0f);
		newFractal.BGR[i] = (newFractal.BGR[i] + sinColorMod) % UINT8_MAX;
	}
	newFractal.alpha = UINT8_MAX;
	

	//nextState->fractalColor = newFractal;	
}