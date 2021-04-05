// cl_string.cpp
//

#include "client.h"

#define MAX_MESSAGESTRINGS 1000
typedef struct
{
	char* string;
	char* wav;
} cl_message_t;

char* cl_messagebuf = NULL;
char* cl_lvlmessagebuf = NULL;
cvar_t* cl_filegamemsg;
cvar_t* cl_filelevelmsg;

cl_message_t cl_levelmsgs[MAX_MESSAGESTRINGS];
cl_message_t cL_gamemsgs[MAX_MESSAGESTRINGS];

/*
==================
CL_LoadTextFile
==================
*/
int CL_LoadTextFile(char* name, char** addr)
{
	int		length;
	char* buffer;

	length = FS_LoadFile(name, (void**)&buffer);
	if (length <= 0)
	{
		Sys_Error("Unable to load %s", name);
		return(0);
	}
	*addr = (char*)malloc(length + 1);
	memcpy(*addr, buffer, length);
	*(*addr + length) = 0;
	FS_FreeFile(buffer);

	return(length + 1);
}

/*
==================
SetupStrings
==================
*/
void SetupStrings(char *buffer, int length, cl_message_t *message_text)
{
	int		i;
	char* p, * startp, * return_p;

	startp = buffer;
	p = 0;
	for (i = 1; p < (buffer + length); ++i)
	{
		if (i > MAX_MESSAGESTRINGS)
		{
			Com_Printf("Too many strings\n");
			return;
		}

		// Read in string up to return
		return_p = strchr(startp, 13);	// Search for return characters 13 10
		if (!return_p)	// At end of file
		{
			break;
		}
		else
			*return_p = 0;

		// Search string for #
		p = strchr(startp, '#');	// Search for # which signifies a wav file
		if ((p) && (p < return_p))
		{
			*p = 0;
			message_text[i].wav = ++p;	// Save stuff after #
		}

		// Save stuff before #
		message_text[i].string = startp;

		do
		{
			p = strchr(startp, '@');	// Search for # which signifies a wav file
			if (p)
				*p = '\n';
		} while (p);

		return_p += 2;	// Hop over 13 10 
		startp = return_p;	// Advance to next string

	}
}

/*
==================
CL_LoadStrings
==================
*/
void CL_LoadStrings(void)
{
	int len;

	cl_filegamemsg = Cvar_Get("file_gamemsg", "gamemsg.txt", 1);
	len = CL_LoadTextFile(cl_filegamemsg->string, &cl_messagebuf);
	SetupStrings(cl_messagebuf, len, &cL_gamemsgs[0]);

	cl_filelevelmsg = Cvar_Get("file_levelmsg", "levelmsg.txt", 1);
	len = CL_LoadTextFile(cl_filelevelmsg->string, &cl_lvlmessagebuf);
	SetupStrings(cl_lvlmessagebuf, len, &cl_levelmsgs[0]);
}

/*
====================
CL_GetGameString
====================
*/
char* CL_GetGameString(int i) {
	return cL_gamemsgs[i].string;
}

/*
================
CL_GetGameWav
================
*/
char* CL_GetGameWav(int i) {
	return cL_gamemsgs[i].wav;
}

/*
================
CL_GetLevelString
================
*/
char* CL_GetLevelString(int i) {
	return cl_levelmsgs[i].string;
}

/*
================
CL_GetLevelWav
================
*/
char* CL_GetLevelWav(int i) {
	return cl_levelmsgs[i].wav;
}