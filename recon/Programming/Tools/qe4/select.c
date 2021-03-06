// select.c

#include "qe3.h"
#include "undo.h"

/*
===========
Test_Ray
===========
*/
#define	DIST_START	999999
trace_t Test_Ray (vec3_t origin, vec3_t dir, int flags)
{
	brush_t	*brush;
	face_t	*face;
	float	dist;
	trace_t	t;

	memset (&t, 0, sizeof(t));
	t.dist = DIST_START;

	if (! (flags & SF_SELECTED_ONLY) )
		for (brush = active_brushes.next ; brush != &active_brushes ; brush=brush->next)
		{
			if ( (flags & SF_ENTITIES_FIRST) && brush->owner == world_entity)
				continue;
			if (FilterBrush (brush))
				continue;
			face = Brush_Ray (origin, dir, brush, &dist);
			if (dist > 0 && dist < t.dist)
			{
				t.dist = dist;
				t.brush = brush;
				t.face = face;
				t.selected = false;
			}
		}
	for (brush = selected_brushes.next ; brush != &selected_brushes ; brush=brush->next)
	{
		if ( (flags & SF_ENTITIES_FIRST) && brush->owner == world_entity)
			continue;
		if (FilterBrush (brush))
			continue;
		face = Brush_Ray (origin, dir, brush, &dist);
		if (dist > 0 && dist < t.dist)
		{
			t.dist = dist;
			t.brush = brush;
			t.face = face;
			t.selected = true;
		}
	}

	// if entites first, but didn't find any, check regular

	if ( (flags & SF_ENTITIES_FIRST) && t.brush == NULL)
		return Test_Ray (origin, dir, flags - SF_ENTITIES_FIRST);

	return t;
}


/*
============
Select_Brush

============
*/
void Select_Brush (brush_t *brush)
{
	brush_t	*b;
	entity_t	*e;

	selected_face = NULL;
	e = brush->owner;
	if (e)
	{
		// select complete entity on first click
		if (e != world_entity)
		{
			for (b=selected_brushes.next ; b != &selected_brushes ; b=b->next)
				if (b->owner == e)
					goto singleselect;
			for (b=e->brushes.onext ; b != &e->brushes ; b=b->onext)
			{
				Brush_RemoveFromList (b);
				Brush_AddToList (b, &selected_brushes);
			}
		}
		else
		{
singleselect:
			Brush_RemoveFromList (brush);
			Brush_AddToList (brush, &selected_brushes);
		}

		if (e->eclass)
		{
			UpdateEntitySel(brush->owner->eclass);
		}
	}
	TEXMOD_UpdateCheck();

}

/*
============
Select_Ray

If the origin is inside a brush, that brush will be ignored.
============
*/
void Select_Ray (vec3_t origin, vec3_t dir, int flags)
{
	trace_t	t;

	t = Test_Ray (origin, dir, flags);
	if (!t.brush)
		return;

	if (flags == SF_SINGLEFACE)
	{
		selected_face = t.face;
		selected_face_brush = t.brush;
		Sys_UpdateWindows (W_ALL);
		g_qeglobals.d_select_mode = sel_brush;
		return;
	}

	// move the brush to the other list

	g_qeglobals.d_select_mode = sel_brush;

	if (t.selected)
	{		
		Brush_RemoveFromList (t.brush);
		Brush_AddToList (t.brush, &active_brushes);
	} else
	{
		Select_Brush (t.brush);
	}

	Sys_UpdateWindows (W_ALL);
	TEXMOD_UpdateCheck();

}


void Select_Delete (qboolean undo)
{
	brush_t	*brush;

	if(undo)
	{
		UNDO_StartBrushDelete("&Undo Deletion");
	}
	selected_face = NULL;
	g_qeglobals.d_select_mode = sel_brush;

	g_qeglobals.d_num_move_points = 0;
	while (selected_brushes.next != &selected_brushes)
	{
		brush = selected_brushes.next;
		Brush_Free (brush);
	}

	// FIXME: remove any entities with no brushes

	Sys_UpdateWindows (W_ALL);
	TEXMOD_UpdateCheck();
}


