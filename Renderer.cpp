
#include "Renderer.h"

Renderer::Renderer(std::string windowTitle, int screenWidth, int screenHeight) :
	_screenWidth(screenWidth), _screenHeight(screenHeight)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		ExitWithSDLError("Error initializing SDL");

	_window = SDL_CreateWindow(windowTitle.c_str(),
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		screenWidth, screenHeight, SDL_WINDOW_SHOWN);
	if (!_window)
		ExitWithSDLError("Error creating window");


	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
	if (!_renderer)
		ExitWithSDLError("Error getting renderer from window");


	_screenSurf = SDL_GetWindowSurface(_window);
	if (!_screenSurf)
		ExitWithSDLError("Error getting surface");

	_screenTex = SDL_CreateTextureFromSurface(_renderer, _screenSurf);
	if (!_screenTex)
		ExitWithSDLError("Error converting Surface to Texture");
	
	// We might not need this?
	//SDL_FreeSurface(_screenSurf);

	SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);

}

Renderer::~Renderer()
{
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window); // SDL destroys the surface for us when we do this
	SDL_Quit();
	std::cout << "Destroyed renderer successfully";
}

SDL_Texture* Renderer::LoadBitmap(const std::string path) const
{
	SDL_Surface* surf = SDL_LoadBMP(path.c_str());
	if (!surf)
	{
		ExitWithSDLError("Failed to load " + path + ".\n");
	}

	SDL_Texture* tex = SDL_CreateTextureFromSurface(_renderer, surf);
	if (!tex)
	{
		SDL_FreeSurface(surf);
		SDL_DestroyTexture(tex);
		ExitWithSDLError("Failed to convert " + path + " to a texture.\n");
	}

	SDL_FreeSurface(surf);

	return tex;
}

void Renderer::ExitWithSDLError(const std::string errorMessage) const
{
	std::cout << errorMessage << ": " << SDL_GetError() << std::endl;
	system("pause");
}