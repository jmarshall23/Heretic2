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
// sv_game.c -- interface to the game dll

#include "server.h"
#include "../qcommon/SinglyLinkedList.h"
#include "../game/g_physics.h"

game_export_t	*ge;

byte SV_Persistant_Effects_Array[0x1B000];

HINSTANCE game_module_handle;

void Sys_LoadGameDll(const char* gamename, HINSTANCE* hinst, DWORD* chkSum);
void Sys_UnloadGameDll(const char* name, HINSTANCE* hinst);

/*
===============
PF_Unicast

Sends the contents of the mutlicast buffer to a single client
===============
*/
void PF_Unicast (edict_t *ent, qboolean reliable)
{
	int		p;
	client_t	*client;

	if (!ent)
		return;

	p = NUM_FOR_EDICT(ent);
	if (p < 1 || p > maxclients->value)
		return;

	client = svs.clients + (p-1);

	if (reliable)
		SZ_Write (&client->netchan.message, sv.multicast.data, sv.multicast.cursize);
	else
		SZ_Write (&client->datagram, sv.multicast.data, sv.multicast.cursize);

	SZ_Clear (&sv.multicast);
}


/*
===============
PF_dprintf

Debug print to server console
===============
*/
void PF_dprintf (char *fmt, ...)
{
	char		msg[1024];
	va_list		argptr;
	
	va_start (argptr,fmt);
	vsprintf (msg, fmt, argptr);
	va_end (argptr);

	Com_Printf ("%s", msg);
}


/*
===============
PF_cprintf

Print to a single client
===============
*/
void PF_cprintf (edict_t *ent, int level, char *fmt, ...)
{
	char		msg[1024];
	va_list		argptr;
	int			n;

	if (ent)
	{
		n = NUM_FOR_EDICT(ent);
		if (n < 1 || n > maxclients->value)
			Com_Error (ERR_DROP, "cprintf to a non-client");
	}

	va_start (argptr,fmt);
	vsprintf (msg, fmt, argptr);
	va_end (argptr);

	if (ent)
		SV_ClientPrintf (svs.clients+(n-1), level, "%s", msg);
	else
		Com_Printf ("%s", msg);
}


/*
===============
PF_centerprintf

centerprint to a single client
===============
*/
void PF_centerprintf (edict_t *ent, char *fmt, ...)
{
	char		msg[1024];
	va_list		argptr;
	int			n;
	
	n = NUM_FOR_EDICT(ent);
	if (n < 1 || n > maxclients->value)
		return;	// Com_Error (ERR_DROP, "centerprintf to a non-client");

	va_start (argptr,fmt);
	vsprintf (msg, fmt, argptr);
	va_end (argptr);

	MSG_WriteByte (&sv.multicast,svc_centerprint);
	MSG_WriteString (&sv.multicast,msg);
	PF_Unicast (ent, true);
}


/*
===============
PF_error

Abort the server with a game error
===============
*/
void PF_error (char *fmt, ...)
{
	char		msg[1024];
	va_list		argptr;
	
	va_start (argptr,fmt);
	vsprintf (msg, fmt, argptr);
	va_end (argptr);

	Com_Error (ERR_DROP, "Game Error: %s", msg);
}


/*
=================
PF_setmodel

Also sets mins and maxs for inline bmodels
=================
*/
void PF_setmodel (edict_t *ent, char *name)
{
	int		i;
	cmodel_t	*mod;

	if (!name)
		Com_Error (ERR_DROP, "PF_setmodel: NULL");

	i = SV_ModelIndex (name);
		
//	ent->model = name;
	ent->s.modelindex = i;

// if it is an inline model, get the size information for it
	if (name[0] == '*')
	{
		mod = CM_InlineModel (name);
		VectorCopy (mod->mins, ent->mins);
		VectorCopy (mod->maxs, ent->maxs);
		SV_LinkEdict (ent);
	}

}

