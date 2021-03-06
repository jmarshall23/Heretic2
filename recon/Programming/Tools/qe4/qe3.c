
#include "qe3.h"
#include "undo.h"

QEGlobals_t  g_qeglobals;

void QE_CheckOpenGLForErrors(void)
{
    int	    i;
    
    while ( ( i = glGetError() ) != GL_NO_ERROR )
    {
		char buffer[100];

		sprintf( buffer, "OpenGL Error: %s", gluErrorString( i ) );

		MessageBox( g_qeglobals.d_hwndMain, buffer , "QuakeEd Error", MB_OK | MB_ICONEXCLAMATION );
		exit( 1 );
    }
}


char *ExpandReletivePath (char *p)
{
	static char	temp[1024];
	char	*base;

	if (!p || !p[0])
		return NULL;
	if (p[0] == '/' || p[0] == '\\')
		return p;

	base = ValueForKey(g_qeglobals.d_project_entity, "basepath");
	sprintf (temp, "%s/%s", base, p);
	return temp;
}


void QE_ResetIdle(void)
{
	int				h,m,s;
	double			diff;
	clock_t			now;
	static clock_t	i_start = 0;

	now = clock();

	if ( now - i_start > ( CLOCKS_PER_SEC * 60 * QE_IDLE_LOG_INTERVAL ) )
	{
		diff = (now - i_start) / CLOCKS_PER_SEC;
		h = diff / (60*60);
		diff -= h * (60*60);
		m = diff / 60;
		diff -= m * 60;
		s = diff;
		WriteUserLog("Idle Time: %d:%02d:%02d",h,m,s);
	}

	i_start = now;
}

/*
===============
QE_CheckAutoSave

If five minutes have passed since making a change
and the map hasn't been saved, save it out.
===============
*/
void QE_CheckAutoSave( void )
{
	static clock_t	s_start;
	clock_t			now;

	now = clock();

	if ( modified != 1 || !s_start)
	{
		s_start = now;
		return;
	}

	if ( now - s_start > ( CLOCKS_PER_SEC * 60 * QE_AUTOSAVE_INTERVAL ) )
	{
		Sys_Printf ("Autosaving...\n");
		Sys_Status ("Autosaving...", 0 );

		Map_SaveFile (ValueForKey(g_qeglobals.d_project_entity, "autosave"), false);

		Sys_Status ("Autosaving...Saved.", 0 );
		modified = 2;
		s_start = now;
	}
}



/*
===========
QE_LoadProject
===========
*/
qboolean QE_LoadProject (char *projectfile)
{
	char	*data;
	char	path2[MAX_PATH];

	Sys_Printf ("QE_LoadProject (%s)\n", projectfile);

	strcpy(path2, projectfile);
	StripExtension(path2);
	DefaultExtension(path2, ".mat");
	QFile_ReadMaterialTypes(ExpandArg(path2));
	if ( LoadFileNoCrash (projectfile, (void *)&data) == -1)
		return false;
	StartTokenParsing (data);
	g_qeglobals.d_project_entity = Entity_Parse (true); // parses project file and 
	// stores key value pairs in linked list g_qeglobals.d_project_entity.epairs, accessed
	// using ValueforKey(g_qeglobals.d_project_entity, "keyname"));
	if (!g_qeglobals.d_project_entity)
		Error ("Couldn't parse %s", projectfile);
	free (data);

	Eclass_InitForSourceDirectory (ValueForKey (g_qeglobals.d_project_entity, "entitypath"));

	FillClassList ();		// list in entity window

	Map_New ();

	FillTextureMenu ();
	FillBSPMenu ();

	return true;
}

/*
===========
QE_KeyDown
===========
*/
#define	SPEED_MOVE	32
#define	SPEED_TURN	22.5