void Select_Deselect (void)
{
	brush_t	*b;

	g_qeglobals.d_workcount++;
	g_qeglobals.d_num_move_points = 0;

	b = selected_brushes.next;

	if (b == &selected_brushes)
	{
		if (selected_face)
		{
			selected_face = NULL;
			Sys_UpdateWindows (W_ALL);
		}
		return;
	}

	selected_face = NULL;
	g_qeglobals.d_select_mode = sel_brush;

	// grab top / bottom height for new brushes
	if (b->mins[2] < b->maxs[2])
	{
		g_qeglobals.d_new_brush_bottom_z = b->mins[2];
		g_qeglobals.d_new_brush_top_z = b->maxs[2];
	}

	selected_brushes.next->prev = &active_brushes;
	selected_brushes.prev->next = active_brushes.next;
	active_brushes.next->prev = selected_brushes.prev;
	active_brushes.next = selected_brushes.next;
	selected_brushes.prev = selected_brushes.next = &selected_brushes;	

	Sys_UpdateWindows (W_ALL);
	TEXMOD_UpdateCheck();
}

/*
============
Select_Move
============
*/
void Select_Move (vec3_t delta)
{
	brush_t	*b;
	qboolean moveorigin;

	moveorigin = true;

// actually move the selected brushes
	for (b = selected_brushes.next ; b != &selected_brushes ; b=b->next)
	{
		Brush_Move (b, delta, moveorigin);
		if (b->owner->eclass->PhysicsModel)
		{
			moveorigin = false;
		}
	}
//	Sys_UpdateWindows (W_ALL);
}

/*
============
Select_Clone

Creates an exact duplicate of the selection in place, then moves
the selected brushes off of their old positions
============
*/
void Select_Clone (void)
{
	brush_t		*b, *b2, *n, *next, *next2;
	vec3_t		delta;
	entity_t	*e;
	qboolean	moveorigin;

	moveorigin = true;

//	UNDO_StartBrushAdd ("&Undo Clone");
	g_qeglobals.d_workcount++;
	g_qeglobals.d_select_mode = sel_brush;

	delta[0] = g_qeglobals.d_gridsize;
	delta[1] = g_qeglobals.d_gridsize;
	delta[2] = 0;

	for (b=selected_brushes.next ; b != &selected_brushes ; b=next)
	{
		next = b->next;
		// if the brush is a world brush, handle simply
		if (b->owner == world_entity)
		{
			n = Brush_Clone (b);
			Brush_AddToList (n, &active_brushes);
			Entity_LinkBrush (world_entity, n);
			Brush_Build( n );
			Brush_Move (b, delta, true);
			continue;
		}

		e = Entity_Clone (b->owner);
		// clear the target / targetname
		DeleteKey (e, "target");
		DeleteKey (e, "target2");
		DeleteKey (e, "targetname");

		// if the brush is a fixed size entity, create a new entity
		if (b->owner->eclass->fixedsize)
		{
			n = Brush_Clone (b);
			Brush_AddToList (n, &active_brushes);
			Entity_LinkBrush (e, n);
			Brush_Build( n );
			Brush_Move (b, delta, true);
			continue;
		}
        
		// brush is a complex entity, grab all the other ones now

		next = &selected_brushes;

		for ( b2 = b ; b2 != &selected_brushes ; b2=next2)
		{
			next2 = b2->next;
			if (b2->owner != b->owner)
			{
				if (next == &selected_brushes)
					next = b2;
				continue;
			}

			// move b2 to the start of selected_brushes,
			// so it won't be hit again
			Brush_RemoveFromList (b2);
			Brush_AddToList (b2, &selected_brushes);
			
			n = Brush_Clone (b2);
			Brush_AddToList (n, &active_brushes);
			Entity_LinkBrush (e, n);
			Brush_Build( n );
			Brush_Move (b2, delta, moveorigin);
			if (b->owner->eclass->PhysicsModel)
			{
				moveorigin = false;
			}
		}

	}
	UNDO_FinishBrushAdd("&Undo Clone");
	Sys_UpdateWindows (W_ALL);
	TEXMOD_UpdateCheck();
}



/*
============
Select_SetTexture
============
*/
void Select_SetTexture (texdef_t *texdef)
{
	brush_t	*b;

	if (selected_face)
	{
		selected_face->texdef = *texdef;
		Brush_Build(selected_face_brush);
	}
	else
	{
		for (b=selected_brushes.next ; b != &selected_brushes ; b=b->next)
			if (!b->owner->eclass->fixedsize)
				Brush_SetTexture (b, texdef);
	}
	Sys_UpdateWindows (W_ALL);
}


