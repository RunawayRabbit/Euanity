
#include "TouhouRenderer.h"


TouhouRenderer::TouhouRenderer(std::string windowTitle, int screenWidth, int screenHeight) :
	Renderer{windowTitle, screenWidth, screenHeight}
{
	// Stub

	_image = LoadBitmap("foo.bmp");
}

TouhouRenderer::~TouhouRenderer()
{
	// Stub
}

void TouhouRenderer::Render(const TouhouState* renderState)
{
	SDL_RenderClear(_renderer);

	int width = 743;
	int height = 495;

	SDL_Rect rect{ renderState->picX - width/2, renderState->picY - height/2, width, height };

	SDL_RenderCopy(_renderer, _image, 0, &rect );


	SDL_RenderPresent(_renderer);
}

