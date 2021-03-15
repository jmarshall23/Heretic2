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

client_fx_export_t GetfxAPI(client_fx_import_t import);

typedef struct sfx_t;

void CL_LogoutEffect (vec3_t org, int type);
void CL_ItemRespawnParticles (vec3_t org);

ResourceManager_t FXBufMgnr;

static vec3_t avelocities [NUMVERTEXNORMALS];

extern	struct model_s	*cl_mod_smoke;
extern	struct model_s	*cl_mod_flash;

void SV_NewTrace(vec3_t start, vec3_t mins, vec3_t maxs, vec3_t end, edict_t* passent, int contentmask, trace_t* tr);
void Sys_LoadGameDll(const char* name, HINSTANCE* hinst, DWORD* chkSum);
void Sys_UnloadGameDll(const char* name, HINSTANCE* hinst);
void S_StartSound(vec3_t origin, int entnum, int entchannel, void* sfx, float fvol, int attenuation, float timeofs);
sfx_s* S_RegisterSound(char* name);
int CL_GetEffect(centity_t* ent, int flags, char* format, ...) { return 0; }

void CL_ShutdownClientEffects()
{
	fxe.ShutDown();
}

qboolean InCameraPVS(vec3_t point) {
	return true;
}

// Screen flash set
void Activate_Screen_Flash(int color) {

}

// Screen flash set
void Activate_Screen_Shake(float intensity, float duration, float current_time, int flags) {

}

// Screen flash unset
void Deactivate_Screen_Flash(void) {

}

void Deactivate_Screen_Shake(void) {

}

qboolean Get_Crosshair(vec3_t origin, byte* type) {
	return true;
}

trace_t		CL_PMTrace(vec3_t start, vec3_t mins, vec3_t maxs, vec3_t end);
void CL_NewTrace(vec3_t start, vec3_t mins, vec3_t maxs, vec3_t end, int brushmask, int flags, trace_t* t) {
	*t = CL_PMTrace(start, mins, maxs, end); // jmarshall: incomplete
}

void CL_Sys_Error(int errLevel, char* fmt, ...)
{
	va_list		argptr;
	char		msg[4096];

	va_start(argptr, fmt);
	vsprintf(msg, fmt, argptr);
	va_end(argptr);

	Sys_Error(msg); // TODO vargs
}

void CL_Printf(int errLevel, char* fmt, ...) {
	va_list		argptr;
	char		msg[4096];

	va_start(argptr, fmt);
	vsprintf(msg, fmt, argptr);
	va_end(argptr);

	Com_Printf(msg);
}

int CL_InitClientEffects(const char* name)
{
	int result; // eax
	DWORD chkSum; // [esp+0h] [ebp-4h] BYREF
	static client_fx_import_t cl_game_import;


	Com_Printf("------ Loading %s ------\n", name);

	cl_game_import.cl_predict = cl_predict;
	cl_game_import.cl = &cl;
	cl_game_import.cls = &cls;
	cl_game_import.server_entities = &cl_entities[0];
	cl_game_import.parse_entities = cl_parse_entities;
	cl_game_import.EffectEventIdTimeArray = EffectEventIdTimeArray;
	cl_game_import.leveltime = (float*)&cl.time;
	cl_game_import.Highestleveltime = (float*)&cl.time;
	cl_game_import.clientPredEffects = &clientPredEffects;
	cl_game_import.net_message = &net_message;
	cl_game_import.PlayerEntPtr = (entity_t**)&cl_entities[0];
	cl_game_import.PlayerAlpha = (float*)&PlayerAlpha;
	cl_game_import.predictinfo = &predictInfo;
	cl_game_import.FXBufMngr = &FXBufMgnr;
	cl_game_import.cl_effectpredict = &cl_effectpredict[0];
	cl_game_import.Sys_Error = CL_Sys_Error;
	cl_game_import.Com_Error = Com_Error;
	cl_game_import.Con_Printf = CL_Printf;
	cl_game_import.Cvar_Get = Cvar_Get;
	cl_game_import.Cvar_Set = Cvar_Set;
	cl_game_import.Cvar_SetValue = Cvar_SetValue;
	cl_game_import.Cvar_VariableValue = Cvar_VariableValue;
	cl_game_import.Cvar_VariableString = Cvar_VariableString;
	cl_game_import.Activate_Screen_Flash = Activate_Screen_Flash;
	cl_game_import.Activate_Screen_Shake = Activate_Screen_Shake;
	cl_game_import.Get_Crosshair = Get_Crosshair;
	cl_game_import.S_StartSound = (void(__cdecl*)(vec_t[], int, int, sfx_s*, float, int, float))S_StartSound;
	cl_game_import.S_RegisterSound = (sfx_s * (__cdecl*)(char*))S_RegisterSound;
	cl_game_import.RegisterModel = re.RegisterModel;
	cl_game_import.GetEffect = CL_GetEffect;
	cl_game_import.TagMalloc = Z_TagMalloc;
	cl_game_import.TagFree = Z_Free;
	cl_game_import.FreeTags = Z_FreeTags;
	cl_game_import.Trace = CL_NewTrace;
	cl_game_import.InCameraPVS = InCameraPVS;
	cl_game_import.FindSurface = re.FindSurface;
	cl_game_import.GetReferencedID = re.GetReferencedID;

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
