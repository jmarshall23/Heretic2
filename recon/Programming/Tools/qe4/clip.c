// *****
// clip.c
// *****

#include "clip.h"



static vec3_t	clipDown;
static vec3_t	clipUp;

void Begin_Clip (int firstx, int firsty)
{
	
	XY_ToGridPoint( firstx, firsty, clipDown);
	return;
}

void Finish_Clip (int lastx, int lasty)
{
	brush_t		*theBrush;
	face_t		*curFace;
	face_t		*theFace;



	UNDO_StartBrushEdit("&Undo Clip");
	theBrush = selected_brushes.next;
	XY_ToGridPoint (lastx, lasty, clipUp);
//need to put in routine to make sure clip points intersect brush!
// possibly a modification of Brush_Ray	

	curFace = theBrush->brush_faces;
	while (curFace->next != NULL)
	{
		curFace = curFace->next;
	}
	theFace = Face_Alloc();
	VectorCopy(clipDown,theFace->planepts[0]);
	VectorCopy(clipUp,theFace->planepts[1]);
	theFace->planepts[2][0] = clipUp[0];
	theFace->planepts[2][1] = clipUp[1];
	theFace->planepts[2][2] = (clipUp[2] + 10);
	theFace->texdef = g_qeglobals.d_texturewin.texdef;


	curFace->next = theFace;
	Brush_Build(theBrush);
	UNDO_FinishBrushEdit(NULL);
	Sys_UpdateWindows (W_ALL);
	return;
}
/*	else
	{
		Sys_Status ("No intersection between clipping segment and selected brush!", 0);
		Sys_Beep ();
	}
	return;
*/

void Draw_Clip(vec3_t start, vec3_t finish)
{
	glColor3f(1.0,0.0,1.0);
	glBegin(GL_LINES);
	glVertex2f(start[0], start[1]);
	glVertex2f(finish[0], finish[1]);
	glEnd();
	return;
}
