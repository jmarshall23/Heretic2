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
// cl_fx.c -- entity effects parsing and management

#include "client.h"
#include <windows.h>
#include "../qcommon/ResourceManager.h"

client_fx_export_t fxe;

float EffectEventIdTimeArray[1000];
int cl_effectpredict[1000];

predictinfo_t predictInfo;
EffectsBuffer_t clientPredEffects;
float PlayerAlpha = 1.0f;

typedef struct sfx_t;

void CL_LogoutEffect (vec3_t org, int type);
void CL_ItemRespawnParticles (vec3_t org);

ResourceManager_t FXBufMgnr;

HINSTANCE clgame_module_handle = NULL;

static vec3_t avelocities [NUMVERTEXNORMALS];

extern	struct model_s	*cl_mod_smoke;
extern	struct model_s	*cl_mod_flash;

void SV_NewTrace(vec3_t start, vec3_t mins, vec3_t maxs, vec3_t end, edict_t* passent, int contentmask, trace_t* tr);
void Sys_LoadGameDll(const char* name, HINSTANCE* hinst, DWORD* chkSum);
void Sys_UnloadGameDll(const char* name, HINSTANCE* hinst);
void S_StartSound(vec3_t origin, int entnum, int entchannel, void* sfx, float fvol, int attenuation, float timeofs);
void* S_RegisterSound(char* name);
int CL_GetEffect(centity_t* ent, int flags, char* format, ...) { return 0; }
client_fx_export_t (*GetfxAPI)(client_fx_import_t import);

void CL_ShutdownClientEffects()
{
	Sys_UnloadGameDll("client effects", &clgame_module_handle);
}

qboolean InCameraPVS(vec3_t point) {
	return true;
}

// Screen flash set
void Activate_Screen_Flash(int color) {

}

// Screen flash set
void Activate_Screen_Shake(int color) {

}

// Screen flash unset
void Deactivate_Screen_Flash(void) {

}

void Deactivate_Screen_Shake(void) {

}

void VectorNegate(vec3_t in, vec3_t out)
{
	assert(out != vec3_origin);

	out[0] = -in[0];
	out[1] = -in[1];
	out[2] = -in[2];
}

void VectorClear(vec3_t in)
{
	assert(in != vec3_origin);

	in[0] = 0;
	in[1] = 0;
	in[2] = 0;
}

qboolean Get_Crosshair(vec3_t origin, byte* type) {
	return true;
}

int CL_InitClientEffects(const char* name)
{
	int result; // eax
	DWORD chkSum; // [esp+0h] [ebp-4h] BYREF
	static client_fx_import_t cl_game_import;

	if (clgame_module_handle)
		CL_ShutdownClientEffects();

	Com_Printf("------ Loading %s ------\n", name);

	Sys_LoadGameDll(name, &clgame_module_handle, &chkSum);
	cl_game_import.cl_predict = cl_predict;
	cl_game_import.cl = &cl;
	cl_game_import.cls = &cls;
	cl_game_import.server_entities = &cl_entities;
	cl_game_import.parse_entities = cl_parse_entities;
	cl_game_import.EffectEventIdTimeArray = EffectEventIdTimeArray;
	cl_game_import.leveltime = (float*)&cl.time;
	cl_game_import.Highestleveltime = (float*)&cl.time;
	cl_game_import.clientPredEffects = &clientPredEffects;
	cl_game_import.net_message = &net_message;
	cl_game_import.PlayerEntPtr = (void**)&cl_entities[0];
	cl_game_import.PlayerAlpha = (float*)&PlayerAlpha;
	cl_game_import.predictinfo = &predictInfo;
	cl_game_import.FXBufMngr = &FXBufMgnr;
	cl_game_import.cl_effectpredict = &cl_effectpredict;
	cl_game_import.Sys_Error = Sys_Error;
	cl_game_import.Com_Error = Com_Error;
	cl_game_import.Con_Printf = Com_Printf;
	cl_game_import.Cvar_Get = Cvar_Get;
	cl_game_import.Cvar_Set = Cvar_Set;
	cl_game_import.Cvar_SetValue = Cvar_SetValue;
	cl_game_import.Cvar_VariableValue = Cvar_VariableValue;
	cl_game_import.Cvar_VariableString = Cvar_VariableString;
	cl_game_import.Activate_Screen_Flash = Activate_Screen_Flash;
	cl_game_import.Activate_Screen_Shake = Activate_Screen_Shake;
	cl_game_import.Get_Crosshair = Get_Crosshair;
	cl_game_import.S_StartSound = S_StartSound;
	cl_game_import.S_RegisterSound = S_RegisterSound;
	cl_game_import.RegisterModel = re.RegisterModel;
	cl_game_import.GetEffect = CL_GetEffect;
	cl_game_import.TagMalloc = Z_TagMalloc;
	cl_game_import.TagFree = Z_Free;
	cl_game_import.FreeTags = Z_FreeTags;
	cl_game_import.Trace = SV_NewTrace;
	cl_game_import.InCameraPVS = InCameraPVS;
	cl_game_import.FindSurface = re.FindSurface;
	cl_game_import.GetReferencedID = re.GetReferencedID;

	GetfxAPI = (client_fx_export_t(__cdecl*)(client_fx_import_t))GetProcAddress(clgame_module_handle, "GetfxAPI");
	if (!GetfxAPI)
		Com_Error(0, "GetProcAddress failed on %s", name);

	fxe = GetfxAPI(cl_game_import);
	if (fxe.api_version != 3)
	{
		CL_ShutdownClientEffects();
		Com_Error(0, "%s has incompatible api_version", name);
	}
	ResMngr_Con(&FXBufMgnr, 192, 256);
	Com_Printf("------------------------------------");
	result = 1;

	fxe.Init();

	return result;
}

