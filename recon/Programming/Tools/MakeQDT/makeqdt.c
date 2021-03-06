#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include <windows.h>

FILE *FH;

#define MAX_FILES 2500

typedef enum project_e {
	PROJECT_QUAKE2,
	PROJECT_WOLF,
	PROJECT_HERETIC2,
	PROJECT_MAX
} project_t;

char FileList[MAX_FILES][256],FileSubList[MAX_FILES][256];
char StartSequenceList[MAX_FILES][256];
int NumFiles = 0;
int NumSubFiles = 0;
int NumRemainingFiles;
char Path[256],FileName[256];

BOOL NoModelGen;
BOOL GenWeaponCode;
int StartWeaponIndex;

char CodeName[256] = "frame";

char SaveCommands[100][256];
int NumCommands = 0;

int NumFrames = 0;
int NumSequences = 0;
DWORD TotalSize = 0;

int TotalFrames = 0;
char FrameList[MAX_FILES+50][256];


char ProjectMainDir[PROJECT_MAX][16] = 
{
	"QUAKE2\\",
	"WOLF\\",
	"HERETIC2\\"
};
char ProjectBaseDir[PROJECT_MAX][16] = 
{
	"BASEQ2\\",
	"BASEW\\",
	"BASE\\"
};
int ActiveProject = PROJECT_HERETIC2;
int UseMd2Model = 0;


void RemoveFiles(char *Pat)
{
	char temp[256];
	int counter;

	for(counter=0;counter<NumFiles;counter++)
	{
		strcpy(temp,FileList[counter]);
		strlwr(temp);
		if (strstr(temp,Pat)) FileList[counter][0] = 0;
	}
}

void CreateSubList(char *Pat)
{
	int Length,counter,c2,Position;
	int LookValue,ThisValue;
	char *Pos;

	NumSubFiles = 0;
	Length = strlen(Pat);
	for(counter=0;counter<NumFiles;counter++)
	{
		if (_strnicmp(FileList[counter],Pat,Length) == 0)
		{
			ThisValue = atol(&FileList[counter][Length]);
			Position = -1;
			for(c2=0;c2<NumSubFiles;c2++)
			{
				LookValue = atol(&FileSubList[c2][Length]);
				if (ThisValue < LookValue) 
				{
					Position = c2;
					break;
				}
			}
			if (Position != -1)
			{
				for(c2=NumSubFiles-1;c2 >= Position;c2--)
					strcpy(FileSubList[c2+1],FileSubList[c2]);

				strcpy(FileSubList[Position],FileList[counter]);
				Pos = strchr(FileSubList[Position],'.');
				if (Pos) *Pos = 0;
			}
			else
			{
				strcpy(FileSubList[NumSubFiles],FileList[counter]);
				Pos = strchr(FileSubList[NumSubFiles],'.');
				if (Pos) *Pos = 0;
			}

			NumSubFiles++;
			NumRemainingFiles--;
			FileList[counter][0] = 0;
		}
	}
}

void CreateExtSubList(char *Pat, BOOL Reset)
{
	int Length,counter,c2,Position;
	char *Pos;

	if (Reset)
	{
		NumSubFiles = 0;
	}
	Length = strlen(Pat);
	for(counter=0;counter<NumFiles;counter++)
	{
		Pos = strchr(FileList[counter],'.');
		if (Pos && _strnicmp(Pos+1,Pat,Length) == 0)
		{
			Position = -1;
			if (Position != -1)
			{
				for(c2=NumSubFiles-1;c2 >= Position;c2--)
					strcpy(FileSubList[c2+1],FileSubList[c2]);

				strcpy(FileSubList[Position],FileList[counter]);
				
			}
			else
				strcpy(FileSubList[NumSubFiles],FileList[counter]);

			NumSubFiles++;
			NumRemainingFiles--;
			FileList[counter][0] = 0;
		}
	}
}

void DoDirectory(void)
{
	WIN32_FIND_DATA filedata;
	HANDLE handle;
	BOOL retval;
	char temp[256];
	int counter;

	sprintf(temp,"%s*.*",Path);
	
	handle = FindFirstFile(temp,&filedata);
	retval = TRUE;

	while (handle != INVALID_HANDLE_VALUE && retval)
	{
		if (filedata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			// Ignore directories until later
		}
		else
		{
			strcpy(temp,filedata.cFileName);
			strlwr(temp);
			if (strstr(temp,".tri") ||
			    strstr(temp,".hrc") ||
			    strstr(temp,".asc") ||
			    strstr(temp,".htr") ||
			    strstr(temp,".lbm") ||
			    strstr(temp,".tga") ||
			    strstr(temp,".pcx"))
			{	
				strcpy(FileList[NumFiles],filedata.cFileName);
//				Pos = strchr(FileList[NumFiles],'.');
//				if (Pos) *Pos = 0;

				for(counter=0;counter<NumFiles;counter++)
					if (strcmpi(FileList[counter],FileList[NumFiles]) == 0) break;

				if (counter >= NumFiles)
				{
					NumFiles++;
					TotalSize += filedata.nFileSizeLow;
				}
			}
		}

		retval = FindNextFile(handle,&filedata);
	}

	if (handle != INVALID_HANDLE_VALUE)
		FindClose(handle);

	NumRemainingFiles = NumFiles;
}

