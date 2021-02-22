#include <SDL.h>
#include <iostream>
#include <string>
#include <cassert>
#include <complex>
#include <immintrin.h>
#include <thread>

#include "MandelbrotDisplay.h"
#include "Color.h"

MandelbrotDisplay::MandelbrotDisplay(std::string windowTitle, int screenWidth, int screenHeight) :
	_screenWidth(screenWidth), _screenHeight(screenHeight)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		ExitWithSDLError("Error initializing SDL");

	_window = SDL_CreateWindow(windowTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
	if (!_window)
		ExitWithSDLError("Error creating window");

	_screen = SDL_GetWindowSurface(_window);
	if (!_screen)
		ExitWithSDLError("Error getting surface");

	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
	if(!_renderer)
		ExitWithSDLError("Error getting renderer from window");

	_texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA32, 
		SDL_TEXTUREACCESS_STREAMING, _screenWidth, _screenHeight);
	if (!_texture)
		ExitWithSDLError("Error creating texture");

	_pixels = std::vector<Color>(
		static_cast<size_t>(_screenWidth) *
		static_cast<size_t>(_screenHeight));
}

MandelbrotDisplay::~MandelbrotDisplay()
{
	SDL_DestroyTexture(_texture);
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window); // SDL destroys the surface for us when we do this
	SDL_Quit();
}


void MandelbrotDisplay::ClearToColor(const Color color)
{
	SDL_FillRect(_screen, NULL, color.value);
}

void MandelbrotDisplay::Render(const std::shared_ptr<State> state)
{
	//ClearToColor(state->backgroundColor);

#if MULTITHREADED_RENDERING
	const int MAX_THREAD_COUNT = 16;
	const int TILED_ROWS = 1;

	int tileHeight = _screenHeight / TILED_ROWS;
	int tileWidth = _screenWidth / MAX_THREAD_COUNT * TILED_ROWS;

	_scaleX = ((double)state->right - state->left) / ((double)_screenWidth);
	_scaleY = ((double)state->bottom - state->top) / ((double)_screenHeight);

	std::thread threads[MAX_THREAD_COUNT];
	int thread = 0;
	for (size_t row = 0; row < TILED_ROWS; row++)
	{
		int startY = row * tileHeight;
		int endY = ((row + 1) * tileHeight);

		for (size_t col = 0; col < MAX_THREAD_COUNT / TILED_ROWS; col++)
		{
			int startX = col * tileWidth;
			int endX = ((col + 1) * tileWidth);

#if SIMD_RENDERING
			threads[thread++] = std::thread(&MandelbrotDisplay::FillPixelsAVX64,
				this, startY, endY, startX, endX, *state);
#else
				threads[thread++] = std::thread(&MandelbrotDisplay::FillPixels,
				this, startY, endY, startX, endX, *state);
#endif // SIMD_RENDERING
		}
	}

	for (size_t i = 0; i < MAX_THREAD_COUNT; i++)
	{
		threads[i].join();
	}

#else
	_scaleX = ((double)state->right - state->left) / ((double)_screenWidth);
	_scaleY = ((double)state->bottom - state->top) / ((double)_screenHeight);


#if SIMD_RENDERING
	FillPixelsAVX64(0, _screenHeight, 0, _screenWidth, *state);
#else
	FillPixels(0, _screenHeight, 0, _screenWidth, *state);
#endif // SIMD_RENDERING
#endif // MULTITHREADED_RENDERING

	SDL_UnlockSurface(_screen);
	BlitPixelsToScreen();
	SDL_LockSurface(_screen);

	SDL_UpdateWindowSurface(_window);

}


inline void MandelbrotDisplay::FillPixels(const int& topPixel, const int& bottomPixel,
	const int& leftPixel, const int& rightPixel, const State& state)
{
	for (auto y = topPixel; y < bottomPixel; y++)
	{
		for (auto x = leftPixel; x < rightPixel; x++)
		{
			std::complex<double> c(x * _scaleX + state.left, y * _scaleY + state.top);
			std::complex<double> z(0, 0);

			int n = -1;
			//while (abs(z) < 2.0 && n < MAX_ITERATIONS)
			while (z.real() * z.real() + z.imag() * z.imag() < 4.0 && n < MAX_ITERATIONS)
			{
				z = (z * z) + c;
				n++;
			}
			int pixelLocation = (y * _screenWidth) + x;
			_pixels[pixelLocation] = IterationsToColor(state, n);
		}
	}
}

