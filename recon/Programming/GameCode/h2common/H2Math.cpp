// H2Math.c
//

#include "../game/q_shared.h"

#define VectorClear(x) {x[0] = x[1] = x[2] = 0; }

H2COMMON_API vec_t VectorNormalize(vec3_t v);

/*
=================
DistanceSquared
=================
*/
H2COMMON_API float DistanceSquared(vec3_t p1, vec3_t p2) {
	vec3_t	v;
	float a, b, c, result;

	VectorSubtract(p2, p1, v);
	
	a = v[0] * v[0];
	b = v[1] * v[1];
	c = v[2] * v[2];

	result = a + b + c;
	return result;
}

/*
=================
AnglesToAxis
=================
*/
H2COMMON_API void AnglesToAxis(vec3_t angles, vec3_t axis[3]) {
	vec3_t	right;

	// angle vectors returns "right" instead of "y axis"
	AngleVectors(angles, axis[0], right, axis[2]);
	VectorSubtract(vec3_origin, right, axis[1]);
}

H2COMMON_API float Vec3Normalize(vec3_t v1)
{
	return VectorNormalize(v1);
}

H2COMMON_API float GetTimeToReachDistance(float a1, float a2, float a3)
{
	double v4; // st7@1
	char v5 = 0.0f; // c3@1
	float v6; // st7@2
	double result; // st7@2
	float v8; // [sp+Ch] [bp+Ch]@2

	v4 = a1;
	if (v5)
	{
		if (v4 == 0.0)
			result = 0.0;
		else
			result = a3 / a1 * 1000.0;
	}
	else
	{
		v6 = sqrt(v4 * a1 + a2 * a3 + a2 * a3);
		v8 = (v6 - a1) * 1000.0 / a2;
		result = (-a1 - v6) * 1000.0 / a2;
		if (result <= v8)
			result = v8;
	}
	return result;
}

H2COMMON_API float Approach(float curr, float dest, float rate)
{
	double result; // st7@1
	double v4; // st6@1
	float v5; // [sp+4h] [bp+4h]@1

	result = curr;
	v4 = dest - curr;
	v5 = v4;
	if (v4 < 0.0)
		result = result - rate;
	if (v4 > 0.0)
		result = result + rate;
	return result;
}

H2COMMON_API float VectorDistance(vec3_t p1, vec3_t p2)
{
	vec3_t dir;
	VectorSubtract(p2, p1, dir);
	return VectorLengthSquared(dir);
}