#pragma once

#include <math.h>
#include <string>

class V2
{
public:
	union
	{
		struct
		{
			float x, y;
		};
		float e[2];
	};

	inline V2 Normalized();
	inline float Length();
	inline float LengthSq();
	
	inline static float Dot(V2 a, V2 b);

	inline V2 Rotate(float radians);
	inline V2 Rot90();
	inline V2 Rot180();
	inline V2 Rot90Neg();

	inline float AngleBetween(V2 a, V2 b);
	inline float AngleBetweenUnorm(V2 a, V2 b);
	
	inline V2 operator+(V2 b);

	inline V2 operator-();
	inline V2 operator-(V2 b);

	inline V2 operator*(float scale);
	inline V2 operator*(V2 scale);

	inline V2 operator/(float scale);

	inline static V2 FromAngle(float radians);

	inline std::string ToString() const;
};

V2 V2::operator+(V2 b)
{
	V2 ret;
	ret.x = x + b.x;
	ret.y = y + b.y;

	return ret;
}

V2 V2::operator-()
{
	V2 ret;
	ret.x = -x;
	ret.y = -y;

	return ret;
}

V2 V2::operator-(V2 b)
{
	return (*this) + (-b);
}

// Scale
V2 V2::operator*(float scale)
{
	V2 ret;
	ret.x = (*this).x * scale;
	ret.y = (*this).y * scale;

	return ret;
}

// component-wise scaling
V2 V2::operator*(V2 scale)
{
	V2 ret;
	ret.x = (*this).x * scale.x;
	ret.y = (*this).y * scale.y;

	return ret;
}

V2 V2::operator/(float scale)
{
	return (*this) * (1.0f / scale);
}

V2 V2::Normalized()
{
	return (*this) * (1.0f / this->Length());
}

float V2::Dot(V2 a, V2 b)
{
	return a.x * b.x
		+ a.y * b.y;
}

V2 V2::Rot90()
{
	return V2{ y, -x };
}

V2 V2::Rot180()
{
	return -*this;
}

V2 V2::Rot90Neg()
{
	return V2{ -y, x };
}

// Use only for normalized vectors. Otherwise use AngleBetweenUnorm.
float V2::AngleBetween(V2 a, V2 b)
{
	return (float)acos(V2::Dot(a, b));
}

// Use for unnormalized vectors
float V2::AngleBetweenUnorm(V2 a, V2 b)
{
	return AngleBetween(a.Normalized(), b.Normalized());
}

float V2::LengthSq()
{
	return Dot(*this, *this);
}

float V2::Length()
{
	return (float)sqrt(this->LengthSq());
}

V2 V2::FromAngle(float radians)
{
	return { (float)cos(radians), (float)sin(radians) };
}

V2 V2::Rotate(float radians)
{
	V2 ret;
	ret.x = (float)(x * cos(radians) - y * sin(radians));
	ret.y = (float)(x * sin(radians) + y * cos(radians));

	return ret;
}

std::string V2::ToString() const
{
	return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
}