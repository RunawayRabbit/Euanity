

#include "Spritesheet.h"
#include <SDL.h>

Spritesheet::Spritesheet(SDL_Texture* texture, int width, int height) :
	_tex(texture), _width(width), _height(height)
{
	_piece = { 0, 0, width, height };
}

void Spritesheet::Render(SDL_Renderer* renderer) const
{

	SDL_Rect position = { 0, 0, _width, _height };

	// Casting away const. Sorry.
	auto _texWithoutConst = (SDL_Texture*)_tex;
	SDL_RenderCopy(renderer, _texWithoutConst, &_piece, &position);
}