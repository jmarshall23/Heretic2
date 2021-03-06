#include "qe3.h"

/*
===================================================

  TEXTURE MODIFY - a modeless dialog found on the entity window

===================================================
*/

static HWND		g_surfwin;
static LRESULT (CALLBACK* OldFieldWindowProc) (HWND, UINT, WPARAM, LPARAM);
static void GetTexMods(void);


#define numTMControls 5
static int texModControl[] = {
	IDC_HSHIFT,
	IDC_VSHIFT,
	IDC_ROTATE,
	IDC_HSCALE,
	IDC_VSCALE
};

static HWND hwndTM[numTMControls];


void TEXMOD_UpdateCheck()
{
	qboolean bHaveTrue = false, bHaveFalse = false, bState;
	brush_t *b;

	for (b=selected_brushes.next ; b != &selected_brushes ; b=b->next)
	{
		if(b->texLocked)
		{
			bHaveTrue = true;
		}
		else
		{
			bHaveFalse = true;
		}
		if(bHaveTrue && bHaveFalse) break;
	}
	if(bHaveTrue)
	{
		if(bHaveFalse)
		{
			bState = 2;
		}
		else
		{
			bState = 1;
		}
	}
	else
	{
		bState = 0;
	}
	CheckDlgButton(g_surfwin, IDC_TEXLOCK_CHECK, bState);
}

static void ResetCheck(int state)
{
	brush_t *b;

	CheckDlgButton(g_surfwin, IDC_TEXLOCK_CHECK, state);
	if(state == 2)	//indeterminate
		return; 
	for (b=selected_brushes.next ; b != &selected_brushes ; b=b->next)
	{
		b->texLocked = state;
	}
}

/*
=========================
FieldWndProc
=========================
*/
static BOOL CALLBACK FieldWndProc(
    HWND hwnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam)
{
	HWND hTab;

    switch (uMsg)
    {
/*	case WM_CHAR:
		if (LOWORD(wParam) == VK_TAB)
			return FALSE;
		if (LOWORD(wParam) == VK_RETURN)
			return FALSE;
		if (LOWORD(wParam) == VK_ESCAPE)
		{
			SetFocus (g_qeglobals.d_hwndCamera);
			return FALSE;
		}
		break;
*/

	case WM_GETDLGCODE:
		return DLGC_WANTALLKEYS;
	case WM_KEYDOWN:
		if (LOWORD(wParam) == VK_TAB)
		{
			if(GetKeyState(VK_SHIFT)&0x8000)
			{
				hTab = GetNextDlgTabItem(g_qeglobals.d_hwndTexModDlg, hwnd, TRUE);
			}
			else
			{
				hTab = GetNextDlgTabItem(g_qeglobals.d_hwndTexModDlg, hwnd, FALSE);
			}
			if(hTab)
			{
				SetFocus(hTab);
			}
		}
		if (LOWORD(wParam) == VK_RETURN)
		{
			GetTexMods ();
			InvalidateRect(g_qeglobals.d_hwndCamera, NULL, false);
			UpdateWindow (g_qeglobals.d_hwndCamera);
		}
		break;
//	case WM_NCHITTEST:
	case WM_LBUTTONDOWN:
		SetFocus (hwnd);
		break;
	}
	return CallWindowProc (OldFieldWindowProc, hwnd, uMsg, wParam, lParam);
}

/*
==============
SetTexMods

Set the fields to the current texdef
===============
*/
void SetTexMods(void)
{
	char	sz[128];
	texdef_t *pt;

	pt = &g_qeglobals.d_texturewin.texdef;
	if (selected_face)
	{
		pt->shift[0] = selected_face->texdef.shift[0];
		pt->shift[1] = selected_face->texdef.shift[1];
		pt->scale[0] = selected_face->texdef.scale[0];
		pt->scale[1] = selected_face->texdef.scale[1];
		pt->rotate = selected_face->texdef.rotate;
	}
	else if ((selected_brushes.next) && (selected_brushes.next != selected_brushes.next->next))
	{
		pt->shift[0] = selected_brushes.next->brush_faces->texdef.shift[0];
		pt->shift[1] = selected_brushes.next->brush_faces->texdef.shift[1];
		pt->scale[0] = selected_brushes.next->brush_faces->texdef.scale[0];
		pt->scale[1] = selected_brushes.next->brush_faces->texdef.scale[1];
		pt->rotate = selected_brushes.next->brush_faces->texdef.rotate;
	}

	SendMessage (g_surfwin, WM_SETREDRAW, 0, 0);

//	SetWindowText(GetDlgItem(g_surfwin, IDC_TEXTURE), pt->name);

	sprintf(sz, "%d", (int)pt->shift[0]);
	SetWindowText(GetDlgItem(g_surfwin, IDC_HSHIFT), sz);

	sprintf(sz, "%d", (int)pt->shift[1]);
	SetWindowText(GetDlgItem(g_surfwin, IDC_VSHIFT), sz);

	sprintf(sz, "%4.2f", pt->scale[0]);
	SetWindowText(GetDlgItem(g_surfwin, IDC_HSCALE), sz);

	sprintf(sz, "%4.2f", pt->scale[1]);
	SetWindowText(GetDlgItem(g_surfwin, IDC_VSCALE), sz);

	sprintf(sz, "%d", (int)pt->rotate);
	SetWindowText(GetDlgItem(g_surfwin, IDC_ROTATE), sz);
/*
	sprintf(sz, "%d", (int)pt->value);
	SetWindowText(GetDlgItem(g_surfwin, IDC_VALUE), sz);

	for (i=0 ; i<32 ; i++)
		SendMessage(GetDlgItem(g_surfwin, g_checkboxes[i]), BM_SETCHECK, !!(pt->flags&(1<<i)), 0 );
	for (i=0 ; i<32 ; i++)
		SendMessage(GetDlgItem(g_surfwin, g_checkboxes[32+i]), BM_SETCHECK, !!(pt->contents&(1<<i)), 0 );
*/
	SendMessage (g_surfwin, WM_SETREDRAW, 1, 0);
	InvalidateRect (g_surfwin, NULL, true);
}


