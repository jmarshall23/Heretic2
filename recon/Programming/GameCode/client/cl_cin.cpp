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
#include "client.h"
#include "../external/libsmacker/smacker.h"

struct cinematics_t
{
	cinematics_t()
	{
		video = NULL;
	}

	smk video;
	unsigned long frameCount;

	/* microseconds per frame */
	double usf;

	unsigned long width;
	unsigned long height;

	unsigned char	a_t, a_c[7], a_d[7];
	unsigned long	a_r[7];
};

cinematics_t	cin;


/*
==================
SCR_StopCinematic
==================
*/
void SCR_StopCinematic (void)
{	
	SCR_FinishCinematic();
}

/*
====================
SCR_FinishCinematic

Called when either the cinematic completes, or it is aborted
====================
*/
void SCR_FinishCinematic (void)
{
	if (cin.video == NULL)
		return;

	// tell the server to advance to the next map / cinematic
	MSG_WriteByte (&cls.netchan.message, clc_stringcmd);
	SZ_Print (&cls.netchan.message, va("nextserver %i\n", cl.servercount));

	smk_close(cin.video);
	cin.video = NULL;
}

//==========================================================================

/*
==================
SCR_RunCinematic

==================
*/
void SCR_RunCinematic (void)
{
	const unsigned char* palette_data;
	const unsigned char* image_data;

	if (cin.video == NULL)
		return;

	smk_next(cin.video);	

	palette_data = smk_get_palette(cin.video);
	image_data = smk_get_video(cin.video);

	re.DrawCinematic(cin.width, cin.height, (unsigned char *)image_data, (paletteRGB_t*)palette_data, 1.0f);
}

/*
==================
SCR_PlayCinematic

==================
*/
void SCR_PlayCinematic (char *arg)
{
	char fileName[512];
	sprintf(fileName, "base/video/%s", arg);
	cin.video = smk_open_file(fileName, SMK_MODE_DISK);

	if (cin.video == NULL)
	{
		Com_Printf("Failed to open SMK %s\n", fileName);
		return;
	}

	smk_info_all(cin.video, NULL, &cin.frameCount, &cin.usf);
	smk_info_video(cin.video, &cin.width, &cin.height, NULL);
	smk_info_audio(cin.video, &cin.a_t, cin.a_c, cin.a_d, cin.a_r);

	smk_enable_all(cin.video, cin.a_t);
	smk_enable_video(cin.video, true);

	/* process first frame */
	smk_first(cin.video);
}

bool CIN_IsCinematicRunning(void) {
	return cin.video != NULL;
}