// H2Physics.c
//

#include "../game/q_shared.h"
#include "../qcommon/q_Physics.h"

#include <stdint.h>

H2COMMON_API void BounceVelocity(vec3_t in, vec3_t normal, vec3_t out, float elasticity)
{
	double v5; // st7@1

	v5 = -((normal[2] * in[2] + in[1] * normal[1] + in[0] * normal[0]) * elasticity);
	out[0] = v5 * normal[0] + in[0];
	out[1] = v5 * normal[1] + in[1];
	out[2] = v5 * normal[2] + in[2];
}

H2COMMON_API qboolean BoundVelocity(float *vel)
{
	float *v1; // ecx@1
	int v2; // edx@1
	signed int v3; // esi@1
	double v4; // st7@2

	v1 = vel;
	v2 = 0;
	v3 = 3;
	do
	{
		v4 = *v1;
		if (v4 <= -0.1 || v4 >= 0.1)
		{
			if (v4 <= 2000.0)
			{
				if (v4 < -2000.0)
					*(uint32_t *)v1 = -990248960;
			}
			else
			{
				*(uint32_t *)v1 = 1157234688;
			}
		}
		else
		{
			*(uint32_t *)v1 = 0;
			++v2;
		}
		++v1;
		--v3;
	} while (v3);
	return v2 != 3;
}