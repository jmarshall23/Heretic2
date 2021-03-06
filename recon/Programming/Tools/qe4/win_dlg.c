
#include "qe3.h"
#include "undo.h"

BOOL CALLBACK GammaDlgProc (
    HWND hwndDlg,	// handle to dialog box
    UINT uMsg,	// message
    WPARAM wParam,	// first message parameter
    LPARAM lParam 	// second message parameter
   )
{
	char sz[256];

	switch (uMsg)
    {
	case WM_INITDIALOG:
		sprintf(sz, "%1.1f", g_qeglobals.d_savedinfo.fGamma);		
		SetWindowText(GetDlgItem(hwndDlg, IDC_G_EDIT), sz);
		return TRUE;
	case WM_COMMAND: 
		switch (LOWORD(wParam)) 
		{ 
		
		case IDOK:
			GetWindowText(GetDlgItem(hwndDlg, IDC_G_EDIT), sz, 255);
			g_qeglobals.d_savedinfo.fGamma = atof(sz);
			EndDialog(hwndDlg, 1);
			return TRUE;

		case IDCANCEL:
			EndDialog(hwndDlg, 0);
			return TRUE;
		}
	}
	return FALSE;
}



void DoGamma(void)
{
	char *psz, sz[256];
	if ( DialogBox(g_qeglobals.d_hInstance, (char *)IDD_GAMMA, g_qeglobals.d_hwndMain, GammaDlgProc))
	{
		psz = ValueForKey(world_entity, "_wad");
		if (psz)
		{
			strcpy(sz, psz);
			Texture_Flush();
			Texture_ShowInuse();
		}
	}
}		

//================================================


void SelectBrush (int entitynum, int brushnum)
{
	entity_t	*e;
	brush_t		*b;
	int			i;

	if (entitynum == 0)
		e = world_entity;
	else
	{
		e = entities.next;
		while (--entitynum)
		{
			e=e->next;
			if (e == &entities)
			{
				Sys_Status ("No such entity.", 0);
				return;
			}
		}
	}

	b = e->brushes.onext;
	if (b == &e->brushes)
	{
		Sys_Status ("No such brush.", 0);
		return;
	}
	while (brushnum--)
	{
		b=b->onext;
		if (b == &e->brushes)
		{
			Sys_Status ("No such brush.", 0);
			return;
		}
	}

	Brush_RemoveFromList (b);
	Brush_AddToList (b, &selected_brushes);


	Sys_UpdateWindows (W_ALL);
	for (i=0 ; i<3 ; i++)
		g_qeglobals.d_xy.origin[i] = (b->mins[i] + b->maxs[i])/2;

	Sys_Status ("Selected.", 0);
}

/*
=================
GetSelectionIndex
=================
*/
void GetSelectionIndex (int *ent, int *brush)
{
	brush_t		*b, *b2;
	entity_t	*entity;

	*ent = *brush = 0;

	b = selected_brushes.next;
	if (b == &selected_brushes)
		return;

	// find entity
	if (b->owner != world_entity)
	{
		(*ent)++;
		for (entity = entities.next ; entity != &entities 
			; entity=entity->next, (*ent)++)
		;
	}

	// find brush
	for (b2=b->owner->brushes.onext 
		; b2 != b && b2 != &b->owner->brushes
		; b2=b2->onext, (*brush)++)
	;
}

BOOL CALLBACK FindBrushDlgProc (
    HWND hwndDlg,	// handle to dialog box
    UINT uMsg,	// message
    WPARAM wParam,	// first message parameter
    LPARAM lParam 	// second message parameter
   )
{
	char entstr[256];
	char brushstr[256];
	HWND	h;
	int		ent, brush;

	switch (uMsg)
    {
	case WM_INITDIALOG:
		// set entity and brush number
		GetSelectionIndex (&ent, &brush);
		sprintf (entstr, "%i", ent);
		sprintf (brushstr, "%i", brush);
		SetWindowText(GetDlgItem(hwndDlg, IDC_FIND_ENTITY), entstr);
		SetWindowText(GetDlgItem(hwndDlg, IDC_FIND_BRUSH), brushstr);

		h = GetDlgItem(hwndDlg, IDC_FIND_ENTITY);
		SetFocus (h);
		return FALSE;

	case WM_COMMAND: 
		switch (LOWORD(wParam)) 
		{ 
			case IDOK:
				GetWindowText(GetDlgItem(hwndDlg, IDC_FIND_ENTITY), entstr, 255);
				GetWindowText(GetDlgItem(hwndDlg, IDC_FIND_BRUSH), brushstr, 255);
				SelectBrush (atoi(entstr), atoi(brushstr));
				EndDialog(hwndDlg, 1);
				return TRUE;

			case IDCANCEL:
				EndDialog(hwndDlg, 0);
				return TRUE;
		}	
	}
	return FALSE;
}



