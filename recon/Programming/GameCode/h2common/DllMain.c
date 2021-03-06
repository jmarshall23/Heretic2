// DllMain.cpp
//

#include <windows.h>
#include "../qcommon/ResourceManager.h"

ResourceManager_t globalResourceManager;

/*
===============
InitResourceManager
===============
*/
void InitResourceManager()
{
	return ResMngr_Con(&globalResourceManager, 8, 256);
}

/*
===============
ShutdownResourceManager
===============
*/

void ShutdownResourceManager()
{
	ResMngr_Des(&globalResourceManager);
}

//
// DllMain
//
BOOL __stdcall DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason)
	{
		if (fdwReason == 1)
		{
			InitResourceManager();
			return 1;
		}
	}
	else
	{
		ShutdownResourceManager();
	}
	return 1;
}
