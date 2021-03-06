#include "qfiles.h"
#include "scriplib.h"
#include <windows.h>

materialtype_t	defaultmaterialtypes[] =
{
	{"gravel",	MATERIAL_GRAVEL},
	{"metal",	MATERIAL_METAL},
	{"stone",	MATERIAL_STONE},
	{"wood",	MATERIAL_WOOD},
	{NULL,		0}
};

materialtype_t	*materialtypes;

void QFile_ReadMaterialTypes(char* filename)
{
	int		i;
	FILE	*f;

	f = fopen (filename, "rb");
	if (!f)
	{
		materialtypes = defaultmaterialtypes;
		return;
	}
	fclose (f);

	free(materialtypes);
	materialtypes = (materialtype_t*)malloc(256 * sizeof(materialtype_t));

	LoadScriptFile(filename);
	i = 0;
	
	while (i < 255)
	{
		GetScriptToken (true);
		if (endofscript)
		{
			break;
		}
		if (strcmp(token, "material") != 0)
		{
			while (ScriptTokenAvailable())
			{
				GetScriptToken(false);
			}
		}
		else
		{
			GetScriptToken(false);
			materialtypes[i].name = (char*)malloc(strlen(token) + 1);
			strcpy(materialtypes[i].name, token);
			GetScriptToken (false);
			materialtypes[i].value = atoi(token);
		}
		i++;
	}
	materialtypes[i].name = NULL;
	materialtypes[i].value = 0;
}