/*
================================================================

  TRANSFORMATIONS

================================================================
*/

void Select_GetBounds (vec3_t mins, vec3_t maxs)
{
	brush_t	*b;
	int		i;

	for (i=0 ; i<3 ; i++)
	{
		mins[i] = 99999;
		maxs[i] = -99999;
	}

	for (b=selected_brushes.next ; b != &selected_brushes ; b=b->next)
		for (i=0 ; i<3 ; i++)
		{
			if (b->mins[i] < mins[i])
				mins[i] = b->mins[i];
			if (b->maxs[i] > maxs[i])
				maxs[i] = b->maxs[i];
		}
}

void Select_GetMid (vec3_t mid)
{
	vec3_t	mins, maxs;
	int		i;

	Select_GetBounds (mins, maxs);
	for (i=0 ; i<3 ; i++)
		mid[i] = g_qeglobals.d_gridsize*floor ( ( (mins[i] + maxs[i])*0.5 )/g_qeglobals.d_gridsize );
		// the gridsize bit makes the origin fit the grid 
}

vec3_t	select_origin;
vec3_t	select_matrix[3];
qboolean	select_fliporder;

void Select_AplyMatrix (void)
{
	brush_t	*b;
	face_t	*f;
	int		i, j;
	vec3_t	temp;

	
	
	for (b=selected_brushes.next ; b != &selected_brushes ; b=b->next)
	{
		for (f=b->brush_faces ; f ; f=f->next)
		{
			for (i=0 ; i<3 ; i++)
			{
				VectorSubtract (f->planepts[i], select_origin, temp);
				for (j=0 ; j<3 ; j++)
				{
					f->planepts[i][j] = DotProduct(temp, select_matrix[j])
						+ select_origin[j];
			
				}
			}
			if (select_fliporder)
			{
				VectorCopy (f->planepts[0], temp);
				VectorCopy (f->planepts[2], f->planepts[0]);
				VectorCopy (temp, f->planepts[2]);
			}
		}
		Brush_Build( b );
	}
	Sys_UpdateWindows (W_ALL);
}


void Select_FlipAxis (int axis)
{
	int		i;

	Select_GetMid (select_origin);

	if ((selected_brushes.next->owner != NULL) && (selected_brushes.next->owner->eclass->PhysicsModel))
	{
		GetVectorForKey(selected_brushes.next->owner, "origin", select_origin);
	}
	for (i=0 ; i<3 ; i++)
	{
		VectorCopy (vec3_origin, select_matrix[i]);
		select_matrix[i][i] = 1;
	}
	select_matrix[axis][axis] = -1;

	select_fliporder = true;
	Select_AplyMatrix ();
}


