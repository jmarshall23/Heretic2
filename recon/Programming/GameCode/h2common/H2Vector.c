// H2Vector.c
//

#include "../game/q_shared.h"
#include "../qcommon/Vector.h"
#include "../qcommon/Matrix.h"
#include "../qcommon/random.h"

H2COMMON_API vec3_t vec3_right = { 1.0f, 0.0f, 0.0f };
H2COMMON_API vec3_t vec3_up = { 0.0f, 0.0f, 1.0f };

H2COMMON_API void AnglesFromDir(vec3_t direction, vec3_t angles)
{
	angles[1] = atan2(direction[1], direction[0]);
	angles[0] = asin(direction[2]);
	angles[2] = 0;
}

H2COMMON_API void AnglesFromDirI(vec3_t direction, vec3_t angles)
{
	angles[1] = atan2(direction[1], direction[0]);
	angles[0] = asin(direction[2]);
	angles[2] = -angles[1];
}

H2COMMON_API void Create_rand_relect_vect(vec3_t in, vec3_t out)
{
	double v3; // st7@3
	float v4; // ST28_4@5
	double v5; // st7@5
	float v6; // [sp+4h] [bp-18h]@3
	vec3_t angles; // [sp+10h] [bp-Ch]@3

	angles[2] = 0.0f;

	if (VectorNormalize(in) >= 0.00050000002)
	{
		in[0] = in[0] * -1.0;
		in[1] = in[1] * -1.0;
		v3 = in[2] * -1.0;
		in[2] = v3;
		AnglesFromDir(in, angles);
		flrand(0.0, 0.5);
		flrand(0.0, 0.30000001);
		v6 = v3;
		if (irand(0, 1))
			v6 = -v6;
		v4 = v3;
		v5 = v4;
		if (irand(0, 1))
			v5 = -v5;
		angles[0] = angles[0] + v6;
		angles[1] = angles[1] + v5;
		DirFromAngles(angles, out);
	}
	else
	{
		out[0] = 1;
		out[1] = 0.0f;
		out[2] = 0.0f;
	}
}

H2COMMON_API void AnglesFromDirAndUp(vec3_t direction, vec3_t up, vec3_t angles)
{
	float v3; // st7@1
	float v4; // st7@1
	float v5; // st7@1
	float v6; // st6@1
	float v7; // st7@1
	vec3_t v8; // [sp+Ch] [bp-84h]@1
	//float v9; // [sp+10h] [bp-80h]@1
	//float v10; // [sp+14h] [bp-7Ch]@1
	float v11; // [sp+18h] [bp-78h]@1
	float v12; // [sp+20h] [bp-70h]@1
	float v13; // [sp+28h] [bp-68h]@1
	float v14; // [sp+30h] [bp-60h]@1
	float v15; // [sp+38h] [bp-58h]@1
	vec3_t v16; // [sp+3Ch] [bp-54h]@1
	float v17; // [sp+40h] [bp-50h]@1
	float v18; // [sp+48h] [bp-48h]@1
	float v19; // [sp+4Ch] [bp-44h]@1
	float v20; // [sp+54h] [bp-3Ch]@1
	float v21; // [sp+58h] [bp-38h]@1
	int v22; // [sp+68h] [bp-28h]@1
	char v23; // [sp+6Ch] [bp-24h]@1

	angles[1] = atan2(angles[0], angles[1]);
	v3 = asin(angles[2]);
	angles[0] = v3;
	v4 = cos(v3);
	memset(&v11, 0, 0x24u);
	v14 = direction[2];
	v13 = 1;
	v11 = v4;
	v15 = v4;
	v12 = -v14;
	Matrix3MultByVec3(&v11, direction, v16);
	v5 = atan2(v16[0], v16[1]);
	memset(&v18, 0, 0x24u);
	v22 = 1;
	angles[1] = v5;
	v6 = cos(v5);
	v18 = v6;
	v21 = v6;
	v7 = sin(v5);
	v19 = v7;
	v20 = -v7;
	Matrix3MultByMatrix3(&v11, &v18, &v23);
	Matrix3MultByVec3(&v23, up, v8);
	v8[0] = 0;
	Vec3Normalize(v8);
	angles[2] = -(atan2(v8[1], v8[2]) - 1.570796370506287);
}

