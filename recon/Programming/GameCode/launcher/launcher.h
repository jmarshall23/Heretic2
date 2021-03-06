#pragma once

void __cdecl R_RenderFrame(refdef_t *refdef);
float R_GetFOV(void);

extern void(*R_RenderFrameEngine)(void *refdef);
void Com_Printf(char *format, ...);
bool R_RenderCaS(void);

float R_GetBrightness(void);

typedef enum
{
	it_skin,
	it_sprite,
	it_wall,
	it_pic,
	it_sky
} imagetype_t;

typedef struct image_s
{
	char	name[MAX_QPATH];			// game path, including extension
	imagetype_t	type;
	int		width, height;				// source image
	int		upload_width, upload_height;	// after power of two and picmip
	int		registration_sequence;		// 0 = free
	struct msurface_s	*texturechain;	// for sort-by-texture world drawing
	int		texnum;						// gl texture binding
	float	sl, tl, sh, th;				// 0,0 - 1,1 unless part of the scrap
	qboolean	scrap;
	qboolean	has_alpha;

	qboolean paletted;
} image_t;

image_t	*GL_FindImage(char *name, imagetype_t type);
extern image_t *(*GL_FindImageEngine)(char *name, imagetype_t type);
refexport_t GetRefAPI(refimport_t imp);

extern refexport_t(*GetRefAPIEngine_t)(refimport_t imp);

qboolean Vid_GetModeInfo(int *width, int *height, int mode);

extern refexport_t gl_ref;
extern refimport_t gl_imp;