void Select_RotateAxis (int axis, float deg)
{

	brush_t	*b;
//	brush_t *bcopy;
//	face_t	*f, *fcopy;	
//	vec3_t  texvect[2], texvectcopy[2];
//	vec3_t  rotnorm, rotvert;
	vec3_t  zaxis, origin;
//	float	oldscale[2], oldshift[2];
//	float	delu, delv, delw;
//	float	maxx, maxy, minx, miny, maxz, minz;
//	float	delucopy, delvcopy, delwcopy;
//	float	maxxcopy, maxycopy, maxzcopy, minxcopy, minycopy, minzcopy;
//	float	dot, dotmax;
//	int		index;
	int stayaway =0;
	char text[255];
	int	oldangle;

	vec3_t	temp;
	int		i, j;
	vec_t	c, s;


	if (deg == 0)
		return;

	Select_GetMid (select_origin);

	if ((selected_brushes.next->owner != NULL) &&
		(selected_brushes.next == selected_brushes.prev) &&	
		strcmp(selected_brushes.next->owner->eclass->name,"worldspawn") && 
		(selected_brushes.next->owner->eclass->fixedsize)) // one selected entity
	{
		GetVectorForKey (selected_brushes.next->owner, "origin", select_origin);
		stayaway =1;
	}

	if ((selected_brushes.next->owner != NULL) && (selected_brushes.next->owner->eclass->PhysicsModel) &&
		(axis == 2))
	{
		oldangle = IntForKey(selected_brushes.next->owner, "angle");
		oldangle -= deg;
		while (oldangle < 0)
		{
			oldangle +=360;
		}
		while (oldangle >= 360)
		{
			oldangle -= 360;
		}
		sprintf(text, "%i", oldangle);
		SetKeyValue(selected_brushes.next->owner, "angle", text);
		GetVectorForKey(selected_brushes.next->owner, "origin", select_origin);
	
	}
	
	
	
	
	select_fliporder = false;

	zaxis[0] = 0;
	zaxis[1] = 0;
	zaxis[2] = 1;

	if (deg == 90)
	{
		for (i=0 ; i<3 ; i++)
		{
			VectorCopy (vec3_origin, select_matrix[i]);
			select_matrix[i][i] = 1;
		}
		i = (axis+1)%3;
		j = (axis+2)%3;
		VectorCopy (select_matrix[i], temp);
		VectorCopy (select_matrix[j], select_matrix[i]);
		VectorSubtract (vec3_origin, temp, select_matrix[j]);
	}
	else
	{
		deg = -deg;
		if (deg == -180)
		{
			c = -1;
			s = 0;
		}
		else if (deg == -270)
		{
			c = 0;
			s = -1;
		}
		else
		{
			c = cos(deg/180*3.14159);
			s = sin (deg/180*3.14159);
		}

		for (i=0 ; i<3 ; i++)
		{
			VectorCopy (vec3_origin, select_matrix[i]);
			select_matrix[i][i] = 1;
		}

		switch (axis)
		{
		case 0:
			select_matrix[1][1] = c;
			select_matrix[1][2] = -s;
			select_matrix[2][1] = s;
			select_matrix[2][2] = c;
			break;
		case 1:
			select_matrix[0][0] = c;
			select_matrix[0][2] = s;
			select_matrix[2][0] = -s;
			select_matrix[2][2] = c;
			break;
		case 2:
			select_matrix[0][0] = c;
			select_matrix[0][1] = -s;
			select_matrix[1][0] = s;
			select_matrix[1][1] = c;
			break;
		}
	}
//	TEXTURE LOCK STUFF STARTS HERE
//	Note:  This is Not done.  It impossible, given the way texture coordinates
//  are currently handled to write an all-inclusive texture lock.  But, one that handles
//  most mundane cases can be done.  This has been started here, but I have been instructed
//  to move on to other things -- dk
/*	for (b=selected_brushes.next ; b != &selected_brushes ; b=b->next)
	{
		if (b->texLocked == true)
		{
			bcopy = Brush_Clone (b);
			for (fcopy = bcopy->brush_faces; fcopy ; fcopy = fcopy->next)
			{
				for (i=0 ; i<3 ; i++)
				{
					VectorSubtract (fcopy->planepts[i], select_origin, temp);
					for (j=0 ; j<3 ; j++)
					{
						fcopy->planepts[i][j] = DotProduct(temp, select_matrix[j])+ select_origin[j];
					}
				}
			}
			
			Brush_Build (bcopy);		

//	at this point, we have two brushes: b is the unrotated brush, and bcopy is the rebuilt,
//	rotated brush.

			for ( f = b->brush_faces; f; f = f->next)
			{
//	for each face on b, get the normal, rotate it, and compare to faces on bcopy to find the
//	its corresponding face
				fcopy = bcopy->brush_faces;
				for (i=0; i<3; i++)
				{
					rotnorm[i] = DotProduct(f->plane.normal, select_matrix[i]);
				}
				VectorNormalize(rotnorm, rotnorm);
//	run through all rotates faces and find the largest Dot Product between norms
//  this avoids roundoff "epsilon" problems
				dotmax = -9999;
				i = 0;
				while (fcopy)
				{
					dot = DotProduct(fcopy->plane.normal, rotnorm);
					if (dot > dotmax)
					{
						dotmax = dot;
						index = i;
					}
					i++;
					fcopy = fcopy->next;
				}
				fcopy = bcopy->brush_faces;
				i = 0;
				while ( i < index )
				{
					i++;
					fcopy = fcopy->next;
				}

//	fcopy is now the rotated face corresponding to the unrotated face, f

				TextureAxisFromPlane(&f->plane, texvect[0], texvect[1]);
				TextureAxisFromPlane(&fcopy->plane, texvectcopy[0], texvectcopy[1]);
//  comparing texvect and texvectcopy will tell us if we need to worry about
//	changing texture axes
		
//	now get max and min x,y,z from both f and fcopy
				maxx = -9999;
				minx = 9999;
				maxy = -9999;
				miny = 9999;
				maxz = -9999;
				minz = 9999;
				maxxcopy = -9999;
				minxcopy = 9999;
				maxycopy = -9999;
				minycopy = 9999;
				maxzcopy = -9999;
				minzcopy = 9999;
				//  fixme:  HOW THE HELL CAN FACE_WINDING BE NULL?
				for ( i = 0 ; i < f->face_winding->numpoints; i++)
				{
						if (f->face_winding->points[i][0] < minx)
						{
							minx = f->face_winding->points[i][0];
						}
						if (f->face_winding->points[i][0] > maxx)
						{
							maxx = f->face_winding->points[i][0];
						}
						if (f->face_winding->points[i][1] < miny)
						{
							miny = f->face_winding->points[i][1];
						}
						if (f->face_winding->points[i][1] > maxy)
						{
							maxy = f->face_winding->points[i][1];
						}
						if (f->face_winding->points[i][2] < minz)
						{
							minz = f->face_winding->points[i][2];
						}
						if (f->face_winding->points[i][2] > maxz)
						{
							maxz = f->face_winding->points[i][2];
						}
						if (fcopy->face_winding->points[i][0] < minxcopy)
						{
							minxcopy = fcopy->face_winding->points[i][0];
						}
						if (fcopy->face_winding->points[i][0] > maxxcopy)
						{
							maxxcopy = fcopy->face_winding->points[i][0];
						}
						if (fcopy->face_winding->points[i][1] < minycopy)
						{
							minycopy = fcopy->face_winding->points[i][1];
						}
						if (fcopy->face_winding->points[i][1] > maxycopy)
						{
							maxycopy = fcopy->face_winding->points[i][1];
						}
						if (fcopy->face_winding->points[i][2] < minzcopy)
						{
							minzcopy = fcopy->face_winding->points[i][2];
						}
						if (fcopy->face_winding->points[i][2] > maxzcopy)
						{
							maxzcopy = fcopy->face_winding->points[i][2];
						}
				}
				VectorSubtract (f->face_winding->points[0], select_origin, temp);
				for ( i=0; i <3; i++)
				{
					rotvert[i] = DotProduct(temp,select_matrix[i]) + select_origin[i];
				}
				//rotvert is the rotated first vertex of this face
				// this rotation changes on top or bottom face
			switch (axis)
				{
				case 0:			//x rotation
					if ((texvectcopy[0][1] == 1) && (texvectcopy[1][2] == -1)) //yz texture plane
					{	// u->y v->z w->y
						f->texdef.rotate += deg;
					}
					else if ((texvectcopy[0][0] == 1) && (texvectcopy[1][2] == -1)) //xz texture plane
					{	// u->x v->z w->y
						delu = maxx - minx;
						delv = maxz - minz;
						delw = maxy - miny;
						delucopy = maxxcopy - minxcopy;
						delvcopy = maxzcopy - minzcopy;
						delwcopy = maxycopy - minycopy;
						if (!f->texdef.scale[0])
						{
							f->texdef.scale[0] = 1;
						}
						oldscale[1] = f->texdef.scale[1];
						f->texdef.scale[1] *= delvcopy / delv;
						f->texdef.shift[1] = (f->texdef.shift[1]*oldscale[1] - (minz - minzcopy)) / f->texdef.scale[1];

					}
					else if ((texvectcopy[0][0] == 1) && (texvectcopy[1][1] == -1)) //xyplane texture for ceiling/floor
					{	// u->x v->y w->z
						delu = maxx - minx;
						delv = maxy - miny;
						delw = maxz - minz;
						delucopy = maxxcopy - minxcopy;
						delvcopy = maxycopy - minycopy;
						delwcopy = maxzcopy - minzcopy;
						if (!f->texdef.scale[0])
						{
							f->texdef.scale[0] = 1;
						}
						oldscale[1] = f->texdef.scale[1];
						f->texdef.scale[1] *= delvcopy / delv;
						f->texdef.shift[1] = (f->texdef.shift[1]*oldscale[1] - (miny - minycopy)) / f->texdef.scale[1];
					}
					break;
				case 1:			//y rotation
					if ((texvectcopy[0][0] == 1) && (texvectcopy[1][2] == -1)) //xz texture plane
					{	// u->x v->z w->y
						f->texdef.rotate -= deg;
					}
					else if ((texvect[0][0] == 1) && (texvect[1][1] == -1)) //xyplane texture for ceiling/floor
					{	// u->x v->y w->z
						if ((texvectcopy[0][0] == 1) && (texvectcopy[1][1] == -1)) // staying in xy
						{
							delu = maxx - minx;
							delv = maxy - miny;
							delw = maxz - minz;
							delucopy = maxxcopy - minxcopy;
							delvcopy = maxycopy - minycopy;
							delwcopy = maxzcopy - minzcopy;
							if (!f->texdef.scale[0])
							{
								f->texdef.scale[0] = 1;
							}
							oldscale[0] = f->texdef.scale[0];
							f->texdef.scale[0] *= delucopy / delu;
							f->texdef.shift[0] = (f->texdef.shift[0]*oldscale[0] + (minx - minxcopy)) / f->texdef.scale[0];
						}
						else if ((texvectcopy[0][1] == 1) && (texvectcopy[1][2] == -1)) // switching to yz
						{
							delu = maxy - miny;
							delv = maxz - minz;
							delw = maxx - minx;
							delucopy = maxycopy - minycopy;
							delvcopy = maxzcopy - minzcopy;
							delwcopy = maxxcopy - minxcopy;
							if (!f->texdef.scale[1])
							{
								f->texdef.scale[1] = 1;
							}
							oldscale[0] = f->texdef.scale[0];
							f->texdef.scale[0] = f->texdef.scale[1];
							f->texdef.scale[1] = oldscale[0];
							f->texdef.scale[1] *= -(delvcopy / delw);
							oldshift[0] = f->texdef.shift[0];
							f->texdef.shift[0] = f->texdef.shift[1];
							f->texdef.shift[1] = oldshift[0];
							f->texdef.shift[1] = (f->texdef.shift[1]*oldscale[0]  + minx - minzcopy) / f->texdef.scale[1];	// FIX ME
							f->texdef.rotate += 90;
						}
					}
					else if ((texvectcopy[0][1] == 1) && (texvectcopy[1][2] == -1)) //yzplane texture for west/east walls
					{	// u->y v->z w->x
						delu = maxy - miny;
						delv = maxz - minz;
						delw = maxx - minx;
						delucopy = maxycopy - minycopy;
						delvcopy = maxzcopy - minzcopy;
						delwcopy = maxxcopy - minxcopy;
						if (!f->texdef.scale[1])
						{
							f->texdef.scale[1] = 1;
						}
						oldscale[1] = f->texdef.scale[1];
						f->texdef.scale[1] *= delvcopy / delv;
						f->texdef.shift[1] = (f->texdef.shift[1]*oldscale[1] - (minz - minzcopy)) / f->texdef.scale[1];
					}
					break;
				case 2:			//z rotation
					if ((texvect[0][0] == 1) && (texvect[1][1] == -1)) //xyplane texture for ceiling/floor
					{	// u->x v->y w->z
						f->texdef.rotate += deg; // rotate ceiling & floor textures
						if (DotProduct(fcopy->plane.normal, zaxis) > 0)
						{
							f->texdef.shift[0] += (f->face_winding->points[0][0] - rotvert[0]) / f->texdef.scale[0];
							f->texdef.shift[1] += (f->face_winding->points[0][1] - rotvert[1]) / f->texdef.scale[1];
						}
						else
						{
							f->texdef.shift[0] += (f->face_winding->points[0][0] - rotvert[0]) / f->texdef.scale[0];
							f->texdef.shift[1] -= (f->face_winding->points[0][1] - rotvert[1]) / f->texdef.scale[1];
						}

						// this texture plane will not change in a z rotation
					}
					else if ((texvect[0][1] == 1) && (texvect[1][2] == -1)) //yzplane texture for west/east walls
					{	// u->y v->z w->x
						if ((texvectcopy[0][1] == 1) && (texvectcopy[1][2] == -1)) // not changing axes
						{
							delu = maxy - miny;
							delv = maxz - minz;
							delw = maxx - minx;
							delucopy = maxycopy - minycopy;
							delvcopy = maxzcopy - minzcopy;
							delwcopy = maxxcopy - minxcopy;
							if (!f->texdef.scale[0])
							{
								f->texdef.scale[0] = 1;
							}
							oldscale[0] = f->texdef.scale[0];
							f->texdef.scale[0] *= delucopy / delu;
							f->texdef.shift[0] = (f->texdef.shift[0]*oldscale[0] + (miny - minycopy)) / f->texdef.scale[0];
						}
						else if ((texvectcopy[0][0] == 1) && (texvectcopy[1][2] == -1))// switching to xzplane
						{
							delu = maxx - minx;
							delv = maxz - minz;
							delw = maxy - miny;
							delucopy = maxxcopy - minxcopy;
							delvcopy = maxzcopy - minzcopy;
							delwcopy = maxycopy - minycopy;
							if (!f->texdef.scale[0])
							{
								f->texdef.scale[0] = 1;
							}
							oldscale[0] = f->texdef.scale[0];
							f->texdef.scale[0] *= delucopy / delw;
							f->texdef.shift[0] = (f->texdef.shift[0]*oldscale[0]  + miny - minxcopy) / f->texdef.scale[0];
						}
						else
						{
							Error;
						}
					}
					else if ((texvect[0][0] == 1) && (texvect[1][2] == -1)) //xzplane texture for north/south walls
					{	// u->x v->z w->y
						if ((texvectcopy[0][0] == 1) && (texvectcopy[1][2] == -1))
						{
							delu = maxx - minx;
							delv = maxz - minz;
							delw = maxy - miny;
							delucopy = maxxcopy - minxcopy;
							delvcopy = maxzcopy - minzcopy;
							delwcopy = maxycopy - minycopy;
							if (!f->texdef.scale[0])
							{
								f->texdef.scale[0] = 1;
							}
							oldscale[0] = f->texdef.scale[0];
							f->texdef.scale[0] *= delucopy / delu;
							f->texdef.shift[0] = (f->texdef.shift[0]*oldscale[0] + (minx - minxcopy)) / f->texdef.scale[0];
						}
						else if ((texvectcopy[0][1] == 1) && (texvectcopy[1][2] == -1)) // switch to yz
						{
							delu = maxy - miny;
							delv = maxz - minz;
							delw = maxx - minx;
							delucopy = maxycopy - minycopy;
							delvcopy = maxzcopy - minzcopy;
							delwcopy = maxxcopy - minxcopy;
							if (!f->texdef.scale[0])
							{
								f->texdef.scale[0] = 1;
							}
							oldscale[0] = f->texdef.scale[0];
							f->texdef.scale[0] *= delucopy / delw;
							f->texdef.shift[0] = (f->texdef.shift[0]*oldscale[0] + (minx - minycopy)) / f->texdef.scale[0];
						}
						else
						{
							Error; // uh oh
						}
					} // end if
					break;
				default:
					break;
				} // end switch
			} // end face loop
		} // end texture lock if
	}	// end brush loop
// TEXTURE LOCK STUFF ENDS HERE*/
	Select_AplyMatrix ();
	if ((selected_brushes.next->owner != NULL) && (stayaway == 0))// recompute origins for fixed-sized, non worldspawn
	{
		for (b=selected_brushes.next ; b != &selected_brushes ; b=b->next)
		{
			if (strcmp(b->owner->eclass->name,"worldspawn") && 
			(b->owner->eclass->fixedsize)) // fixedsize non-worldspawn
			{
				VectorSubtract (b->owner->brushes.onext->mins, b->owner->eclass->mins, origin); 
				sprintf (text, "%i %i %i", (int)origin[0],
				(int)origin[1], (int)origin[2]);
				SetKeyValue (b->owner, "origin", text);
			}
		}
	}
}

