
#include "Color.h"

Color Color::AlphaBlend(const Color& source, const Color& dest)
{
	Color retValue;
	float srcAlpha = static_cast<float>(source.alpha) / 255.0f;

	for (size_t i = 0; i < 3; i++)
	{
		retValue.BGR[i] = (uint8_t)
			((source.BGR[i] * srcAlpha) +
				dest.BGR[i] * (1.0f - srcAlpha));
	}

	retValue.alpha = 255;
	return retValue;
}