/*
===============
PF_Configstring

===============
*/
void PF_Configstring (int index, char *val)
{
	if (index < 0 || index >= MAX_CONFIGSTRINGS)
		Com_Error (ERR_DROP, "configstring: bad index %i\n", index);

	if (!val)
		val = "";

	// change the string in sv
	strcpy (sv.configstrings[index], val);
	
	if (sv.state != ss_loading)
	{	// send the update to everyone
		SZ_Clear (&sv.multicast);
		MSG_WriteChar (&sv.multicast, svc_configstring);
		MSG_WriteShort (&sv.multicast, index);
		MSG_WriteString (&sv.multicast, val);

		SV_Multicast (vec3_origin, MULTICAST_ALL_R);
	}
}



void PF_WriteChar (int c) {MSG_WriteChar (&sv.multicast, c);}
void PF_WriteByte (int c) {MSG_WriteByte (&sv.multicast, c);}
void PF_WriteShort (int c) {MSG_WriteShort (&sv.multicast, c);}
void PF_WriteLong (int c) {MSG_WriteLong (&sv.multicast, c);}
void PF_WriteFloat (float f) {MSG_WriteFloat (&sv.multicast, f);}
void PF_WriteString (char *s) {MSG_WriteString (&sv.multicast, s);}
void PF_WritePos (vec3_t pos) {MSG_WritePos (&sv.multicast, pos);}
void PF_WriteDir (vec3_t dir) {MSG_WriteDir (&sv.multicast, dir);}
void PF_WriteAngle (float f) {MSG_WriteAngle (&sv.multicast, f);}


/*
=================
PF_inPVS

Also checks portalareas so that doors block sight
=================
*/
qboolean PF_inPVS (vec3_t p1, vec3_t p2)
{
	int		leafnum;
	int		cluster;
	int		area1, area2;
	byte	*mask;

	leafnum = CM_PointLeafnum (p1);
	cluster = CM_LeafCluster (leafnum);
	area1 = CM_LeafArea (leafnum);
	mask = CM_ClusterPVS (cluster);

	leafnum = CM_PointLeafnum (p2);
	cluster = CM_LeafCluster (leafnum);
	area2 = CM_LeafArea (leafnum);
	if ( mask && (!(mask[cluster>>3] & (1<<(cluster&7)) ) ) )
		return false;
	if (!CM_AreasConnected (area1, area2))
		return false;		// a door blocks sight
	return true;
}


/*
=================
PF_inPHS

Also checks portalareas so that doors block sound
=================
*/
qboolean PF_inPHS (vec3_t p1, vec3_t p2)
{
	int		leafnum;
	int		cluster;
	int		area1, area2;
	byte	*mask;

	leafnum = CM_PointLeafnum (p1);
	cluster = CM_LeafCluster (leafnum);
	area1 = CM_LeafArea (leafnum);
	mask = CM_ClusterPHS (cluster);

	leafnum = CM_PointLeafnum (p2);
	cluster = CM_LeafCluster (leafnum);
	area2 = CM_LeafArea (leafnum);
	if ( mask && (!(mask[cluster>>3] & (1<<(cluster&7)) ) ) )
		return false;		// more than one bounce away
	if (!CM_AreasConnected (area1, area2))
		return false;		// a door blocks hearing

	return true;
}

void PF_StartSound (edict_t *entity, int channel, int sound_num, float volume,
    float attenuation, float timeofs)
{
	if (!entity)
		return;
	SV_StartSound (NULL, entity, channel, sound_num, volume, attenuation, timeofs);
}

//==============================================

/*
===============
SV_ShutdownGameProgs

Called when either the entire server is being killed, or
it is changing to a different game directory.
===============
*/
void SV_ShutdownGameProgs (void)
{
	if (!ge)
		return;
	ge->Shutdown ();
	Sys_UnloadGameDll("game", &game_module_handle);
	ge = NULL;
}

/*
===============
SV_InitGameProgs

Init the game subsystem for a new map
===============
*/
void SCR_DebugGraph (float value, int color);

void SV_BCaption(int printlevel, short stringid) {

}

void SV_BroadcastObituary(int printlevel, short stringid, short client1, short client2) {

}