/*
================================================================

GROUP SELECTIONS

================================================================
*/

void Select_CompleteTall (void)
{
	brush_t	*b, *next;
	int		i;
	vec3_t	mins, maxs;

	if (!QE_SingleBrush ())
		return;

	UNDO_ClearSelection(selected_brushes.next);
	g_qeglobals.d_select_mode = sel_brush;

	VectorCopy (selected_brushes.next->mins, mins);
	VectorCopy (selected_brushes.next->maxs, maxs);
	Select_Delete (FALSE);

	for (b=active_brushes.next ; b != &active_brushes ; b=next)
	{
		next = b->next;
		for (i=0 ; i<2 ; i++)
			if (b->maxs[i] > maxs[i] || b->mins[i] < mins[i])
				break;
		if (i == 2)
		{
			Brush_RemoveFromList (b);
			Brush_AddToList (b, &selected_brushes);
		}
	}
	Sys_UpdateWindows (W_ALL);
	TEXMOD_UpdateCheck();
}

void Select_PartialTall (void)
{
	brush_t	*b, *next;
	int		i;
	vec3_t	mins, maxs;

	if (!QE_SingleBrush ())
		return;

	UNDO_ClearSelection(selected_brushes.next);
	g_qeglobals.d_select_mode = sel_brush;

	VectorCopy (selected_brushes.next->mins, mins);
	VectorCopy (selected_brushes.next->maxs, maxs);
	Select_Delete (FALSE);

	for (b=active_brushes.next ; b != &active_brushes ; b=next)
	{
		next = b->next;
		for (i=0 ; i<2 ; i++)
			if (b->mins[i] > maxs[i] || b->maxs[i] < mins[i])
				break;
		if (i == 2)
		{
			Brush_RemoveFromList (b);
			Brush_AddToList (b, &selected_brushes);
		}
	}

	Sys_UpdateWindows (W_ALL);
	TEXMOD_UpdateCheck();
}

