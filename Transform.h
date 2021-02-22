#pragma once

#include "V2.h"
#include "M3x3.h"

struct Transform
{
	V2 pos;

	// Sets the object's scale directly
	void SetScale(float scale);
	void SetScale(float xScale, float yScale);

	void Rotate(float radians);
	void SetRotation(float radians);

private:
	M3x3 matrix;
};
