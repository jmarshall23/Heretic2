// H2Motion.c
//

#include "../game/q_shared.h"

H2COMMON_API float GetDistanceOverTime(float a1, float a2, float a3)
{
	return a2 * a3 * a3 * 0.5 + a1 * a3;
}

H2COMMON_API void GetPositionOverTime(vec3_t a1, vec3_t a2, vec3_t a3, float a4, vec3_t a5)
{
	for (int i = 0; i < 3; i++)
	{
		a5[i] = GetDistanceOverTime(a2[i], a3[i], a4) + a1[i];
	}
}

H2COMMON_API void GetVelocityOverTime(vec3_t velocity, vec3_t accel, float time, vec3_t output)
{
	output[0] = time * accel[0] + velocity[0];
	output[1] = time * accel[1] + velocity[1];
	output[2] = time * accel[2] + velocity[2];
}