void	SV_CreateEffect (entity_state_t* ent, int type, int flags, vec3_t origin, char* format, ...) { }
void	SV_RemoveEffects(entity_state_t* ent, int type) { }
void	SV_CreateEffectEvent (byte EventId, entity_state_t* ent, int type, int flags, vec3_t origin, char* format, ...) { }
void	SV_RemoveEffectsEvent(byte EventId, entity_state_t* ent, int type) { }
int		SV_CreatePersistantEffect(entity_state_t* ent, int type, int flags, vec3_t origin, char* format, ...) { return 0; }
void	SV_GameMsgCenterPrintf(edict_t* ent, short msg) { }
void	SV_LevelMsgCenterPrintf(edict_t* ent, short msg) { }
void	SV_MsgVarCenterPrintf(edict_t* ent, short msg, int vari) { }
void	SV_MsgDualCenterPrintf(edict_t* ent, short msg1, short msg2) { }
void	SV_CaptionPrintf(edict_t* ent, short msg) { }
void	SV_ChangeCDTrack(edict_t* ent, int track, int loop) { }
void	SV_CLPrintf(edict_t* ent, edict_t* from, int color, char* fmt, ...) { }
qboolean SV_ResizeBoundingForm (edict_t* self, struct FormMove_s* formMove) { return false; }
qboolean SV_CheckDistances(vec3_t origin, float dist) { return false; }
qboolean SV_RemovePersistantEffect(int toRemove, int call_from) { return false; }
void SV_SoundEvent(byte EventId, float leveltime, edict_t* ent, int channel, int soundindex, float volume, float attenuation, float timeofs) { }
void SV_ModelRemove (char* name) { };
void SV_SoundRemove(char* name) { };
void SV_CleanLevel(void) { };

void SV_ClearPersistantEffects(void) { }

int		 SV_GetContentsAtPoint(vec3_t point) {
	return SV_PointContents(point); // Not correct.
}

int		 SV_FindEntitiesInBounds(vec3_t mins, vec3_t maxs, struct SinglyLinkedList_s* list, int areatype) { 
	edict_t* idlist[1024];
	int numEnts;
	
	numEnts = SV_AreaEdicts(mins, maxs, idlist, 1024, areatype);

	for (int i = 0; i < numEnts; i++)
	{
		GenericUnion4_t temp;

		temp.t_void_p = idlist[i];
		SLList_Push(list, temp);
	}

	return numEnts; 
}

void SV_NewTrace(vec3_t start, vec3_t mins, vec3_t maxs, vec3_t end, edict_t* passent, int contentmask, trace_t* tr) {	
	*tr = SV_Trace(start, mins, maxs, end, passent, contentmask);
}

void	 SV_TraceBoundingForm(FormMove_t* formMove) {
	SV_NewTrace(formMove->start, formMove->mins, formMove->maxs, formMove->end, formMove->passEntity, formMove->clipMask, &formMove->trace);

	//formMove->trace = CM_BoxTrace(formMove->start, formMove->end, formMove->mins, formMove->maxs, 0, formMove->clipMask);
}

char* FS_Userdir(void) {
	return "";
}