void CreateFrames(void)
{
	int counter,c2,Length;
	char temp[256];

	for(counter=0;counter<NumFiles;counter++)
	{
		if (FileList[counter][0])
		{
			StartSequenceList[NumSequences][0] = 0;
			NumSequences++;

			Length = strlen(FileList[counter]);
			// they might have numbers at the beginning
			if (Length <= 3) c2 = 0;
			else c2 = 2;

			for(;c2<Length;c2++)
				if (isdigit(FileList[counter][c2]))
					break;

			strcpy(temp,FileList[counter]);
			if (c2 < Length)
			{
				temp[c2] = 0;
			}
			CreateSubList(temp);

			if (NumSubFiles)
				strcpy(StartSequenceList[NumSequences-1],FileSubList[0]);

			fprintf(FH,"//\n");
			for(c2=0;c2<NumSubFiles;c2++)
			{
				if (c2 % 5 == 0)
				{
					if (c2) fprintf(FH,"\n");
					if (UseMd2Model)
					{
						fprintf(FH,"$frame ");
					}
					else
					{
						fprintf(FH,"$fm_frame ");
					}
				}
				fprintf(FH,"%-12s ",FileSubList[c2]);
				strcpy(FrameList[TotalFrames],FileSubList[c2]);
				TotalFrames++;
			}
			fprintf(FH,"\n\n");
			NumFrames += NumSubFiles;

			FrameList[TotalFrames][0] = 0;
			TotalFrames++;
		}
	}

	TotalFrames--;
}

void GenerateFrameCode(void)
{
	int counter,c2,NextIndex,FirstIndex,FrameIndex;
	char space[100];

	memset(space,' ',sizeof(space));
	space[strlen(CodeName)] = 0;

	fprintf(FH,"\n\n\n// Frame Code\n");
	c2 = 0;
	FirstIndex = 0;
	FrameIndex = StartWeaponIndex-1;
	for(counter=0;counter<TotalFrames;counter++)
	{
		if (!FrameList[counter+1][0]) 
		{
			NextIndex = FirstIndex;
		}
		else if (!FrameList[counter][0])
		{
			c2 = 0;
			FirstIndex = counter+1;
			fprintf(FH,"\n");
			continue;
		}
		else 
		{
			c2++;
			NextIndex = counter+1;
		}

		FrameIndex++;
		if (GenWeaponCode)
		{
			if (NextIndex == FirstIndex)
				fprintf(FH,"void() %s_%-10s = [ $%-10s, %-10s %s ] { self.weaponframe = %d; };\n",
					CodeName,FrameList[counter],FrameList[counter],"player_run",space,FrameIndex);
			else
				fprintf(FH,"void() %s_%-10s = [ $%-10s, %s_%-10s ] { self.weaponframe = %d; };\n",
					CodeName,FrameList[counter],FrameList[counter],CodeName,FrameList[NextIndex],FrameIndex);
		}
		else
		{
			fprintf(FH,"void() %s_%-10s = [ $%-10s, %s_%-10s ] {  };\n",
				CodeName,FrameList[counter],FrameList[counter],CodeName,FrameList[NextIndex]);
		}
	}

	fprintf(FH,"\n\n\n\n");
}
















void PERR(BOOL bSuccess, char * api) 
{
    DWORD       dwError;
    TCHAR       szMsgBuf[500];

    if (!(bSuccess)) 
    {
        dwError = GetLastError();
        printf("\nFile:%s\nError occured in %s on line %d\n", __FILE__, api, __LINE__);

	    FormatMessage(
	        FORMAT_MESSAGE_FROM_SYSTEM,
	        NULL,                               // ignored
	        dwError,                     // message id
	        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),   // message language
	        szMsgBuf,                   // address of buffer pointer
	        500,                                  // minimum buffer size
	        NULL );                              // no other arguments

        printf("\nError:(%ld)%s ", dwError, szMsgBuf);
     }
}
		