void DoFind(void)
{
	DialogBox(g_qeglobals.d_hInstance, (char *)IDD_FINDBRUSH, g_qeglobals.d_hwndMain, FindBrushDlgProc);
}	


/*
===================================================

  Set Max View Distance

===================================================
*/

BOOL CALLBACK SeMaxViewDistDlgProc (
    HWND hwndDlg,	// handle to dialog box
    UINT uMsg,	// message
    WPARAM wParam,	// first message parameter
    LPARAM lParam 	// second message parameter
   )
{
	char	toStr[256];
	char*	curDist;
	int		decimal, sign;
	float	vDist;
	HWND	h;

	switch (uMsg)
    {
	case WM_INITDIALOG:
		curDist = _fcvt(g_qeglobals.d_maxViewDist,0, &decimal, &sign);
		SetWindowText(GetDlgItem(hwndDlg, IDC_MAXVIEWDIST), curDist);
		h = GetDlgItem(hwndDlg, IDC_MAXVIEWDIST);
		SetFocus (h);//put the cursor on the x text entry box
		return FALSE;

	case WM_COMMAND: 
		GetWindowText(GetDlgItem(hwndDlg, IDC_MAXVIEWDIST), toStr, 255);
		vDist = atof(toStr);
		switch (LOWORD(wParam)) 
		{ 
			case IDOK:
				// set some global view range here.
				if(vDist > 16)
				{	// less than this would be meaningless, I believe
					g_qeglobals.d_maxViewDist = vDist;	
					g_qeglobals.d_savedinfo.maxViewDist = vDist;
				}
				Sys_UpdateWindows (W_CAMERA);
				EndDialog(hwndDlg, 0);
				return TRUE;

			case IDCANCEL:
				EndDialog(hwndDlg, 0);
				return TRUE;
		}	
	}
	return FALSE;
}

void DoSetMaxViewDist(void)
{
	DialogBox(g_qeglobals.d_hInstance, (char *)IDD_MAXVIEWDIST, g_qeglobals.d_hwndMain, SeMaxViewDistDlgProc);
}


void DoDefaultTextures(void)
{
	brush_t	*b;
	face_t	*f;

	for (b=active_brushes.next ; b != &active_brushes ; b=b->next)
	{
		for(f=b->brush_faces; f ; f=f->next)
		{
			f->texdef.scale[0] = f->d_texture->scale_x;
			f->texdef.scale[1] = f->d_texture->scale_y;
		}
	}

	if(camera.draw_mode == cd_texture)
	{
		camera.draw_mode = cd_solid;
		Map_BuildBrushData(false);
		Sys_UpdateWindows (W_ALL);

		if (camera.draw_mode != cd_texture)
		{
			camera.draw_mode = cd_texture;
			Map_BuildBrushData(false);
		}

		Sys_UpdateWindows (W_ALL);
	}
}

/*
===================================================

  REPLACE TEXTURE

===================================================
*/

BOOL CALLBACK ReplaceTextureDlgProc (
    HWND hwndDlg,	// handle to dialog box
    UINT uMsg,	// message
    WPARAM wParam,	// first message parameter
    LPARAM lParam 	// second message parameter
   )
{
	char fromStr[256];
	char toStr[256];

	switch (uMsg)
    {
	case WM_INITDIALOG:
		// init the two boxes to the last two selected textures
		SetWindowText(GetDlgItem(hwndDlg, IDC_REP_EDIT2), g_qeglobals.d_texturewin.texdef.name);
		SetWindowText(GetDlgItem(hwndDlg, IDC_REP_EDIT1), g_qeglobals.d_texturewin.prevTexDef.name);
		return FALSE;

	case WM_COMMAND: 
		GetWindowText(GetDlgItem(hwndDlg, IDC_REP_EDIT2), toStr, 255);
		GetWindowText(GetDlgItem(hwndDlg, IDC_REP_EDIT1), fromStr, 255);
		switch (LOWORD(wParam)) 
		{ 
			case IDD_REPLACEALL:
				Texture_Replace(fromStr, toStr, 1);
				EndDialog(hwndDlg, 0);
				return TRUE;

			case IDD_REPLACESELECTED:
				Texture_Replace(fromStr, toStr, 0);
				EndDialog(hwndDlg, 0);
				return TRUE;

			case IDCANCEL:
				EndDialog(hwndDlg, 0);
				return TRUE;
		}	
	}
	return FALSE;
}



