// scriplib.h

#ifndef __CMDLIB__
#include "cmdlib.h"
#endif

#define	MAXTOKEN	1024

extern	char	token[MAXTOKEN];
extern	char	*scriptbuffer,*script_p,*scriptend_p;
extern	int		grabbed;
extern	int		scriptline;
extern	qboolean	endofscript;


void LoadScriptFile (char *filename);
void ParseFromMemory (char *buffer, int size);

qboolean GetScriptToken (qboolean crossline);
void UnGetScriptToken (void);
qboolean ScriptTokenAvailable (void);


