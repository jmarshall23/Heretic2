// launcher.cpp
//

#include <windows.h>
#include <string>
#include "MinHook.h"
#include "MinHook.h"
#include "../game/q_shared.h"
#include "../client/ref.h"

#include "launcher.h"

extern "C" int __cdecl Quake2Main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine);
void(*Com_ColourPrintfEngine)(PalIdx_t color, char *fmt, ...);
void(*Com_PrintfEngine)(char *fmt, ...);
void(*Com_ErrorEngine)(int type, char *format, ...);
void(*VID_CheckChangesEngine)(void);
void(*Com_CheckCopyProtectionEngine)(void);
void(*Com_CheckCPUInfoEngine)(void);
void(*Sys_InitEngine)(void);
void(*Sys_ErrorEngine)(const char *error, ...);

//
// Com_ColourPrintf
//
void Com_ColourPrintf(PalIdx_t color, char *format, ...) {
	va_list		argptr;
	static char		string[1024];

	if (strstr(format, "GL_EXTENSIONS")) {
		return;
	}

	va_start(argptr, format);
	vsprintf(string, format, argptr);
	va_end(argptr);
	Com_ColourPrintfEngine(color, string);
	OutputDebugStringA(string);
}

//
// Com_Printf
//
void Com_Printf(char *format, ...) {
	va_list		argptr;
	static char		string[1024];

	if (strstr(format, "GL_EXTENSIONS")) {
		return;
	}

	va_start(argptr, format);
	vsprintf(string, format, argptr);
	va_end(argptr);
	Com_ColourPrintf(P_WHITE, string);
	Com_ColourPrintf(P_WHITE, "\n");
}


//
// Com_Error
//
void Com_Error(int type, char *format, ...) {
	va_list		argptr;
	static char		string[1024];

	va_start(argptr, format);
	vsprintf(string, format, argptr);
	va_end(argptr);
	OutputDebugStringA(string);
	Com_ErrorEngine(type, string);
}

void Sys_Error2(const char *error, ...) {

}

void(*VID_LoadLibrary)(const char *) = (void(__cdecl *)(const char *))0x10034D90;

//
// VID_CheckChanges
//
void VID_CheckChanges(void) {
	static bool videoNeedsSetup = true;

	if (!videoNeedsSetup)
		return;

	VID_LoadLibrary("ref_gl.dll");

	videoNeedsSetup = false;
}

//
// Com_CheckCopyProtection
//
void Com_CheckCopyProtection(void) {
	// Don't do the CD check here.
}

//
// Com_CheckCPUInfo
//
void Com_CheckCPUInfo(void) {
	
}

//
// Sys_Init
//
void Sys_Init(void) {
	// There was a call here binding printf in quake2.dll that was causing some kind of heap curroption.
	// TODO: Implemenet console stuff for dedicated if need be. Originall this code did that and windows
	// version checks.

	timeBeginPeriod(1);
}

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	std::string cmdline = lpCmdLine;
	cmdline += " +set gl_driver opengl32 +set vid_ref gl ";

	if(!strstr(lpCmdLine, "+set game")) {
		cmdline += " +set game portals";
	}

	MH_Initialize();

	HMODULE q2module = LoadLibrary("quake2.dll");
	{
		void *function = (LPVOID)GetProcAddress(q2module, "Com_ColourPrintf");
		MH_CreateHook(function, Com_ColourPrintf, (LPVOID *)&Com_ColourPrintfEngine);
		MH_EnableHook(function);
	}
	{
		void *function = (LPVOID)GetProcAddress(q2module, "Com_Printf");
		MH_CreateHook(function, Com_Printf, (LPVOID *)&Com_PrintfEngine);
		MH_EnableHook(function);
	}
	{
		void *function = (LPVOID)GetProcAddress(q2module, "Com_Error");
		MH_CreateHook(function, Com_Error, (LPVOID *)&Com_ErrorEngine);
		MH_EnableHook(function);
	}

	{
		void *function = (LPVOID)0x10034F80;
		MH_CreateHook(function, VID_CheckChanges, (LPVOID *)&VID_CheckChangesEngine);
		MH_EnableHook(function);
	}

	{
		void *function = (LPVOID)0x10033650;
		MH_CreateHook(function, Sys_Error2, (LPVOID *)&Sys_ErrorEngine);
		MH_EnableHook(function);
	}

	{
		void *function = (LPVOID)0x10033870;
		MH_CreateHook(function, Com_CheckCopyProtection, (LPVOID *)&Com_CheckCopyProtectionEngine);
		MH_EnableHook(function);
	}

	{
		void *function = (LPVOID)0x10013C80;
		MH_CreateHook(function, Com_CheckCPUInfo, (LPVOID *)&Com_CheckCPUInfoEngine);
		MH_EnableHook(function);
	}

	{
		void *function = (LPVOID)0x100338B0;
		MH_CreateHook(function, Sys_Init, (LPVOID *)&Sys_InitEngine);
		MH_EnableHook(function);
	}

	// Force load the library.
	//HMODULE ref_gl_module = LoadLibrary("ref_gl.dll");
	//
	//{
	//	void *function = GetProcAddress(ref_gl_module, "GetRefAPI");
	//	MH_CreateHook(function, GetRefAPI, (LPVOID *)&GetRefAPIEngine_t);
	//	MH_EnableHook(function);
	//}
	//
	//{
	//	void *function2 = (LPVOID)0x109E4090;
	//	MH_CreateHook(function2, &GL_FindImage, (LPVOID *)&GL_FindImageEngine);
	//	MH_EnableHook(function2);
	//}

	// Force load the WndSnd dll so we can hook the cdaudio stuff.
	HMODULE wndsnd_module = LoadLibrary("WinSnd.dll");
	{
		void *old_function;

		void *function = (void *)0x10001000; // CDAudio_Play
		void *new_function = GetProcAddress(wndsnd_module, "S_PlayMusic");
		MH_CreateHook(function, new_function, (LPVOID *)&old_function);
		MH_EnableHook(function);
	}

	return Quake2Main(hInstance, hPrevInstance, (char *)cmdline.c_str());
}