void DoTextureReplace(void)
{
	DialogBox(g_qeglobals.d_hInstance, (char *)IDD_REPLACE, g_qeglobals.d_hwndMain, ReplaceTextureDlgProc);
}	


	
/*
===================================================

  ARBITRARY ROTATE

===================================================
*/


BOOL CALLBACK RotateDlgProc (
    HWND hwndDlg,	// handle to dialog box
    UINT uMsg,	// message
    WPARAM wParam,	// first message parameter
    LPARAM lParam 	// second message parameter
   )
{
	char	str[256];
	HWND	h;
	float	v;

	switch (uMsg)
    {
	case WM_INITDIALOG:
		h = GetDlgItem(hwndDlg, IDC_ROTX);
		SetFocus (h);//put the cursor on the x text entry box
		return FALSE;

	case WM_COMMAND: 
		switch (LOWORD(wParam)) 
		{ 
		
		case IDOK:
			UNDO_StartBrushEdit ("&Undo Rotation");
			GetWindowText(GetDlgItem(hwndDlg, IDC_ROTX), str, 255);
			v = atof(str);
			if (v)
				Select_RotateAxis (0, v);

			GetWindowText(GetDlgItem(hwndDlg, IDC_ROTY), str, 255);
			v = atof(str);
			if (v)
				Select_RotateAxis (1, v);

			GetWindowText(GetDlgItem(hwndDlg, IDC_ROTZ), str, 255);
			v = atof(str);
			if (v)
				Select_RotateAxis (2, v);
			UNDO_FinishBrushEdit (NULL);
			EndDialog(hwndDlg, 1);
			return TRUE;

		case IDCANCEL:
			EndDialog(hwndDlg, 0);
			return TRUE;
		}	
	}

	return FALSE;
}



void DoRotate(void)
{
	DialogBox(g_qeglobals.d_hInstance, (char *)IDD_ROTATE, g_qeglobals.d_hwndMain, RotateDlgProc);
}

/*
===================================================

  ARBITRARY ROTATE

===================================================
*/


BOOL CALLBACK ScaleDlgProc (
    HWND hwndDlg,	// handle to dialog box
    UINT uMsg,	// message
    WPARAM wParam,	// first message parameter
    LPARAM lParam 	// second message parameter
   )
{
	char	str[256];
	HWND	h;
	float	v;
	int		b1, nox, noy, noz;

	switch (uMsg)
    {
	case WM_INITDIALOG:
		h = GetDlgItem(hwndDlg, IDC_SCALE);
		SetWindowText(h, "1.0");
		SetFocus (h);//set focus to scale text entry box
		SendMessage(h, WM_LBUTTONDBLCLK, 0, 0);//don't know an easier way to highlight this text --ss
		return FALSE;

	case WM_COMMAND: 
		switch (LOWORD(wParam)) 
		{ 
		
		case IDOK:
			GetWindowText(GetDlgItem(hwndDlg, IDC_SCALE), str, 255);
			v = atof(str);
			if (v)
			{	// Scale all of the selected brushes here by this value
				UNDO_StartBrushEdit ("&Undo Scale");
				b1 = SendMessage(GetDlgItem(hwndDlg, IDC_SCALE_EACH_ORIGIN), BM_GETCHECK, 0, 0);
				b1 = (b1) ? 0:1; // these need to be swapped for the function
				nox = SendMessage(GetDlgItem(hwndDlg, IDC_CHECK_NOX), BM_GETCHECK, 0, 0);
				noy = SendMessage(GetDlgItem(hwndDlg, IDC_CHECK_NOY), BM_GETCHECK, 0, 0);
				noz = SendMessage(GetDlgItem(hwndDlg, IDC_CHECK_NOZ), BM_GETCHECK, 0, 0);

				Brush_Scale(v, b1, nox, noy, noz);
				UNDO_FinishBrushEdit (NULL);
			}
			EndDialog(hwndDlg, 1);
			return TRUE;

		case IDCANCEL:
			EndDialog(hwndDlg, 0);

			return TRUE;
		}	
	}

	return FALSE;
}



