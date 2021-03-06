#include "qe3.h"
#include "undo.h"

typedef enum
{	//these are used so undo knows what happened at the end
	drag_op_none,	// used to signal undo hasn't started yet
	drag_op_noundo,	// such as selection, deselection
	drag_op_create,	
	drag_op_modify	//includes any edge & vertex manipulation and move
} drag_ops;

static drag_ops drag_type;

/*

  drag either multiple brushes, or select plane points from
  a single brush.

*/

qboolean	drag_ok;
vec3_t	drag_xvec;
vec3_t	drag_yvec;

static	int	buttonstate;
static	int	pressx, pressy;
static	vec3_t	pressdelta;
static	int	buttonx, buttony;


//int		num_move_points;
//float	*move_points[1024];

int		lastx, lasty;

qboolean	drag_first;


void	AxializeVector (vec3_t v)
{
	vec3_t	a;
	float	o;
	int		i;

	if (!v[0] && !v[1])
		return;
	if (!v[1] && !v[2])
		return;
	if (!v[0] && !v[2])
		return;

	for (i=0 ; i<3 ; i++)
		a[i] = fabs(v[i]);
	if (a[0] > a[1] && a[0] > a[2])
		i = 0;
	else if (a[1] > a[0] && a[1] > a[2])
		i = 1;
	else
		i = 2;

	o = v[i];
	VectorCopy (vec3_origin, v);
	if (o<0)
		v[i] = -1;
	else
		v[i] = 1;
	
}


/*
===========
Drag_Setup
===========
*/
void Drag_Setup (int x, int y, int buttons,
		   vec3_t xaxis, vec3_t yaxis,
		   vec3_t origin, vec3_t dir)
{
	trace_t	t;
	face_t	*f;
	brush_t *b;

	drag_type = drag_op_none;

	if (selected_brushes.next == &selected_brushes)
	{
		drag_type = drag_op_create;
		Sys_Status("No selection to drag\n", 0);
		return;
	}

	drag_first = true;
	g_qeglobals.d_num_move_points = 0;
	VectorCopy (vec3_origin, pressdelta);
	pressx = x;
	pressy = y;

	VectorCopy (xaxis, drag_xvec);
	AxializeVector (drag_xvec);
	VectorCopy (yaxis, drag_yvec);
	AxializeVector (drag_yvec);

	if (g_qeglobals.d_select_mode == sel_vertex)
	{
		SelectVertexByRay (origin, dir);	
		if (g_qeglobals.d_num_move_points)
		{
			drag_ok = true;
			return;
		}
	}
	if (g_qeglobals.d_select_mode == sel_edge)
	{
		SelectEdgeByRay (origin, dir);	
		if (g_qeglobals.d_num_move_points)
		{
			drag_ok = true;
			return;
		}
	}


	//
	// check for direct hit first
	//
	t = Test_Ray (origin, dir, true);
	if (t.selected)
	{
		drag_ok = true;

		if (buttons == (MK_LBUTTON|MK_CONTROL) )
		{
			Sys_Printf ("Shear dragging face\n");
			Brush_SelectFaceForDragging (t.brush, t.face, true);
		}
		else if (buttons == (MK_LBUTTON|MK_CONTROL|MK_SHIFT) )
		{
			Sys_Printf ("Sticky dragging brush\n");
			for (f=t.brush->brush_faces ; f ; f=f->next)
				Brush_SelectFaceForDragging (t.brush, f, false);
		}
		else
			Sys_Printf ("Dragging entire selection\n");
		
		return;
	}

	if (g_qeglobals.d_select_mode == sel_vertex || g_qeglobals.d_select_mode == sel_edge)
		return;

	//
	// check for side hit
	//
	if (selected_brushes.next->next != &selected_brushes)
	{
//		if (buttons == (MK_LBUTTON|MK_RBUTTON))
//		{
			//  here is the multiple resize
			b = selected_brushes.next;
			while (b->brush_faces != NULL)
			{
				Brush_SideSelect (b, origin, dir, false);
				b = b->next;
			}	
			Sys_Printf ("Multiple Brush Side Stretch\n");
			Sys_Printf ("happy now?\n");
			drag_ok = true;
			return;
//		}
//		else
//		{
//			return;
//		}
	}

	if (selected_brushes.next->owner->eclass->fixedsize)
	{
		Sys_Printf ("Can't stretch fixed size entities\n");
		return;
	}


	if (buttons & MK_CONTROL)
		Brush_SideSelect (selected_brushes.next, origin, dir, true);
	else
		Brush_SideSelect (selected_brushes.next, origin, dir, false);


	Sys_Printf ("Side stretch\n");
	drag_ok = true;
}

