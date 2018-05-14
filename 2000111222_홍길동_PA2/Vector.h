#pragma once

#include <cmath>

class VECTOR3D
{
public:

	VECTOR3D()
		:x(0.0f), y(0.0f), z(0.0f) {}

	VECTOR3D(float x, float y, float z)
		:x(x), y(y), z(z) {}

	float Magnitude();
	float InnerProduct(VECTOR3D v);
	VECTOR3D CrossProduct(VECTOR3D v);
	void Normalize();

	VECTOR3D operator+(VECTOR3D v);
	VECTOR3D& operator+=(VECTOR3D v);

	VECTOR3D operator-(VECTOR3D v);
	VECTOR3D operator*(float val);

	VECTOR3D operator/(float val);
	VECTOR3D operator/=(float val);

	float x;
	float y;
	float z;
};

VECTOR3D operator*(float val, VECTOR3D v);