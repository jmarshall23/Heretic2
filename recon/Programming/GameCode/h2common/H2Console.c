// H2Console.c
//

#include "../game/q_shared.h"

int *(*dword_10961344)(const char *, ...) = 0x10961344; // weak

H2COMMON_API void Set_Com_Printf(void(*toSet) (char *fmt, ...))
{
	dword_10961344 = *(int(*)(const char *, ...))toSet;
}