void SV_InitGameProgs (void)
{
	game_import_t	import;
	game_export_t* (*GetGameApi)(game_import_t * import);

	// unload anything we have now
	if (ge)
		SV_ShutdownGameProgs ();


	// load a new game dll
	import.multicast = SV_Multicast;
	import.unicast = PF_Unicast;
	import.bprintf = SV_BroadcastPrintf;
	import.bcaption = SV_BCaption;
	import.Obituary = SV_BroadcastObituary;
	import.dprintf = PF_dprintf;
	import.cprintf = PF_cprintf;
	import.clprintf = SV_CLPrintf;
	import.centerprintf = PF_centerprintf;
	import.gamemsg_centerprintf = SV_GameMsgCenterPrintf;
	import.levelmsg_centerprintf = SV_LevelMsgCenterPrintf;
	import.captionprintf = SV_CaptionPrintf;
	import.msgvar_centerprintf = SV_MsgVarCenterPrintf;
	import.msgdual_centerprintf = SV_MsgDualCenterPrintf;
	import.error = PF_error;
	import.changeCDtrack = SV_ChangeCDTrack;
	import.linkentity = SV_LinkEdict;
	import.unlinkentity = SV_UnlinkEdict;
	import.BoxEdicts = SV_AreaEdicts;
	import.trace = SV_NewTrace;
	import.pointcontents = SV_PointContents;
	import.setmodel = PF_setmodel;
	import.inPVS = PF_inPVS;
	import.inPHS = PF_inPHS;
	import.Pmove = Pmove;
	import.FindEntitiesInBounds = SV_FindEntitiesInBounds;
	import.TraceBoundingForm = SV_TraceBoundingForm;
	import.ResizeBoundingForm = SV_ResizeBoundingForm;
	import.GetContentsAtPoint = SV_GetContentsAtPoint;
	import.CheckDistances = SV_CheckDistances;
	import.cleanlevel = SV_CleanLevel;
	import.modelindex = SV_ModelIndex;
	import.modelremove = SV_ModelRemove;
	import.soundindex = SV_SoundIndex;
	import.soundremove = SV_SoundRemove;
	import.imageindex = SV_ImageIndex;
	import.configstring = PF_Configstring;
	import.sound = PF_StartSound;
	import.soundevent = SV_SoundEvent;
	import.positioned_sound = SV_StartSound;
	import.WriteChar = PF_WriteChar;
	import.WriteByte = PF_WriteChar;
	import.WriteShort = PF_WriteShort;
	import.WriteLong = PF_WriteLong;
	import.WriteFloat = PF_WriteFloat;
	import.WriteString = PF_WriteString;
	import.WritePosition = PF_WritePos;
	import.WriteDir = PF_WriteDir;
	import.WriteAngle = PF_WriteAngle;
	import.CreateEffect = SV_CreateEffect;
	import.RemoveEffects = SV_RemoveEffects;
	import.CreateEffectEvent = SV_CreateEffectEvent;
	import.RemoveEffectsEvent = SV_RemoveEffectsEvent;
	import.CreatePersistantEffect = SV_CreatePersistantEffect;
	import.RemovePersistantEffect = SV_RemovePersistantEffect;
	import.TagMalloc = Z_TagMalloc;
	import.TagFree = Z_Free;
	import.FreeTags = Z_FreeTags;
	import.cvar = Cvar_Get;
	import.cvar_set = Cvar_Set;
	import.cvar_forceset = Cvar_ForceSet;
	import.cvar_variablevalue = Cvar_VariableValue;
	import.argc = Cmd_Argc;
	import.argv = Cmd_Argv;
	import.args = Cmd_Args;
	import.AddCommandString = Cbuf_AddText;
	import.DebugGraph = SCR_DebugGraph;
	import.SetAreaPortalState = CM_SetAreaPortalState;
	import.AreasConnected = CM_AreasConnected;
	import.FS_LoadFile = FS_LoadFile;
	import.FS_FreeFile = FS_FreeFile;
	import.FS_Userdir = FS_Userdir;
	import.FS_CreatePath = FS_CreatePath;
	import.Sys_LoadGameDll = Sys_LoadGameDll;
	import.Sys_UnloadGameDll = Sys_UnloadGameDll;
	import.ClearPersistantEffects = SV_ClearPersistantEffects;
	import.Persistant_Effects_Array = &SV_Persistant_Effects_Array;

	Sys_LoadGameDll("gamex86.dll", &game_module_handle, NULL);
	GetGameApi = (game_export_t * (__cdecl*)(game_import_t*))GetProcAddress(game_module_handle, "GetGameAPI");
	ge = (game_export_t *)GetGameApi(&import);

	if (!ge)
		Com_Error (ERR_DROP, "failed to load game DLL");
	if (ge->apiversion != GAME_API_VERSION)
		Com_Error (ERR_DROP, "game is version %i, not %i", ge->apiversion,
		GAME_API_VERSION);

	ge->Init ();
}
