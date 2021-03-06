//
// l3dslib.h: header file for loading triangles from a 3DS triangle file
//
void DefaultNodesList(mesh_node_t **nodesList, int *num_mesh_nodes, int *numtriangles);

void Load3DSTriangleList (char *filename, triangle_t **pptri, int *numtriangles, mesh_node_t **ppmnodes, int *num_mesh_nodes);
void LoadASC(char *fileName, triangle_t **triList, int *triangleCount, mesh_node_t **ppmnodes, int *num_mesh_nodes);
