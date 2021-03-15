//
// p_dll.c
//
// Copyright 1998 Raven Software
//
// Heretic II
//

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include "player.h"
#include "g_local.h"
#include "qcommon.h"

PLAYER_API player_export_t GetPlayerAPI(void);

// Structure containing functions and data pointers exported from the player DLL.

player_export_t	playerExport;

// Handle to player DLL. 

static HINSTANCE player_library = NULL;

// ************************************************************************************************
// P_Freelib
// ---------
// ************************************************************************************************

void P_Freelib()
{
	P_Shutdown();
}

// ************************************************************************************************
// P_Load
// ------
// ************************************************************************************************
unsigned int P_Load(char *name)
{
	P_Init();

	playerExport = GetPlayerAPI();
	return 0;
}

