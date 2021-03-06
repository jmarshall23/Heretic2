//	clip.h
// routines for clipping brush


#include "qe3.h"
#include "undo.h"

	void Begin_Clip(int firstx, int firsty);
	void Finish_Clip(int lastx, int lasty);
	void Draw_Clip(vec3_t start, vec3_t finish);