void Select_Touching (void)
{
	brush_t	*b, *next;
	int		i;
	vec3_t	mins, maxs;

	if (!QE_SingleBrush ())
		return;

	g_qeglobals.d_select_mode = sel_brush;

	VectorCopy (selected_brushes.next->mins, mins);
	VectorCopy (selected_brushes.next->maxs, maxs);

	for (b=active_brushes.next ; b != &active_brushes ; b=next)
	{
		next = b->next;
		for (i=0 ; i<3 ; i++)
			if (b->mins[i] > maxs[i]+1 || b->maxs[i] < mins[i]-1)
				break;
		if (i == 3)
		{
			Brush_RemoveFromList (b);
			Brush_AddToList (b, &selected_brushes);
		}
	}

	Sys_UpdateWindows (W_ALL);
	TEXMOD_UpdateCheck();
}

void Select_Inside (void)
{
	brush_t	*b, *next;
	int		i;
	vec3_t	mins, maxs;

	if (!QE_SingleBrush ())
		return;

	UNDO_ClearSelection(selected_brushes.next);
	g_qeglobals.d_select_mode = sel_brush;

	VectorCopy (selected_brushes.next->mins, mins);
	VectorCopy (selected_brushes.next->maxs, maxs);
	Select_Delete (FALSE);

	for (b=active_brushes.next ; b != &active_brushes ; b=next)
	{
		next = b->next;
		for (i=0 ; i<3 ; i++)
			if (b->maxs[i] > maxs[i] || b->mins[i] < mins[i])
				break;
		if (i == 3)
		{
			Brush_RemoveFromList (b);
			Brush_AddToList (b, &selected_brushes);
		}
	}
	Sys_UpdateWindows (W_ALL);
	TEXMOD_UpdateCheck();
}

