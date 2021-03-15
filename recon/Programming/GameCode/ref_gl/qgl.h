/*
Copyright (C) 1997-2001 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/
/*
** gl.H
*/

#ifndef __gl_H__
#define __gl_H__

#include "gl/glew.h"

qboolean QGL_Init(const char* dllname);
void GLimp_EnableLogging(qboolean enable);
void QGL_Shutdown(void);

extern  int   (WINAPI* qwglChoosePixelFormat)(HDC, CONST PIXELFORMATDESCRIPTOR*);
extern  int   (WINAPI* qwglDescribePixelFormat) (HDC, int, UINT, LPPIXELFORMATDESCRIPTOR);
extern  int   (WINAPI* qwglGetPixelFormat)(HDC);
extern  BOOL(WINAPI* qwglSetPixelFormat)(HDC, int, CONST PIXELFORMATDESCRIPTOR*);
extern  BOOL(WINAPI* qwglSwapBuffers)(HDC);

extern BOOL(WINAPI* qwglCopyContext)(HGLRC, HGLRC, UINT);
extern HGLRC(WINAPI* qwglCreateContext)(HDC);
extern HGLRC(WINAPI* qwglCreateLayerContext)(HDC, int);
extern BOOL(WINAPI* qwglDeleteContext)(HGLRC);
extern HGLRC(WINAPI* qwglGetCurrentContext)(VOID);
extern HDC(WINAPI* qwglGetCurrentDC)(VOID);
extern PROC(WINAPI* qwglGetProcAddress)(LPCSTR);
extern BOOL(WINAPI* qwglMakeCurrent)(HDC, HGLRC);
extern BOOL(WINAPI* qwglShareLists)(HGLRC, HGLRC);
extern BOOL(WINAPI* qwglUseFontBitmaps)(HDC, DWORD, DWORD, DWORD);

extern BOOL(WINAPI* qwglUseFontOutlines)(HDC, DWORD, DWORD, DWORD, FLOAT,
	FLOAT, int, LPGLYPHMETRICSFLOAT);

extern BOOL(WINAPI* qwglDescribeLayerPlane)(HDC, int, int, UINT,
	LPLAYERPLANEDESCRIPTOR);
extern int  (WINAPI* qwglSetLayerPaletteEntries)(HDC, int, int, int,
	CONST COLORREF*);
extern int  (WINAPI* qwglGetLayerPaletteEntries)(HDC, int, int, int,
	COLORREF*);
extern BOOL(WINAPI* qwglRealizeLayerPalette)(HDC, int, BOOL);
extern BOOL(WINAPI* qwglSwapLayerBuffers)(HDC, UINT);

extern BOOL(WINAPI* qwglSwapIntervalEXT)(int interval);

extern BOOL(WINAPI* qwglGetDeviceGammaRampEXT) (unsigned char* pRed, unsigned char* pGreen, unsigned char* pBlue);
extern BOOL(WINAPI* qwglSetDeviceGammaRampEXT) (const unsigned char* pRed, const unsigned char* pGreen, const unsigned char* pBlue);


#endif