/*
==============================================================

LIGHT STYLE MANAGEMENT

==============================================================
*/

typedef struct
{
	int		length;
	float	value[3];
	float	map[MAX_QPATH];
} clightstyle_t;

clightstyle_t	cl_lightstyle[MAX_LIGHTSTYLES];
int			lastofs;

/*
================
CL_ClearLightStyles
================
*/
void CL_ClearLightStyles (void)
{
	memset (cl_lightstyle, 0, sizeof(cl_lightstyle));
	lastofs = -1;
}

/*
================
CL_RunLightStyles
================
*/
void CL_RunLightStyles (void)
{
	int		ofs;
	int		i;
	clightstyle_t	*ls;

	ofs = cl.time / 100;
	if (ofs == lastofs)
		return;
	lastofs = ofs;

	for (i=0,ls=cl_lightstyle ; i<MAX_LIGHTSTYLES ; i++, ls++)
	{
		if (!ls->length)
		{
			ls->value[0] = ls->value[1] = ls->value[2] = 1.0;
			continue;
		}
		if (ls->length == 1)
			ls->value[0] = ls->value[1] = ls->value[2] = ls->map[0];
		else
			ls->value[0] = ls->value[1] = ls->value[2] = ls->map[ofs%ls->length];
	}
}


void CL_SetLightstyle (int i)
{
	fxe.SetLightstyle(i);
}

/*
================
CL_AddLightStyles
================
*/
void CL_AddLightStyles (void)
{
	int		i;
	clightstyle_t	*ls;

	for (i=0,ls=cl_lightstyle ; i<MAX_LIGHTSTYLES ; i++, ls++)
		V_AddLightStyle (i, ls->value[0], ls->value[1], ls->value[2]);
}

/*
==============================================================

DLIGHT MANAGEMENT

==============================================================
*/

cdlight_t		cl_dlights[MAX_DLIGHTS];

/*
================
CL_ClearDlights
================
*/
void CL_ClearDlights (void)
{
	memset (cl_dlights, 0, sizeof(cl_dlights));
}

/*
===============
CL_AllocDlight

===============
*/
cdlight_t *CL_AllocDlight (int key)
{
	int		i;
	cdlight_t	*dl;

// first look for an exact key match
	if (key)
	{
		dl = cl_dlights;
		for (i=0 ; i<MAX_DLIGHTS ; i++, dl++)
		{
			if (dl->key == key)
			{
				memset (dl, 0, sizeof(*dl));
				dl->key = key;
				return dl;
			}
		}
	}

// then look for anything else
	dl = cl_dlights;
	for (i=0 ; i<MAX_DLIGHTS ; i++, dl++)
	{
		if (dl->die < cl.time)
		{
			memset (dl, 0, sizeof(*dl));
			dl->key = key;
			return dl;
		}
	}

	dl = &cl_dlights[0];
	memset (dl, 0, sizeof(*dl));
	dl->key = key;
	return dl;
}

/*
===============
CL_NewDlight
===============
*/
void CL_NewDlight (int key, float x, float y, float z, float radius, float time)
{
	cdlight_t	*dl;

	dl = CL_AllocDlight (key);
	dl->origin[0] = x;
	dl->origin[1] = y;
	dl->origin[2] = z;
	dl->radius = radius;
	dl->die = cl.time + time;
}


/*
===============
CL_RunDLights

===============
*/
void CL_RunDLights (void)
{
	int			i;
	cdlight_t	*dl;

	dl = cl_dlights;
	for (i=0 ; i<MAX_DLIGHTS ; i++, dl++)
	{
		if (!dl->radius)
			continue;
		
		if (dl->die < cl.time)
		{
			dl->radius = 0;
			return;
		}
		dl->radius -= cls.frametime*dl->decay;
		if (dl->radius < 0)
			dl->radius = 0;
	}
}

/*
===============
CL_AddDLights

===============
*/
void CL_AddDLights (void)
{
	int			i;
	cdlight_t	*dl;

	dl = cl_dlights;

	for (i = 0; i < MAX_DLIGHTS; i++, dl++)
	{
		if (!dl->radius)
			continue;

		// negative light in software. only black allowed
		if ((dl->color[0] < 0) || (dl->color[1] < 0) || (dl->color[2] < 0))
		{
			dl->radius = -(dl->radius);
			dl->color[0] = 1;
			dl->color[1] = 1;
			dl->color[2] = 1;
		}
		V_AddLight(dl->origin, dl->radius,
			dl->color[0], dl->color[1], dl->color[2]);
	}
}