entity_t *peLink;

void UpdateTarget(vec3_t origin, vec3_t dir)
{
	trace_t	t;
	entity_t *pe;
	int i;
	char sz[128];

	t = Test_Ray (origin, dir, 0);

	if (!t.brush)
		return;

	pe = t.brush->owner;

	if (pe == NULL)
		return;

	// is this the first?
	if (peLink != NULL)
	{

		// Get the target id from out current target
		// if there is no id, make one

		i = IntForKey(pe, "target");
		if (i <= 0)
		{
			i = GetUniqueTargetId(1);
			sprintf(sz, "%d", i);

			SetKeyValue(pe, "target", sz);
		}

		// set the target # into our src

		sprintf(sz, "%d", i);
		SetKeyValue(peLink, "targetname", sz);

		Sys_UpdateWindows(W_ENTITY);

	}

	// promote the target to the src

	peLink = pe;
	
}


/*
===========
Drag_Reset - resets type so undo works properly
===========
*/
void Drag_Reset ()
{
	drag_type = drag_op_none;
}


/*
===========
Drag_Begin
===========
*/
void Drag_Begin (int x, int y, int buttons,
		   vec3_t xaxis, vec3_t yaxis,
		   vec3_t origin, vec3_t dir)
{
	trace_t	t;

	drag_ok = false;
	VectorCopy (vec3_origin, pressdelta);

	drag_first = true;
	peLink = NULL;

	// shift LBUTTON = select entire brush
	if (buttons == (MK_LBUTTON | MK_SHIFT))
	{
//		selection, don't want undo for this
		drag_type = drag_op_noundo;
		if (!dir[0] && !dir[1])
			Select_Ray (origin, dir, SF_ENTITIES_FIRST);	// hack for XY
		else
			Select_Ray (origin, dir, 0);
		return;
	}

	// ctrl-shift LBUTTON = select single face
	if (buttons == (MK_LBUTTON | MK_CONTROL | MK_SHIFT))
	{
		drag_type = drag_op_noundo;
		Select_Deselect ();
		Select_Ray (origin, dir, SF_SINGLEFACE);
		return;
	}

	// LBUTTON + all other modifiers = manipulate selection
	if (buttons & MK_LBUTTON)
	{
		Drag_Setup (x, y, buttons, xaxis, yaxis, origin, dir);
		if(drag_type == drag_op_none)
		{
			UNDO_StartBrushDrag ();
//			drag_type = drag_op_modify;
		}
		return;
	}

	// middle button = grab texture
	if (buttons == MK_MBUTTON)
	{
		drag_type = drag_op_noundo;
		t = Test_Ray (origin, dir, false);
		if (t.face)
		{
			g_qeglobals.d_new_brush_bottom_z = t.brush->mins[2];
			g_qeglobals.d_new_brush_top_z = t.brush->maxs[2];
			Texture_SetTexture (&t.face->texdef);
		}
		else
			Sys_Printf ("Did not select a texture\n");
		return;
	}

	// ctrl-middle button = set entire brush to texture
	if (buttons == (MK_MBUTTON|MK_CONTROL) )
	{
		drag_type = drag_op_noundo;
		t = Test_Ray (origin, dir, false);
		if (t.brush)
		{
			if (t.brush->brush_faces->texdef.name[0] == '(')
				Sys_Printf ("Can't change an entity texture\n");
			else
			{
				Brush_SetTexture (t.brush, &g_qeglobals.d_texturewin.texdef);
				Sys_UpdateWindows (W_ALL);
			}
		}
		else
			Sys_Printf ("Didn't hit a brush\n");
		return;
	}

	// ctrl-shift-middle button = set single face to texture
	if (buttons == (MK_MBUTTON|MK_SHIFT|MK_CONTROL) )
	{
		drag_type = drag_op_noundo;
		t = Test_Ray (origin, dir, false);
		if (t.brush)
		{
			if (t.brush->brush_faces->texdef.name[0] == '(')
				Sys_Printf ("Can't change an entity texture\n");
			else
			{
				t.face->texdef = g_qeglobals.d_texturewin.texdef;
				Brush_Build( t.brush );
				Sys_UpdateWindows (W_ALL);
			}
		}
		else
			Sys_Printf ("Didn't hit a brush\n");
		return;
	}
	drag_type = drag_op_noundo;

}


