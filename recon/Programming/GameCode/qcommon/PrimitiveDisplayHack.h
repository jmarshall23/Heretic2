#ifndef PRIMITIVEDISPLAYHACK_H
#define PRIMITIVEDISPLAYHACK_H

// **********************************************************************************
//
//	Develoment Display Aids
// 
//	DO NOT use these for production code.  These are only usuable as development aids
//	in single player mode.  These will not work in multiplayer.  Some are also not valid
//	in the render DLL (AddServerParticle).
// 
// **********************************************************************************

#if 0
#define NONCLIENT_PRIMITIVE_DISPLAY_HACK
#endif

#ifdef NONCLIENT_PRIMITIVE_DISPLAY_HACK

#include "q_ClientServer.h"
#include "q_Surface.h"

extern H2COMMON_API int *r_numparticles;
extern H2COMMON_API struct particle_s *r_particles;

H2COMMON_API void AddServerParticle(vec3_t org, paletteRGBA_t color, float scale, int type);

// WARNING!!!! don't call this from any server or game code; only valid on the client!!!!
H2COMMON_API int (*GetPoly)(struct mnode_s *node, vec3_t start, vec3_t end, Surface_t *surface);

#if 1
#define BBOX_DISPLAY_HACK
#endif

#ifdef BBOX_DISPLAY_HACK
typedef struct BBoxDisplayInfo_s
{
	qboolean display;
	float *mins;
	float *maxs;
} BBoxDisplayInfo_t;

extern H2COMMON_API BBoxDisplayInfo_t entBBoxs[MAX_EDICTS];

H2COMMON_API void InitBBoxDisplayInfo(int entNum, float *mins, float *maxs);
H2COMMON_API void EnableBBoxDisplay(int entNum);
H2COMMON_API void DisableBBoxDisplay(int entNum);

#endif	// BBOX_DISPLAY_HACK

#endif	// NONCLIENT_PRIMITIVE_DISPLAY_HACK

#endif	// PRIMITIVEDISPLAYHACK_H