void RunCommand(char *Name)
{
	BOOL bSuccess;  /* BOOL return code for APIs */
	STARTUPINFO si;  /* for CreateProcess call */
	PROCESS_INFORMATION pi;  /* for CreateProcess call */

	GetStartupInfo(&si);

	bSuccess = CreateProcess(NULL,  /* filename */
		Name,  /* full command line for child */
		NULL,  /* process security descriptor */
		NULL,  /* thread security descriptor */
		TRUE,  /* inherit handles? Also use if STARTF_USESTDHANDLES */
		0,  /* creation flags */
		NULL,  /* inherited environment address */
		NULL,  /* startup dir; NULL = start in current */
		&si,  /* pointer to startup info (input) */
		&pi);  /* pointer to process info (output) */
	PERR(bSuccess, "CreateProcess");

	CloseHandle(pi.hThread);

	WaitForSingleObject(pi.hProcess,INFINITE);
	CloseHandle(pi.hProcess);
}


void DoNormalCode(char *Name)
{
	fprintf(FH,"/*QUAKED %s (1 0 0) (0 0 0) (50 50 50)\n",Name);
	fprintf(FH,"New item for QuakeEd\n");
	fprintf(FH,"\n");
	fprintf(FH,"-------------------------FIELDS-------------------------\n");
	fprintf(FH,"--------------------------------------------------------\n");
	fprintf(FH,"\n");
	fprintf(FH,"*/\n");
	fprintf(FH,"void() %s =\n",Name);
	fprintf(FH,"{\n");
	fprintf(FH,"   if (deathmatch)\n");
	fprintf(FH,"   {\n");
	fprintf(FH,"      remove(self);\n");
	fprintf(FH,"      return;\n");
	fprintf(FH,"   }\n");
	fprintf(FH,"\n");
	fprintf(FH,"   precache_model2 (\"models/%s.mdl\");\n",Name);
	fprintf(FH,"   self.solid = SOLID_SLIDEBOX;\n");
	fprintf(FH,"   self.movetype = MOVETYPE_NONE;\n");
	fprintf(FH,"\n");
	fprintf(FH,"   setmodel (self, \"models/%s.mdl\");\n",Name);
	fprintf(FH,"   self.skin = 0;\n");
	fprintf(FH,"\n");
	fprintf(FH,"   setsize (self, '0 0 0', '50 50 50');\n");
	fprintf(FH,"   self.health = 100;\n");
	fprintf(FH,"};\n");
}

void DoWeaponCode(char *Name)
{
	int counter;

	fprintf(FH,"\n");
	fprintf(FH,"\nvoid () FireTestWeapon =\n");
	fprintf(FH,"{\n");

	fprintf(FH,"   dprint(\"Testing Sequence \");\n");
	fprintf(FH,"   dprint(ftos(self.cnt));\n");
	fprintf(FH,"   dprint(\"\\n\");\n");
	fprintf(FH,"\n");
	for(counter=0;counter<NumSequences;counter++)
	{
		if (!counter) fprintf(FH,"   if (self.cnt == %d)\n",counter);
		else fprintf(FH,"   else if (self.cnt == %d)\n",counter);
		fprintf(FH,"      %s_%s();\n",CodeName,StartSequenceList[counter]);
	}

	fprintf(FH,"\n");
	fprintf(FH,"   self.cnt = self.cnt + 1;\n");
	fprintf(FH,"   if (self.cnt >= %d)\n",NumSequences);
	fprintf(FH,"      self.cnt = 0;\n");
	fprintf(FH,"};\n");

}

void ReadOldQDT(char *Name)
{
	FILE *FH;
	char temp[256];
	int start;

	FH = fopen(Name,"r");
	if (!FH) return;

	while(!feof(FH))
	{
		fgets(temp,sizeof(temp),FH);
		if (feof(FH)) continue;

		temp[strlen(temp)-1] = 0; // get rid of the lf

		start = 0;
		while (temp[start] != 0 && temp[start] < 33)
			start++;

		if (temp[start] != '$') continue;

		if (_strnicmp(&temp[start],"$fm_frame",9) == 0) break;
		if (_strnicmp(&temp[start],"$frame",6) == 0) break;

		if (_strnicmp(&temp[start],"$fm_skin",8) == 0) continue;
		if (_strnicmp(&temp[start],"$skin",5) == 0) continue;
		if (_strnicmp(&temp[start],"$fm_base",8) == 0) continue;
		if (_strnicmp(&temp[start],"$fm_stbase",10) == 0) continue;
		if (_strnicmp(&temp[start],"$base",5) == 0) continue;
		if (_strnicmp(&temp[start],"$fm_cd",6) == 0) continue;
		if (_strnicmp(&temp[start],"$cd",3) == 0) continue;

		strcpy(SaveCommands[NumCommands],temp);
		NumCommands++;
	}

	fclose(FH);
}