void DoScale(void)
{
	DialogBox(g_qeglobals.d_hInstance, (char *)IDD_SCALE, g_qeglobals.d_hwndMain, ScaleDlgProc);
}
		
/*
===================================================

  ARBITRARY SIDES

===================================================
*/


BOOL CALLBACK SidesDlgProc (
    HWND hwndDlg,	// handle to dialog box
    UINT uMsg,	// message
    WPARAM wParam,	// first message parameter
    LPARAM lParam 	// second message parameter
   )
{
	char str[256];
	HWND	h;

	switch (uMsg)
    {
	case WM_INITDIALOG:
		h = GetDlgItem(hwndDlg, IDC_FIND_ENTITY);
		SetFocus (h);
		return FALSE;

	case WM_COMMAND: 
		switch (LOWORD(wParam)) { 
		
		case IDOK:
			GetWindowText(GetDlgItem(hwndDlg, IDC_SIDES), str, 255);
			
			UNDO_StartBrushEdit("&Undo Sides");
			Brush_MakeSided (atoi(str));
			UNDO_FinishBrushEdit (NULL);

			EndDialog(hwndDlg, 1);
		break;

		case ID_BRUSH_SNAPOK:

			GetWindowText(GetDlgItem(hwndDlg, IDC_SIDES), str, 255);
			
			UNDO_StartBrushEdit("&Undo Sides");
			Brush_MakeSided_Snap (atoi(str));
			UNDO_FinishBrushEdit (NULL);

			EndDialog(hwndDlg, 1);
		break;

		case IDCANCEL:
			EndDialog(hwndDlg, 0);
		break;
	}	
	default:
		return FALSE;
	}
}



void DoSides(void)
{
	DialogBox(g_qeglobals.d_hInstance, (char *)IDD_SIDES, g_qeglobals.d_hwndMain, SidesDlgProc);
}		

//======================================================================

/*
===================
DoAbout
===================
*/
BOOL CALLBACK AboutDlgProc( HWND hwndDlg,
						    UINT uMsg,
						    WPARAM wParam,
						    LPARAM lParam )
{
	switch (uMsg)
    {
	case WM_INITDIALOG:
		{
			char renderer[1024];
			char version[1024];
			char vendor[1024];
			char extensions[4096];

			sprintf( renderer, "Renderer:\t%s", glGetString( GL_RENDERER ) );
			sprintf( version, "Version:\t\t%s", glGetString( GL_VERSION ) );
			sprintf( vendor, "Vendor:\t\t%s", glGetString( GL_VENDOR ) );
			sprintf( extensions, "\n%s", glGetString( GL_EXTENSIONS ) );

			SetWindowText( GetDlgItem( hwndDlg, IDC_ABOUT_GLRENDERER ),   renderer );
			SetWindowText( GetDlgItem( hwndDlg, IDC_ABOUT_GLVERSION ),    version );
			SetWindowText( GetDlgItem( hwndDlg, IDC_ABOUT_GLVENDOR ),     vendor );
			SetWindowText( GetDlgItem( hwndDlg, IDC_ABOUT_GLEXTENSIONS ), extensions );
		}
		return TRUE;

	case WM_CLOSE:
		EndDialog( hwndDlg, 1 );
		return TRUE;

	case WM_COMMAND:
		if ( LOWORD( wParam ) == IDOK )
			EndDialog(hwndDlg, 1);
		return TRUE;
	}
	return FALSE;
}

void DoAbout(void)
{
	DialogBox( g_qeglobals.d_hInstance, ( char * ) IDD_ABOUT, g_qeglobals.d_hwndMain, AboutDlgProc );
}


/*
===================================================

  SURFACE INSPECTOR

===================================================
*/

texdef_t	g_old_texdef;
qboolean	g_changed_surface;