qboolean QE_KeyDown (int key)
{
	switch (key)
	{
	case VK_UP:
		VectorMA (camera.origin, SPEED_MOVE, camera.forward, camera.origin);
		Sys_UpdateWindows (W_CAMERA|W_XY_OVERLAY);
		break;
	case VK_DOWN:
		VectorMA (camera.origin, -SPEED_MOVE, camera.forward, camera.origin);
		Sys_UpdateWindows (W_CAMERA|W_XY_OVERLAY);
		break;
	case VK_LEFT:
		camera.angles[1] += SPEED_TURN;
		Sys_UpdateWindows (W_CAMERA|W_XY_OVERLAY);
		break;
	case VK_RIGHT:
		camera.angles[1] -= SPEED_TURN;
		Sys_UpdateWindows (W_CAMERA|W_XY_OVERLAY);
		break;
	case VK_COMMA:
		VectorMA (camera.origin, -SPEED_MOVE, camera.right, camera.origin);
		Sys_UpdateWindows (W_CAMERA|W_XY_OVERLAY);
		break;
	case VK_PERIOD:
		VectorMA (camera.origin, SPEED_MOVE, camera.right, camera.origin);
		Sys_UpdateWindows (W_CAMERA|W_XY_OVERLAY);
		break;
	case VK_BACK:
		PostMessage (g_qeglobals.d_hwndMain, WM_COMMAND, ID_SELECTION_DELETE, 0);
		break;
	case VK_ESCAPE:
		PostMessage (g_qeglobals.d_hwndMain, WM_COMMAND, ID_SELECTION_DESELECT, 0);
		break;
	case VK_END:
		PostMessage (g_qeglobals.d_hwndMain, WM_COMMAND, ID_VIEW_CENTER, 0);
		break;
	case VK_DELETE:
		PostMessage (g_qeglobals.d_hwndMain, WM_COMMAND, ID_VIEW_ZOOMIN, 0);
		break;
	case VK_INSERT:
		PostMessage (g_qeglobals.d_hwndMain, WM_COMMAND, ID_VIEW_ZOOMOUT, 0);
		break;
	case VK_NEXT:
		PostMessage (g_qeglobals.d_hwndMain, WM_COMMAND, ID_VIEW_DOWNFLOOR, 0);
		break;
	case VK_PRIOR:
		PostMessage (g_qeglobals.d_hwndMain, WM_COMMAND, ID_VIEW_UPFLOOR, 0);
		break;
	case 'A':
		camera.angles[0] += SPEED_TURN;
		if (camera.angles[0] > 85)
			camera.angles[0] = 85;
		Sys_UpdateWindows (W_CAMERA|W_XY_OVERLAY);
		break;
	case 'C':
		camera.origin[2] -= SPEED_MOVE;
		Sys_UpdateWindows (W_CAMERA|W_XY_OVERLAY|W_Z_OVERLAY);
		break;
	case 'D':
		camera.origin[2] += SPEED_MOVE;
		Sys_UpdateWindows (W_CAMERA|W_XY_OVERLAY|W_Z_OVERLAY);
		break;
	case 'E':
		PostMessage (g_qeglobals.d_hwndMain, WM_COMMAND, ID_SELECTION_DRAGEDGES, 0);
		break;
	case 'G':
		PostMessage (g_qeglobals.d_hwndMain, WM_COMMAND, ID_BRUSH_ROTATEX, 0);
		break;
	case 'H':
		PostMessage (g_qeglobals.d_hwndMain, WM_COMMAND, ID_BRUSH_ROTATEY, 0);
		break;
	case 'I':
		PostMessage (g_qeglobals.d_hwndMain, WM_COMMAND, ID_BRUSH_FLIPZ, 0);
		break;
	case 'J':
		PostMessage (g_qeglobals.d_hwndMain, WM_COMMAND, ID_BRUSH_ROTATEZ, 0);
		break;
	case 'K':
		PostMessage( g_qeglobals.d_hwndMain, WM_COMMAND, ID_MISC_SELECTENTITYCOLOR, 0 );
		break;
	case 'N':
		PostMessage (g_qeglobals.d_hwndMain, WM_COMMAND, ID_VIEW_ENTITY, 0);
		break;
	case 'O':
		PostMessage (g_qeglobals.d_hwndMain, WM_COMMAND, ID_VIEW_CONSOLE, 0);
		break;
	case 'S':
		PostMessage (g_qeglobals.d_hwndMain, WM_COMMAND, ID_TEXTURES_INSPECTOR, 0);
		break;
	case 'T':
		PostMessage (g_qeglobals.d_hwndMain, WM_COMMAND, ID_VIEW_TEXTURE, 0);
		break;
	case 'U':
		PostMessage (g_qeglobals.d_hwndMain, WM_COMMAND, ID_BRUSH_FLIPY, 0);
		break;
	case 'V':
		PostMessage (g_qeglobals.d_hwndMain, WM_COMMAND, ID_SELECTION_DRAGVERTECIES, 0);
		break;
	case 'W':
		PostMessage (g_qeglobals.d_hwndMain, WM_COMMAND, ID_SELECTION_TOWER, 0);
		break;
	case 'Y':
		PostMessage (g_qeglobals.d_hwndMain, WM_COMMAND, ID_BRUSH_FLIPX, 0);
		break;
	case 'Z':
		camera.angles[0] -= SPEED_TURN;
		if (camera.angles[0] < -85)
			camera.angles[0] = -85;
		Sys_UpdateWindows (W_CAMERA|W_XY_OVERLAY);
		break;
	case '0':
		g_qeglobals.d_showgrid = !g_qeglobals.d_showgrid;
		PostMessage( g_qeglobals.d_hwndXY, WM_PAINT, 0, 0 );
		break;
	case '1':
		PostMessage (g_qeglobals.d_hwndMain, WM_COMMAND, ID_GRID_1, 0);
		break;
	case '2':
		PostMessage (g_qeglobals.d_hwndMain, WM_COMMAND, ID_GRID_2, 0);
		break;
	case '3':
		PostMessage (g_qeglobals.d_hwndMain, WM_COMMAND, ID_GRID_4, 0);
		break;
	case '4':
		PostMessage (g_qeglobals.d_hwndMain, WM_COMMAND, ID_GRID_8, 0);
		break;
	case '5':
		PostMessage (g_qeglobals.d_hwndMain, WM_COMMAND, ID_GRID_16, 0);
		break;
	case '6':
		PostMessage (g_qeglobals.d_hwndMain, WM_COMMAND, ID_GRID_32, 0);
		break;
	case '7':
		PostMessage (g_qeglobals.d_hwndMain, WM_COMMAND, ID_GRID_64, 0);
		break;
	case ' ':
		PostMessage (g_qeglobals.d_hwndMain, WM_COMMAND, ID_SELECTION_CLONE, 0);
		break;
	default:
		return false;
	}

	return true;
}

