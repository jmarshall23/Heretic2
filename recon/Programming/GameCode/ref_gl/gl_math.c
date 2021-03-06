// gl_math.c
//

#include "gl_local.h"

void GL_LerpVert(vec3_t newPoint, vec3_t oldPoint, vec3_t interpolatedPoint, float move[3], float frontv[3], float backv[3])
{
	interpolatedPoint[0] = move[0] + oldPoint[0] * backv[0] + newPoint[0] * frontv[0];
	interpolatedPoint[1] = move[1] + oldPoint[1] * backv[1] + newPoint[1] * frontv[1];
	interpolatedPoint[2] = move[2] + oldPoint[2] * backv[2] + newPoint[2] * frontv[2];
}