int	g_checkboxes[64] = { 
	IDC_CHECK1, IDC_CHECK2, IDC_CHECK3, IDC_CHECK4, 
	IDC_CHECK5, IDC_CHECK6, IDC_CHECK7, IDC_CHECK8, 
	IDC_CHECK9, IDC_CHECK10, IDC_CHECK11, IDC_CHECK12, 
	IDC_CHECK13, IDC_CHECK14, IDC_CHECK15, IDC_CHECK16,
	IDC_CHECK17, IDC_CHECK18, IDC_CHECK19, IDC_CHECK20,
	IDC_CHECK21, IDC_CHECK22, IDC_CHECK23, IDC_CHECK24,
	IDC_CHECK25, IDC_CHECK26, IDC_CHECK27, IDC_CHECK28,
	IDC_CHECK29, IDC_CHECK30, IDC_CHECK31, IDC_CHECK32,

	IDC_CHECK33, IDC_CHECK34, IDC_CHECK35, IDC_CHECK36,
	IDC_CHECK37, IDC_CHECK38, IDC_CHECK39, IDC_CHECK40,
	IDC_CHECK41, IDC_CHECK42, IDC_CHECK43, IDC_CHECK44,
	IDC_CHECK45, IDC_CHECK46, IDC_CHECK47, IDC_CHECK48,
	IDC_CHECK49, IDC_CHECK50, IDC_CHECK51, IDC_CHECK52,
	IDC_CHECK53, IDC_CHECK54, IDC_CHECK55, IDC_CHECK56,
	IDC_CHECK57, IDC_CHECK58, IDC_CHECK59, IDC_CHECK60,
	IDC_CHECK61, IDC_CHECK62, IDC_CHECK63, IDC_CHECK64
 };

/*
==============
SetSurfMods

Set the fields to the current texdef
===============
*/
void SetSurfMods(HWND hwnd)
{
	char	sz[128];
	texdef_t *pt;
	int		i;
	HWND comboBox;
	materialtype_t		*mat;

	pt = &g_qeglobals.d_texturewin.texdef;

	i = (pt->flags & 0x0FF000000) >> 24;
	comboBox = GetDlgItem(hwnd, IDC_TEXTURE_MATERIAL);
	for (mat=materialtypes ; mat->name ; mat++)
	{
		SendMessage(comboBox, CB_ADDSTRING, 0, (LPARAM)mat->name);
		if (i == mat->value)
		{
			SendMessage(comboBox, CB_SELECTSTRING, (WPARAM)(-1), (LPARAM)mat->name);
		}
	}

	SendMessage (hwnd, WM_SETREDRAW, 0, 0);

	SetWindowText(GetDlgItem(hwnd, IDC_TEXTURE), pt->name);

	sprintf(sz, "%d", (int)pt->shift[0]);
	SetWindowText(GetDlgItem(hwnd, IDC_HSHIFT), sz);

	sprintf(sz, "%d", (int)pt->shift[1]);
	SetWindowText(GetDlgItem(hwnd, IDC_VSHIFT), sz);

	sprintf(sz, "%4.2f", pt->scale[0]);
	SetWindowText(GetDlgItem(hwnd, IDC_HSCALE), sz);

	sprintf(sz, "%4.2f", pt->scale[1]);
	SetWindowText(GetDlgItem(hwnd, IDC_VSCALE), sz);

	sprintf(sz, "%d", (int)pt->rotate);
	SetWindowText(GetDlgItem(hwnd, IDC_ROTATE), sz);

	sprintf(sz, "%d", (int)pt->value);
	SetWindowText(GetDlgItem(hwnd, IDC_VALUE), sz);

	sprintf(sz, "%f", pt->lighting[0]);
	SetWindowText(GetDlgItem(hwnd, IDC_LIGHT_RED), sz);

	sprintf(sz, "%f", pt->lighting[1]);
	SetWindowText(GetDlgItem(hwnd, IDC_LIGHT_GREEN), sz);

	sprintf(sz, "%f", pt->lighting[2]);
	SetWindowText(GetDlgItem(hwnd, IDC_LIGHT_BLUE), sz);

	sprintf(sz, "%f", pt->lighting[3]);
	SetWindowText(GetDlgItem(hwnd, IDC_LIGHT_ALPHA), sz);

//	for (i=0 ; i<32 ; i++)  top 8 bits are now material
	for (i=0 ; i<24 ; i++)
		SendMessage(GetDlgItem(hwnd, g_checkboxes[i]), BM_SETCHECK, !!(pt->flags&(1<<i)), 0 );
	for (i=0 ; i<32 ; i++)
		SendMessage(GetDlgItem(hwnd, g_checkboxes[32+i]), BM_SETCHECK, !!(pt->contents&(1<<i)), 0 );

	SendMessage (hwnd, WM_SETREDRAW, 1, 0);
	InvalidateRect (hwnd, NULL, true);
}


