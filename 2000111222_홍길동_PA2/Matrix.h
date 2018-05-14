#pragma once

#include "Vector.h"

class MATRIX
{
public:
	float m[4][4];

	MATRIX(float mIn[4][4] = NULL)
	{
		if (mIn)
			set(mIn);
		else
			memset(m, 0, sizeof(float) * 16);
	}

	void set(float glMatrix[4][4])
	{
		memcpy(m, glMatrix, sizeof(float) * 16);
	}

	float* matrix()
	{
		return (float*)m;
	}
};