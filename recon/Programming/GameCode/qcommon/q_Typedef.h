#ifndef Q_TYPEDEF_H
#define Q_TYPEDEF_H

typedef float vec_t;
typedef vec_t vec2_t[2];
typedef vec_t vec3_t[3];
typedef double vec3d_t[3];
typedef vec_t vec5_t[5];

typedef float matrix3_t[3][3];
typedef float matrix3d_t[3][3];

typedef	int	fixed4_t;
typedef	int	fixed8_t;
typedef	int	fixed16_t;

// jmarshall
typedef float matrix4_t[4][4];
// jmarshall end

typedef unsigned char 		byte;
#ifndef __cplusplus
typedef enum {false, true}	qboolean;
#else
typedef int qboolean;
#endif

#define edict_s edict_t
#define sfx_s sfx_t
#define client_entity_s client_entity_t

typedef struct paletteRGBA_s
{
	union
	{
		struct
		{
			byte r,g,b,a;
		};
		unsigned c;
		byte c_array[4];
	};
} paletteRGBA_t;

#endif