/*
===============
ConnectEntities

Sets target / targetname on the two entities selected
from the first selected to the secon
===============
*/
void ConnectEntities (void)
{
	entity_t	*e1, *e2, *e;
	char		*target, *tn, *target2, *target1;
	int			maxtarg, targetnum;
	char		newtarg[32];

	if (!QE_DoubleEntity())
	{
		Sys_Status ("Must have two different entities selected.", 0);
		Sys_Beep ();
		return;
	}

	e1 = g_qeglobals.d_select_order[0]->owner;
	e2 = g_qeglobals.d_select_order[1]->owner;

	if (e1 == world_entity || e2 == world_entity)
	{
		Sys_Status ("Can't connect to the world.", 0);
		Sys_Beep ();
		return;
	}

	target1 = ValueForKey (e1, "target");
	target = target1;

	target2 = ValueForKey (e1, "target2");	
	if ((target && target[0]) && (target2 && target2[0]))// e1 has a target and target2 name
	{
		strcpy (newtarg, target);
	}
	else
	{
		target = ValueForKey (e2, "targetname");
		if (target && target[0])
			strcpy (newtarg, target);
		else
		{
			// make a unique target value
			maxtarg = 0;
			for (e=entities.next ; e != &entities ; e=e->next)
			{
				tn = ValueForKey (e, "targetname");
				if (tn && tn[0])
				{
					targetnum = atoi(tn+1);
					if (targetnum > maxtarg)
						maxtarg = targetnum;
				}
			}
			sprintf (newtarg, "t%i", maxtarg+1);
		}
	}
	
	if (target1 && target1[0] && (!(strcmp(ValueForKey(e1, "classname"), "info_buoy"))))
	{
		SetKeyValue ( e1, "target2", newtarg);
	}
	else
	{
		SetKeyValue (e1, "target", newtarg);
	}
	SetKeyValue (e2, "targetname", newtarg);
	Sys_UpdateWindows (W_XY | W_CAMERA);

	Select_Deselect();
	Select_Brush (g_qeglobals.d_select_order[1]);
}

