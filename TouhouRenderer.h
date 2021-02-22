#pragma once

#include <SDL.h>

#include "Renderer.h"
#include "TouhouState.h"


class TouhouRenderer : public Renderer
{
public:
	TouhouRenderer(std::string windowName, int windowWidth, int windowHeight);
	~TouhouRenderer();

	void Render(const TouhouState* renderState);

private:
	SDL_Texture* _image;

	void BlitBackground() const;

};