H2COMMON_API void RealAngleVectors(vec3_t angles, vec3_t forward, vec3_t right, vec3_t up)
{
	float v5; // st7@1
	float v6; // st6@1
	float v7; // st5@1
	float v8; // st4@4
	int result; // eax@5
	float v10; // st5@6
	float v11; // [sp+0h] [bp-8h]@1
	float v12; // [sp+4h] [bp-4h]@1
	float a1a; // [sp+Ch] [bp+4h]@1

	v5 = sin(angles[1]);
	v6 = cos(angles[1]);
	v7 = sin(angles[0]);
	v12 = cos(angles[0]);
	a1a = sin(angles[2]);
	v11 = cos(angles[2]);
	if (forward)
	{
		forward[0] = v12 * v6;
		forward[1] = v12 * v5;
		forward[2] = -v7;
	}
	if (right)
	{
		v8 = a1a * v7;
		right[0] = v11 * v5 - v8 * v6;
		right[1] = -(v8 * v5) - v11 * v6;
		right[2] = *(float *)&a1a * v12 * -1.0;
	}
	if (up)
	{
		v10 = v7 * v11;
		up[0] = v10 * v6 + a1a * v5;
		up[1] = v10 * v5 - a1a * v6;
		up[2] = v11 * v12;
	}
}

H2COMMON_API void DirAndUpFromAngles(vec3_t angles, vec3_t direction, vec3_t up)
{
	matrix3_t v4; // [sp+0h] [bp-24h]@1

	Matrix3FromAngles(angles, v4);
	Matrix3MultByVec3(v4, vec3_right, direction);
	Matrix3MultByVec3(v4, vec3_up, up);
}

H2COMMON_API void vectoangles(vec3_t vec, vec3_t angles) {
	float *v2; 
	vec3_t v3; 

	if (*vec != 0.0 || vec[1] != 0.0 || vec[2] != 0.0)
	{
		VectorNormalize2(vec, v3);
		AnglesFromDir(v3, angles);
		angles[0] = angles[0] * 57.295776;
		angles[1] = angles[1] * 57.295776;
		angles[2] = angles[2] * 57.295776;
	}
	else
	{
		angles[0] = 0;
		angles[1] = 0;
		angles[2] = 0;
	}
}

H2COMMON_API void DirFromAngles(vec3_t angles, vec3_t direction)
{
	float v4; // st7@1
	float v5; // st7@1
	float v6; // st6@1
	float v7; // [sp+4h] [bp+4h]@1
	float v8; // [sp+8h] [bp+8h]@1

	v4 = angles[1];
	v7 = cos(v4);
	direction[0] = v7;
	v8 = sin(v4);
	direction[1] = v8;
	v5 = angles[0];
	v6 = cos(v5);
	direction[1] = v8 * v6;
	direction[2] = sin(v5);
} 

H2COMMON_API void VectorAverage(vec3_t veca, vec3_t vecb, vec3_t vecc)
{
	vecc[0] = (veca[0] + vecb[0]) * 0.5;
	vecc[1] = (veca[1] + vecb[1]) * 0.5;
	vecc[2] = (veca[2] + vecb[2]) * 0.5;
}

H2COMMON_API void VectorGetOffsetOrigin(vec3_t off, vec3_t org, vec_t degree, vec3_t out)
{
	float v4; // ST10_4@1
	matrix3_t v7; // [sp+8h] [bp-24h]@1

	v4 = degree * 0.017453292;
	CreateYawMatrix(v7, v4);

	Matrix3MultByVec3(v7, off, out);
	out[0] = org[0] + out[0];
	out[1] = org[1] + out[1];
	out[2] = org[2] + out[2];
}

H2COMMON_API vec_t VectorSeparation(vec3_t a1, vec3_t a2)
{
	float v2; // st7@1
	float v3; // st6@1
	float v4; // st5@1

	v2 = *a1 - *a2;
	v3 = a1[1] - a2[1];
	v4 = a1[2] - a2[2];
	return sqrt(v4 * v4 + v2 * v2 + v3 * v3);
}

H2COMMON_API void VectorRandomCopy(vec3_t a1, vec3_t a2, float a3)
{
	float v3; // ST28_4@1

	v3 = -a3;
	a2[0] = flrand(v3, a3) + a1[0];
	a2[1] = flrand(v3, a3) + a1[1];
	a2[2] = flrand(v3, a3) + a1[2];
}

H2COMMON_API void VectorRandomAdd(vec3_t a1, vec3_t a2, vec3_t a3)
{
	a3[0] = flrand(-a2[0], a2[0]) + a1[0];
	a3[1] = flrand(-a2[1], a2[1]) + a1[1];
	a3[2] = flrand(-a2[2], a2[2]) + a1[2];
}

H2COMMON_API float vhlen(vec3_t p1, vec3_t p2)
{
	float v2; // st7@1
	float v3; // st6@1

	v2 = p1[0] - p2[0];
	v3 = p1[1] - p2[1];
	return sqrt(v3 * v3 + v2 * v2);
}

H2COMMON_API qboolean Vec3IsZeroEpsilon(vec3_t in)
{
	return in[0] < 0.00050000002 && in[0] > -0.00050000002
		&& in[1] < 0.00050000002 && in[1] > -0.00050000002
		&& in[2] < 0.00050000002 && in[2] > 0.00050000002;
}
