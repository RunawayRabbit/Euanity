
#include "Game.h"


Game::Game() :
	_input(InputHandler(*this))
{
	isRunning = true;
}

void Game::ProcessSDLInput()
{
	_input.ProcessSDLInput();
}