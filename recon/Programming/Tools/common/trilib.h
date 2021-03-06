//
// trilib.h: header file for loading triangles from an Alias triangle file
//

#include "fmodel.h"

#define MAXTRIANGLES	MAX_FM_TRIANGLES

typedef struct 
{
	vec3_t		verts[3];
#ifdef _QDATA
	int			indicies[3];
	float		uv[3][2];
	qboolean	HasUV;
#endif
} triangle_t;

#define NUM_CLUSTERS 8

typedef struct
{
	char	name[64];
	byte	tris[MAXTRIANGLES>>3];
	byte	verts[MAX_FM_VERTS>>3];
	int		start_glcmds, num_glcmds;

	int *clusters[NUM_CLUSTERS];
	struct IntListNode_s *vertLists[NUM_CLUSTERS];
	int num_verts[NUM_CLUSTERS + 1];
	int new_num_verts[NUM_CLUSTERS + 1];
	qboolean clustered;
} mesh_node_t;

void LoadTriangleList (char *filename, triangle_t **pptri, int *numtriangles, mesh_node_t **ppmnodes, int *num_mesh_nodes);

