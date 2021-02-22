#pragma once

#include <memory>

#include "Game.h"
#include "TouhouState.h"
#include "InputBuffer.h"

class TouhouGame : public Game
{
public:
	TouhouGame();

	void Simulate( float deltaTime );

	TouhouState* GetRenderBuffer();

private:
	std::unique_ptr<TouhouState> previousState;
	std::unique_ptr<TouhouState> currentState;
};