int FindCommand(char *Which)
{
	int counter,start,length;

	length = 0;
	while(Which[length] != 0 && Which[length] > 32)
		length++;

	for(counter=0;counter<NumCommands;counter++)
	{
		start = 0;
		while (SaveCommands[counter][start] != 0 && SaveCommands[counter][start] < 33)
			start++;

		if (_strnicmp(&SaveCommands[counter][start],Which,length) == 0)
			return counter;
	}

	return -1;
}










#define PATHSEPERATOR   '/'

void DefaultExtension (char *path, char *extension)
{
	char    *src;
//
// if path doesn't have a .EXT, append extension
// (extension should include the .)
//
	src = path + strlen(path) - 1;

	while (*src != PATHSEPERATOR && src != path)
	{
		if (*src == '.')
			return;                 // it has an extension
		src--;
	}

	strcat (path, extension);
}




#define COMMAND_LINE "makeqdt [-nogen] [-md2] [-fm] <model.qdt>\n"

int main(int argc, char **argv)
{
	char Buffer[MAX_PATH],*FilePtr,*Pos,BaseName[64],temp[MAX_PATH],CDPath[MAX_PATH], NewFile[MAX_PATH],STBaseName[64];
	int counter,Position;

	GetTempPath(sizeof(Buffer),Buffer);

	NoModelGen = GenWeaponCode = FALSE;
	StartWeaponIndex = 1;

	printf("makeqdt V1.2 (%s %s) by RJ!\n\n",__DATE__,__TIME__);
	if (argc < 2)
	{
		printf(COMMAND_LINE);
		return 0;
	}

	counter = 1;
	while(counter < argc)
	{
		if (strcmpi(argv[counter],"-q2") == 0)
			ActiveProject = PROJECT_QUAKE2;
		else if (strcmpi(argv[counter],"-wolf") == 0)
			ActiveProject = PROJECT_WOLF;
		else if (strcmpi(argv[counter],"-ht2") == 0)
			ActiveProject = PROJECT_HERETIC2;
		else if (strcmpi(argv[counter],"-md2") == 0)
			UseMd2Model = TRUE;
		else if (strcmpi(argv[counter],"-fm") == 0)
			UseMd2Model = FALSE;
		else if (strcmpi(argv[counter],"-nogen") == 0) 
			NoModelGen = TRUE;
		else if (strcmpi(argv[counter],"-weapon") == 0) 
			GenWeaponCode = TRUE;
		else if (strcmpi(argv[counter],"-index") == 0) 
		{
		   counter++;
			if (counter < argc) StartWeaponIndex = atoi(argv[counter]);
		}
		else if (strcmpi(argv[counter],"-name") == 0) 
		{
			counter++;
			if (counter < argc) strcpy(CodeName,argv[counter]);
		}
		else
		{
			strcpy(temp,argv[counter]);
			DefaultExtension(temp,".qdt");
      		GetFullPathName(temp,sizeof(Buffer),Buffer,&FilePtr);
			strcpy(NewFile,Buffer);
			strlwr(NewFile);
			if (strstr(NewFile,ProjectBaseDir[ActiveProject]))
			{
				*(strstr(NewFile,ProjectBaseDir[ActiveProject])+6) = 0;
				strcat(NewFile,"scripts\\");
				strcat(NewFile,temp);
			}
		}

		counter++;
	}

	if (Buffer[0] == 0) 
	{
		printf(COMMAND_LINE);
		return 0;
	}

	strcpy(FileName,FilePtr);
	*FilePtr = 0;
	strcpy(Path,Buffer);
	*FilePtr = FileName[0];

	printf("Path: %s\n",Path);
	printf("QDT: %s\n",FileName);

	DoDirectory();

	FH = fopen(NewFile,"r");
	if (FH)
	{
		fclose(FH);
		printf("File exists: %s\n",FileName);
		printf("Do you want to delete this? (Y/N) ");
		counter = getche();
		printf("\n");
		if (counter != 'y' && counter != 'Y') 
			return 0;
		printf("\n");
		ReadOldQDT(NewFile);
	}

	FH = fopen(NewFile,"w");
	if (!FH) 
	{
		printf("Could not create %s\n",Buffer);
		return 0;
	}

	*(FilePtr-1) = 0;
	strcpy(temp, Buffer);
	_strlwr(temp);
	if (strstr(temp,"models"))
	{
		strcpy(CDPath, strstr(temp,"models")+7);
	}
	else
		strcpy(CDPath, temp);

	if (UseMd2Model)
	{
		fprintf(FH,"$cd %s\n",CDPath);
		Position = FindCommand("$origin");
	}
	else
	{
		fprintf(FH,"$fm_cd %s\n",CDPath);
		Position = FindCommand("$fm_origin");
	}

	if (Position >= 0) 
	{
		fprintf(FH,"%s\n",SaveCommands[Position]);
		SaveCommands[Position][0] = 0;
	}
	else
	{
		if (UseMd2Model)
		{
			fprintf(FH,"$origin 0 0 0\n");
		}
		else
		{
			fprintf(FH,"$fm_origin 0 0 0\n");
		}
	}

	RemoveFiles("bin");
	RemoveFiles("!");
	CreateSubList("base");
	if (!NumSubFiles)
	{
		fclose(FH);
		printf("No base* file!\n");
		return 1;
	}
	else if (NumSubFiles > 1)
	{
		fclose(FH);
		printf("More than one base* files!\n");
		return 1;
	}
	strcpy(BaseName,FileSubList[0]);

	CreateSubList("stbase");
	STBaseName[0]=0;
	if (NumSubFiles > 1)
	{
		printf("More than one STbase* files!\n");
	}
	else if (NumSubFiles)
	{
		strcpy(STBaseName,FileSubList[0]);
	}

	CreateExtSubList("pcx", TRUE);
	CreateExtSubList("tga", FALSE);
	if (!NumSubFiles)
	{
		fclose(FH);
		printf("No .pcx or .tga files!\n");
		return 2;
	}

	temp[0] = 0;	//find the first skin for the base cmd
	for(counter=0;counter<NumSubFiles;counter++)
	{
//		if (FileSubList[counter][0] == '!')
//			continue;

		strcpy(temp,FileSubList[counter]);
		break;
	}

	if (UseMd2Model)
	{
		if (STBaseName[0])
		{
			printf("Md2 commands don't support STBase files!\n");
			return -1;
		}
		fprintf(FH,"$base %s %s\n",BaseName,temp);
	}
	else
	{
		if (STBaseName[0])
			fprintf(FH,"$fm_basest %s %s %s\n",BaseName,temp,STBaseName);
		else
			fprintf(FH,"$fm_base %s %s\n",BaseName,temp);
	}

//	RemoveFiles("base");	

	printf("Number of skin pages: %d\n",NumSubFiles);
	for(counter=0;counter<NumSubFiles;counter++)
	{
//		if (FileSubList[counter][0] == '!')
//			continue;

//		if (strchr(FileSubList[counter],'.'))
//			*strchr(FileSubList[counter],'.') = 0;
		if (UseMd2Model)
		{
			fprintf(FH,"$skin %s\n",FileSubList[counter]);
		}
		else
		{
			fprintf(FH,"$fm_skin %s\n",FileSubList[counter]);
		}
	}
//	RemoveFiles(".pcx");

	if (UseMd2Model)
	{
		Position = FindCommand("$scale");
	}
	else
	{
		Position = FindCommand("$fm_scale");
	}

	if (Position >= 0) 
	{
		fprintf(FH,"%s\n",SaveCommands[Position]);
		SaveCommands[Position][0] = 0;
	}
	else
	{
		if (UseMd2Model)
		{
			fprintf(FH,"$scale 1\n");
		}
		else
		{
			fprintf(FH,"$fm_scale 1\n");
		}
	}
	for(counter=0;counter<NumCommands;counter++)
	{
		if (SaveCommands[counter][0])
			fprintf(FH,"%s\n",SaveCommands[counter]);
	}
	fprintf(FH,"\n");

	CreateFrames();
	//GenerateFrameCode();

	printf("Number of animation sequences: %d\n",NumSequences);
	printf("Total number of animation frames: %d\n",NumFrames);
//	printf("Approximate model size: %u\n",TotalSize);

	Pos = strchr(FilePtr,'.');
	if (Pos) *Pos = 0;

/*	if (GenWeaponCode)
	{
		DoWeaponCode(FilePtr);
	}
	else
	{
		DoNormalCode(FilePtr);
	}
*/

	fclose(FH);

	if (!NoModelGen)
	{
	   sprintf(temp,"i:\\%sbin_nt\\qdata %s",ProjectMainDir[ActiveProject],NewFile);
	   printf("\nExecuting '%s'...\n\n",temp);
	   RunCommand(temp);
	}

#ifdef _DEBUG
	getch();
#endif

	return 0;
}