/*
==============
GetTexMods

Reads the fields to get the current texdef
===============
*/
static void GetTexMods(void)
{
	char	sz[128];
	texdef_t *pt;

	pt = &g_qeglobals.d_texturewin.texdef;
	if (selected_face)
	{
		pt = &selected_face->texdef;
	}
	else if ((selected_brushes.next) && (selected_brushes.next != selected_brushes.next->next))
	{
		pt = &selected_brushes.next->brush_faces->texdef;
	}


	GetWindowText (GetDlgItem(g_surfwin, IDC_HSHIFT), sz, 127);
	pt->shift[0] = atof(sz);

	GetWindowText (GetDlgItem(g_surfwin, IDC_VSHIFT), sz, 127);
	pt->shift[1] = atof(sz);

	GetWindowText(GetDlgItem(g_surfwin, IDC_HSCALE), sz, 127);
	pt->scale[0] = atof(sz);

	GetWindowText(GetDlgItem(g_surfwin, IDC_VSCALE), sz, 127);
	pt->scale[1] = atof(sz);

	GetWindowText(GetDlgItem(g_surfwin, IDC_ROTATE), sz, 127);
	pt->rotate = atof(sz);

	Select_SetTexture(pt);
}

/*
=================
UpdateSpinners
=================
*/
static void UpdateSpinners(unsigned uMsg, WPARAM wParam, LPARAM lParam)
{
	int nScrollCode;
	HWND hwnd;
	texdef_t *pt;

	pt = &g_qeglobals.d_texturewin.texdef;
	if (selected_face)
	{
		pt = &selected_face->texdef;
	}
	else if ((selected_brushes.next) && (selected_brushes.next != selected_brushes.next->next))
	{
		pt = &selected_brushes.next->brush_faces->texdef;
	}

	nScrollCode = (int) LOWORD(wParam);  // scroll bar value 
	hwnd = (HWND) lParam;       // handle of scroll bar 

	if ((nScrollCode != SB_LINEUP) && (nScrollCode != SB_LINEDOWN))
		return;
	
	if (hwnd == GetDlgItem(g_surfwin, IDC_ROTATEA))
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

	else if (hwnd == GetDlgItem(g_surfwin, IDC_HSCALEA))
	{
		if (nScrollCode == SB_LINEDOWN)
			pt->scale[0] -= 0.1;
		else
			pt->scale[0] += 0.1;
	}
	
	else if (hwnd == GetDlgItem(g_surfwin, IDC_VSCALEA))
	{
		if (nScrollCode == SB_LINEUP)
			pt->scale[1] += 0.1;
		else
			pt->scale[1] -= 0.1;
	} 
	
	else if (hwnd == GetDlgItem(g_surfwin, IDC_HSHIFTA))
	{
		if (nScrollCode == SB_LINEDOWN)
			pt->shift[0] -= 8;
		else
			pt->shift[0] += 8;
	}
	
	else if (hwnd == GetDlgItem(g_surfwin, IDC_VSHIFTA))
	{
		if (nScrollCode == SB_LINEUP)
			pt->shift[1] += 8;
		else
			pt->shift[1] -= 8;
	}

	SetTexMods();
	Select_SetTexture(pt);
}


BOOL CALLBACK TexModifyProc (
    HWND hwndDlg,	// handle to dialog box
    UINT uMsg,	// message
    WPARAM wParam,	// first message parameter
    LPARAM lParam 	// second message parameter
   )
{
	int		i;

	switch (uMsg)
    {
	case WM_INITDIALOG:
		g_surfwin = hwndDlg;
		for(i=0; i<5; i++)
		{
			hwndTM[i] = GetDlgItem(hwndDlg, texModControl[i]);		
			if(i==0)
			{
				OldFieldWindowProc = (void *)GetWindowLong (hwndTM[i], GWL_WNDPROC);
			}
			SetWindowLong (hwndTM[i], GWL_WNDPROC, (long)FieldWndProc);
		}
		SetTexMods ();
		return FALSE;
	case WM_COMMAND:
		if(LOWORD(wParam) == IDC_TEXLOCK_CHECK)
		{
			switch(IsDlgButtonChecked(hwndDlg, IDC_TEXLOCK_CHECK))
			{
			case 0:	//off, turn 'em on
				ResetCheck(1);
				break;
			case 1:	//on, turn 'em off
			case 2:	//undecided, turn 'em off
				ResetCheck(0);
				break;
			}
		}
		return FALSE;

	case WM_LBUTTONDOWN:
		SetFocus(hwndDlg);
		SetTexMods ();
		return FALSE;
	case WM_HSCROLL:
	case WM_VSCROLL:
		UpdateSpinners(uMsg, wParam, lParam);
		InvalidateRect(g_qeglobals.d_hwndCamera, NULL, false);
		UpdateWindow (g_qeglobals.d_hwndCamera);
		return 0;

	default:
		return FALSE;
	}
}