/*
=============
Select_Ungroup

Turn the currently selected entity back into normal brushes
=============
*/
void  Select_Ungroup (void)
{
	entity_t	*e;
	brush_t		*b;

	e = selected_brushes.next->owner;

	if (!e || e == world_entity || e->eclass->fixedsize)
	{
		Sys_Status ("Not a grouped entity.", 0);
		return;
	}

	for (b=e->brushes.onext ; b != &e->brushes ; b=e->brushes.onext)
	{
		Brush_RemoveFromList (b);
		Brush_AddToList (b, &active_brushes);
		Entity_UnlinkBrush (b);
		Entity_LinkBrush (world_entity, b);
		Brush_Build( b );
		b->owner = world_entity;
	}

	Entity_Free (e);
	Sys_UpdateWindows (W_ALL);
}

/*
====================
Select_MakeStructural
====================
*/
void Select_MakeStructural (void)
{
	brush_t	*b;
	face_t	*f;

	for (b=selected_brushes.next ; b != &selected_brushes ; b=b->next)
		for (f=b->brush_faces ; f ; f=f->next)
			f->texdef.contents &= ~CONTENTS_DETAIL;
	Select_Deselect ();
	Sys_UpdateWindows (W_ALL);
}

void Select_MakeDetail (void)
{
	brush_t	*b;
	face_t	*f;

	for (b=selected_brushes.next ; b != &selected_brushes ; b=b->next)
		for (f=b->brush_faces ; f ; f=f->next)
			f->texdef.contents |= CONTENTS_DETAIL;
	Select_Deselect ();
	Sys_UpdateWindows (W_ALL);
}