/*
===========
MoveSelection
===========
*/
void MoveSelection (vec3_t move)
{
	int		i;
	brush_t	*b;

	if (!move[0] && !move[1] && !move[2])
		return;

	drag_type = drag_op_modify;
	Sys_UpdateWindows (W_XY|W_CAMERA);

	//
	// dragging only a part of the selection
	//
	if (g_qeglobals.d_num_move_points)
	{
		for (i=0 ; i<g_qeglobals.d_num_move_points ; i++)
			VectorAdd (g_qeglobals.d_move_points[i], move, g_qeglobals.d_move_points[i]);

		for (b=selected_brushes.next ; b != &selected_brushes ; b=b->next)
		{
			Brush_Build( b );
			for (i=0 ; i<3 ; i++)
				if (b->mins[i] > b->maxs[i]
				|| b->maxs[i] - b->mins[i] > 4096)
					break;	// dragged backwards or fucked up
			if (i != 3)
				break;
		}

		// if any of the brushes were crushed out of existance
		// calcel the entire move
		if (b != &selected_brushes)
		{
			Sys_Printf ("Brush dragged backwards, move canceled\n");
			for (i=0 ; i<g_qeglobals.d_num_move_points ; i++)
				VectorSubtract (g_qeglobals.d_move_points[i], move, g_qeglobals.d_move_points[i]);

			for (b=selected_brushes.next ; b != &selected_brushes ; b=b->next)
				Brush_Build( b );
		}

	}
	else
	{
		//
		// if there are lots of brushes selected, just translate instead
		// of rebuilding the brushes
		//
		if (drag_yvec[2] == 0 && selected_brushes.next->next != &selected_brushes)
		{
			VectorAdd (g_qeglobals.d_select_translate, move, g_qeglobals.d_select_translate);
		}
		else
		{
			Select_Move (move);
		}
	}
}

/*
===========
Drag_MouseMoved
===========
*/
void Drag_MouseMoved (int x, int y, int buttons)
{
	vec3_t	move, delta;
	int		i;
	char	movestring[128];

	if (!buttons)
	{
		drag_ok = false;
		return;
	}
	if (!drag_ok)
		return;

	// clear along one axis
	if (buttons & MK_SHIFT)
	{
		drag_first = false;
		if (abs(x-pressx) > abs(y-pressy))
			y = pressy;
		else
			x = pressx;
	}


	for (i=0 ; i<3 ; i++)
	{
		move[i] = drag_xvec[i]*(x - pressx) 
				+ drag_yvec[i]*(y - pressy);
		move[i] = floor(move[i]/g_qeglobals.d_gridsize+0.5)*g_qeglobals.d_gridsize;
	}

	sprintf (movestring, "drag (%i %i %i)", (int)move[0], (int)move[1], (int)move[2]);
	Sys_Status (movestring, 0);

	VectorSubtract (move, pressdelta, delta);
	MoveSelection (delta);
	VectorCopy (move, pressdelta);
}

/*
===========
Drag_MouseUp
===========
*/
void Drag_MouseUp (void)
{
//	UNDO_FinishBrushEdit ();

	Sys_Status ("drag completed.", 0);
	if (g_qeglobals.d_select_translate[0] || g_qeglobals.d_select_translate[1] || g_qeglobals.d_select_translate[2])
	{
		Select_Move (g_qeglobals.d_select_translate);
		VectorCopy (vec3_origin, g_qeglobals.d_select_translate);
		Sys_UpdateWindows (W_CAMERA);
	}
	switch(drag_type)
	{
	case drag_op_modify:
		UNDO_FinishBrushDrag ("&Undo Drag");
		break;
//	case drag_op_create:
//			UNDO_FinishBrushAdd ("Undo New Brush");
	}
}