void MandelbrotDisplay::FillPixelsAVX64(const int& topPixel, const int& bottomPixel,
	const int& leftPixel, const int& rightPixel, const State& state)
{
	//@NOTE: four-wide double. Remake and test with 8-wide float?

	__m256d xScale = _mm256_set1_pd(_scaleX);
	__m256d xOffsets = _mm256_mul_pd(_mm256_set_pd(0, 1, 2, 3), xScale);
	__m256d stateLeft = _mm256_set1_pd(state.left);
	__m256d xStride = _mm256_set1_pd(_scaleX * 4.0);

	// No need for intrinsics in Y
	double yPosSingle = state.top;

	for (int y = topPixel; y < bottomPixel; y++)
	{
		__m256d xPos = _mm256_add_pd(stateLeft, xOffsets);
		__m256d cImag = _mm256_set1_pd(yPosSingle);

		for (int x = leftPixel; x < rightPixel; x += 4)
		{
			//x * scaleX + state.left;
			// x*scaleX, x+1*scaleX, x+2*scaleX, x+3*ScaleX
			__m256d cReal = xPos;

			__m256d zReal = _mm256_setzero_pd();
			__m256d zImag = _mm256_setzero_pd();

			__m256i mask = _mm256_set1_epi64x(-1);
			// int n = -1;
			__m256i iterations = _mm256_setzero_si256();
			while (_mm256_movemask_pd(_mm256_castsi256_pd(mask)) > 0)
			{
				__m256d zRealSq = _mm256_mul_pd(zReal, zReal);
				__m256d zImagSq = _mm256_mul_pd(zImag, zImag);

				// zRealSq - zImagSq + cReal
				__m256d zNewReal = _mm256_add_pd(_mm256_sub_pd(zRealSq, zImagSq), cReal);

				// zReal * zImag * two + cImag 
				__m256d two = _mm256_set1_pd(2.0);
				__m256d zNewImag = _mm256_fmadd_pd(_mm256_mul_pd(zReal, zImag), two, cImag);

				zReal = zNewReal;
				zImag = zNewImag;

				// (zRealSq + zImagSq < 4.0)
				__m256d sum = _mm256_add_pd(zRealSq, zImagSq);
				__m256d mask1 = _mm256_cmp_pd(sum, _mm256_set1_pd(4.0), _CMP_LT_OQ);

				// n < MAX_ITERATIONS
				__m256i maxInterations = _mm256_set1_epi64x(MAX_ITERATIONS); // -1 because we are comparing >, not <
				__m256i mask2 = _mm256_cmpgt_epi64(maxInterations, iterations); // Why is there no _mm_cmplt_epi64?

				// while (zRealSq + zImagSq < 4.0 && n < MAX_ITERATIONS)	
				mask = _mm256_and_si256(_mm256_castpd_si256(mask1), mask2); // Combine mask1 and mask2
				__m256i one = _mm256_set1_epi64x(1);

				__m256i maskedOne = _mm256_and_si256(mask, one); // int 1 in every slot where condition is met

				iterations = _mm256_add_epi64(iterations, maskedOne); // n++;
			}

			int pixelPos = y * _screenWidth + x;
			_pixels[pixelPos + 0] = IterationsToColor(state, iterations.m256i_i64[3]);
			_pixels[pixelPos + 1] = IterationsToColor(state, iterations.m256i_i64[2]);
			_pixels[pixelPos + 2] = IterationsToColor(state, iterations.m256i_i64[1]);
			_pixels[pixelPos + 3] = IterationsToColor(state, iterations.m256i_i64[0]);
		}

		yPosSingle += _scaleY;
	}
}

void MandelbrotDisplay::BlitPixelsToScreen()
{
	//@TODO: What's the modern cpp way of doing this?
	uint32_t* dest = (uint32_t*)_screen->pixels;
	for (size_t i = 0; i < _pixels.size(); i++)
	{
		/*
		Color destColor;
		destColor.value = *dest;
		dest[i] = Color::AlphaBlend(_pixels[i], destColor).value;
		*/

		dest[i] = _pixels[i].value;
	}
}

Color MandelbrotDisplay::IterationsToColor(const State& state, int iterations)
{
	Color color;
	color.value = 0;
	if (iterations == 0) return color;

	double iterRange = (double)iterations / (double)MAX_ITERATIONS;
	double t = -1 * log(iterRange);

	color = state.fractalColor;
	color.red = static_cast<uint8_t>(255 * t);

	return color;
}
