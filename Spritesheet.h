#pragma once
#include <SDL_render.h>

class Sprite
{
public:
	Sprite(SDL_Texture* texture, int width, int height);

	virtual void Render(SDL_Renderer* renderer) const;

protected:

	const SDL_Texture* _tex;
	SDL_Rect _piece;

};

class AnimatingSpritesheet : public Sprite
{


private:
	const int _width;
	const int _height;
};