qboolean QE_SingleBrush (void)
{
	if ( (selected_brushes.next == &selected_brushes)
		|| (selected_brushes.next->next != &selected_brushes) )
	{
		Sys_Printf ("Error: you must have a single brush selected\n");
		return false;
	}
	if (selected_brushes.next->owner->eclass->fixedsize)
	{
		Sys_Printf ("Error: you cannot manipulate fixed size entities\n");
		return false;
	}

	return true;
}

// similar to above, except allows fixed size
qboolean QE_DoubleBrush (void)
{
	brush_t *brush;
	brush = selected_brushes.next;
	if(brush == &selected_brushes)
	{
		return false;
	}
	// brushes are added at head, so reverse order
	g_qeglobals.d_select_order[1] = brush;
	brush = brush->next;
	if(brush == &selected_brushes)
	{
		return false;
	}
	g_qeglobals.d_select_order[0] = brush;
	if(brush->next != &selected_brushes)
	{
		return false;
	}
	return true;
}

qboolean QE_DoubleEntity (void)
{
	entity_t *e1,*e2,*e3;
	brush_t *b1,*b2;

	b1 = selected_brushes.next;
	if((b1 == &selected_brushes) || (b1->next == &selected_brushes))
	{
		return false; // one or no brush selected
	}

	e1 = b1->owner;
	b2 = b1->next;
	e2 = b2->owner;
	while (e1 == e2)
	{
		b2 = b2->next;
		if(b2 == &selected_brushes)
		{
			return false; // only one entity
		}
		e2 = b2->owner;
	}
	// brushes are added at head, so reverse order
	g_qeglobals.d_select_order[1] = b1;
	g_qeglobals.d_select_order[0] = b2;
	// now we have brushes from two different entities selected
	// are there more entities selected??
	while (b2 != &selected_brushes)
	{
		b2 = b2->next;
		e3 = b2->owner;
		if ((e3) && (e3 != e1) && (e3 != e2))
		{
			return false; // more than two entities selected
		}
	}
	
	return true;
}

void SurfaceDlg_Init (void);
void QE_Init (void)
{
	/*
	** initialize variables
	*/
	g_qeglobals.d_gridsize = 8;
	g_qeglobals.d_showgrid = true;

	g_qeglobals.uMSH_MOUSEWHEEL = 0;
	g_qeglobals.d_hwndBsp = 0;// Q: 0 or NULL for handles? --ss
	if (g_qeglobals.d_savedinfo.maxViewDist < 16) // in other words, has not yet been defined
	{
		g_qeglobals.d_savedinfo.maxViewDist = 1024;
	}	
	g_qeglobals.d_maxViewDist = g_qeglobals.d_savedinfo.maxViewDist;//1024.0;

	/*
	** other stuff
	*/
	Texture_Init ();
	Cam_Init ();
	XY_Init ();
	Z_Init ();
	UNDO_Init ();
	SurfaceDlg_Init ();
}

void QE_ConvertDOSToUnixName( char *dst, const char *src )
{
	while ( *src )
	{
		if ( *src == '\\' )
			*dst = '/';
		else
			*dst = *src;
		dst++; src++;
	}
	*dst = 0;
}

int g_numbrushes, g_numentities;

void QE_CountBrushesAndUpdateStatusBar( void )
{
	static int      s_lastbrushcount, s_lastentitycount;
	static qboolean s_didonce;
	
	entity_t   *e;
	brush_t	   *b, *next;

	g_numbrushes = 0;
	g_numentities = 0;
	
	if ( active_brushes.next != NULL )
	{
		for ( b = active_brushes.next ; b != NULL && b != &active_brushes ; b=next)
		{
			next = b->next;
			if (b->brush_faces )
			{
				if ( !b->owner->eclass->fixedsize)
					g_numbrushes++;
				else
					g_numentities++;
			}
		}
	}

	if ( entities.next != NULL )
	{
		for ( e = entities.next ; e != &entities && g_numentities != MAX_MAP_ENTITIES ; e = e->next)
		{
			g_numentities++;
		}
	}

	if ( ( ( g_numbrushes != s_lastbrushcount ) || ( g_numentities != s_lastentitycount ) ) || ( !s_didonce ) )
	{
		Sys_UpdateStatusBar();

		s_lastbrushcount = g_numbrushes;
		s_lastentitycount = g_numentities;
		s_didonce = true;
	}
}

