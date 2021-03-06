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

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	std::string cmdline = lpCmdLine;
	cmdline += " +set gl_driver opengl32 +set vid_ref gl ";

	if(!strstr(lpCmdLine, "+set game")) {
		cmdline += " +set game portals";
	}

	return Quake2Main(hInstance, hPrevInstance, (char *)cmdline.c_str());
}
