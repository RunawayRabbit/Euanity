#pragma once

#include <SDL.h>
#include <string>
#include <vector>
#include <memory>
#include <iostream>

#include "Color.h"
#include "State.h"

class Renderer
{
public:
	// virtual void Render(const std::shared_ptr<RenderState> state) = 0;
	virtual ~Renderer();
	
	Renderer() = delete;
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;

protected:
	Renderer(std::string windowName, int windowWidth, int windowHeight);

	SDL_Texture* LoadBitmap(const std::string path) const;

	void ExitWithSDLError(const std::string errorMessage) const;

	SDL_Window* _window;
	SDL_Surface* _screenSurf;
	SDL_Renderer* _renderer;
	SDL_Texture* _screenTex;

	const int _screenWidth;
	const int _screenHeight;

};