/*
==============
GetSurfMods

Reads the fields to get the current texdef
===============
*/
void GetSurfMods(HWND hwnd)
{
	char	sz[128];
	texdef_t *pt;
	int		b;
	int		i;
	HWND comboBox;
	materialtype_t		*mat;

	pt = &g_qeglobals.d_texturewin.texdef;

	GetWindowText (GetDlgItem(hwnd, IDC_TEXTURE), sz, 127);
	strncpy (pt->name, sz, sizeof(pt->name)-1);
	if (pt->name[0] <= ' ')
	{
		strcpy (pt->name, "none");
		SetWindowText(GetDlgItem(hwnd, IDC_TEXTURE), pt->name);
	}

	GetWindowText (GetDlgItem(hwnd, IDC_HSHIFT), sz, 127);
	pt->shift[0] = atof(sz);

	GetWindowText (GetDlgItem(hwnd, IDC_VSHIFT), sz, 127);
	pt->shift[1] = atof(sz);

	GetWindowText(GetDlgItem(hwnd, IDC_HSCALE), sz, 127);
	pt->scale[0] = atof(sz);

	GetWindowText(GetDlgItem(hwnd, IDC_VSCALE), sz, 127);
	pt->scale[1] = atof(sz);

	GetWindowText(GetDlgItem(hwnd, IDC_ROTATE), sz, 127);
	pt->rotate = atof(sz);

	GetWindowText(GetDlgItem(hwnd, IDC_VALUE), sz, 127);
	pt->value = atof(sz);

	GetWindowText(GetDlgItem(hwnd, IDC_LIGHT_RED), sz, 127);
	pt->lighting[0] = atof(sz);

	GetWindowText(GetDlgItem(hwnd, IDC_LIGHT_GREEN), sz, 127);
	pt->lighting[1] = atof(sz);

	GetWindowText(GetDlgItem(hwnd, IDC_LIGHT_BLUE), sz, 127);
	pt->lighting[2] = atof(sz);

	GetWindowText(GetDlgItem(hwnd, IDC_LIGHT_ALPHA), sz, 127);
	pt->lighting[3] = atof(sz);

	pt->flags = 0;
//	for (i=0 ; i<32 ; i++) top 8 bits are now material
	for (i=0 ; i<24 ; i++)
	{
		b = SendMessage(GetDlgItem(hwnd, g_checkboxes[i]), BM_GETCHECK, 0, 0);
		if (b != 1 && b != 0)
			continue;
		pt->flags |= b<<i;
	}

	pt->contents = 0;
	for (i=0 ; i<32 ; i++)
	{
		b = SendMessage(GetDlgItem(hwnd, g_checkboxes[32+i]), BM_GETCHECK, 0, 0);
		if (b != 1 && b != 0)
			continue;
		pt->contents |= b<<i;
	}

	comboBox = GetDlgItem(hwnd, IDC_TEXTURE_MATERIAL);
	GetWindowText(comboBox, sz, 127);
	for (mat=materialtypes ; mat->name ; mat++)
	{
		if (!strcmp(mat->name, sz))
		{
			// assumes SURF_MATERIAL is in top 8 bits
			pt->flags = (pt->flags & 0x0FFFFFF) | (mat->value << 24);
			break;
		}
	}

	g_changed_surface = true;
	Select_SetTexture(pt);
}

