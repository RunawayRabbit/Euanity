#pragma once

#include <SDL.h>
#include <string>
#include <memory>

#include "Renderer.h"

class MandelbrotDisplay : public Renderer
{
public:
	MandelbrotDisplay(std::string windowTitle, int screenWidth, int screenHeight);
	~MandelbrotDisplay();

	void Render(const std::shared_ptr<State> state);

private:
	SDL_Window* _window;
	SDL_Surface* _screen;
	SDL_Renderer* _renderer;
	SDL_Texture* _texture;

	std::vector<Color> _pixels;

	const int _screenWidth;
	const int _screenHeight;

	double _scaleX;
	double _scaleY;

	const int MAX_ITERATIONS = 128;

	void ClearToColor(const Color color);

	void FillPixels(const int& topPixel, const int& bottomPixel,
		const int& leftPixel, const int& rightPixel, const State& state);
	void FillPixelsAVX64(const int& topPixel, const int& bottomPixel,
		const int& leftPixel, const int& rightPixel, const State& state);

	Color IterationsToColor(const State& state, int iterations);
	void BlitPixelsToScreen();
};