/*
=================
UpdateSpinners
=================
*/
void UpdateSpinners(HWND wnd, unsigned uMsg, WPARAM wParam, LPARAM lParam)
{
	int nScrollCode;
	HWND hwnd;
	texdef_t *pt;

	pt = &g_qeglobals.d_texturewin.texdef;

	nScrollCode = (int) LOWORD(wParam);  // scroll bar value 
	hwnd = (HWND) lParam;       // handle of scroll bar 

	if ((nScrollCode != SB_LINEUP) && (nScrollCode != SB_LINEDOWN))
		return;
	
	if (hwnd == GetDlgItem(wnd, IDC_ROTATEA))
	{
		if (nScrollCode == SB_LINEUP)
			pt->rotate += 45;
		else
			pt->rotate -= 45;

		if (pt->rotate < 0)
			pt->rotate += 360;

		if (pt->rotate >= 360)
			pt->rotate -= 360;
	}

	else if (hwnd == GetDlgItem(wnd, IDC_HSCALEA))
	{
		if (nScrollCode == SB_LINEDOWN)
			pt->scale[0] -= 0.1;
		else
			pt->scale[0] += 0.1;
	}
	
	else if (hwnd == GetDlgItem(wnd, IDC_VSCALEA))
	{
		if (nScrollCode == SB_LINEUP)
			pt->scale[1] += 0.1;
		else
			pt->scale[1] -= 0.1;
	} 
	
	else if (hwnd == GetDlgItem(wnd, IDC_HSHIFTA))
	{
		if (nScrollCode == SB_LINEDOWN)
			pt->shift[0] -= 8;
		else
			pt->shift[0] += 8;
	}
	
	else if (hwnd == GetDlgItem(wnd, IDC_VSHIFTA))
	{
		if (nScrollCode == SB_LINEUP)
			pt->shift[1] += 8;
		else
			pt->shift[1] -= 8;
	}

	SetSurfMods(wnd);
	g_changed_surface = true;
	Select_SetTexture(pt);
}



BOOL CALLBACK SurfaceDlgProc (
    HWND hwndDlg,	// handle to dialog box
    UINT uMsg,	// message
    WPARAM wParam,	// first message parameter
    LPARAM lParam 	// second message parameter
   )
{
	switch (uMsg)
    {
	case WM_INITDIALOG:
		SetSurfMods (hwndDlg);
		return 0;

	case WM_COMMAND: 
		switch (LOWORD(wParam)) 
		{ 
		
		case IDOK:
			GetSurfMods (hwndDlg);
			EndDialog(hwndDlg, 1);
			return 1;
		break;

		case IDAPPLY:
			GetSurfMods (hwndDlg);
			InvalidateRect(g_qeglobals.d_hwndCamera, NULL, false);
			UpdateWindow (g_qeglobals.d_hwndCamera);
			return 1;
		break;

		case IDCANCEL:
//			g_qeglobals.d_texturewin.texdef = g_old_texdef;
//			if (g_changed_surface)
//				Select_SetTexture(&g_qeglobals.d_texturewin.texdef);
			EndDialog(hwndDlg, 0);
			return 1;
		break;
		}	
		break;

	case WM_HSCROLL:
	case WM_VSCROLL:
		UpdateSpinners(hwndDlg, uMsg, wParam, lParam);
		InvalidateRect(g_qeglobals.d_hwndCamera, NULL, false);
		UpdateWindow (g_qeglobals.d_hwndCamera);
		return 1;

	default:
		return 0;
	}
	return 0;
}

static undo_t surfaceUndo;

void SurfaceDlg_Init()
{
	surfaceUndo.brushdata.next = surfaceUndo.brushdata.prev = &surfaceUndo.brushdata;
}

void DoSurface (void)
{
	brush_t *from, *data;	

	//need to save all data if brush selection active
	// how can you tell if only one face selected??????
	if(!selected_face)
	{
		while (surfaceUndo.brushdata.next != &surfaceUndo.brushdata)
		{
			from = surfaceUndo.brushdata.next;
			Brush_Free (from);
		}

		from = &selected_brushes;
		while (from->next != &selected_brushes)
		{
			from = from->next;
			data = Brush_Clone(from);
			Brush_AddToList(data, &surfaceUndo.brushdata);	//store data
		}	
	}
	else
	{
		// save current state for cancel
		g_old_texdef = g_qeglobals.d_texturewin.texdef;
	}
	g_changed_surface = false;

	if(!DialogBox(g_qeglobals.d_hInstance, (char *)IDD_SURFACE, g_qeglobals.d_hwndMain, SurfaceDlgProc))
	{	//cancelled, undo changes
		if(g_changed_surface)
		{
			if(!selected_face)
			{
					while (selected_brushes.next != &selected_brushes)
					{
						from = selected_brushes.next;
						Brush_Free (from);
					}
					UNDO_Reset ();		//clone can destroy undo info
					UNDO_BrushClone (&surfaceUndo);				
			}
			else
			{
				g_qeglobals.d_texturewin.texdef = g_old_texdef;
				Select_SetTexture(&g_qeglobals.d_texturewin.texdef);
			}
		}
	}
}		

