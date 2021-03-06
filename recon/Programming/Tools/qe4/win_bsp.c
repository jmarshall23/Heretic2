
#include "qe3.h"


struct ClientS
{
	DWORD					ConnectTime,ProcessTime,StartTime;
	BOOL					TaskRunning;
	HANDLE					OutputReadPipe,OutputWritePipe;
	PROCESS_INFORMATION		pi;
};


static struct ClientS		Client;
static HWND					hwndBsp = 0;

static void SetCancelButton (char *text, ...)
{
	va_list argptr;
	char	buf[32768];
	char	*out;
	HWND	h;

	h=GetDlgItem(hwndBsp, IDCANCEL);

	if (!h)
	{
//		Error("Couldn't find BSP Cancel Button");
		return;
	}

	va_start (argptr,text);
	vsprintf (buf, text,argptr);
	va_end (argptr);

	out = TranslateString (buf);

	SendMessage (h,
		WM_SETTEXT,
		0,
		(LPARAM)out);
}

static void CleanProcess(DWORD exitcode)
{
	CloseHandle(Client.pi.hThread);
	CloseHandle(Client.pi.hProcess);

	CloseHandle(Client.OutputReadPipe);
	CloseHandle(Client.OutputWritePipe);

	Client.TaskRunning = false;
	Client.StartTime = 0;


	SetCancelButton("Continue");
	bsp_process = 0;
	if (exitcode == ERROR_CANCELLED)
	{
		Sys_Printf ("\nBsp process cancelled.\n");
	}
	else
	{
		if (exitcode)
		{
			Sys_Printf ("\nBsp process quit, exitcode: %d.\n",exitcode);
		}
		else
		{
			Sys_Printf ("\nBsp process completed.\n",exitcode);
		}
	}
	Sys_Beep ();
	Pointfile_Check ();
}


static void bsp_Printf (char *text, ...)
{
	va_list argptr;
	char	buf[32768];
	char	*out;
	HWND	h;

	h=GetDlgItem(hwndBsp, IDC_BSPTEXT);

	va_start (argptr,text);
	vsprintf (buf, text,argptr);
	va_end (argptr);

	out = TranslateString (buf);

//	if (!h)//uh oh, no bsp dialog window--just throw it into the console
//	{
//		Error("Couldn't find BSP Output Window");
		Sys_Printf(out);// seein it in the console annoys me, personally
		return;
//	}


//	SendMessage (h,
//		EM_REPLACESEL,
//		0,
//		(LPARAM)out);
}

// this should run only on a separate thread!!
void ReadNFeed_Thread (int i)
{
    char					buffer[1024];
	DWORD					ReadAmount;
	BOOL					Success;
	BOOL					ret;
	DWORD					exitcode;

	int						threadDone = false;

	Sleep (100);	// give output window a chance to show up

	while(!threadDone)  // hmmm...well, this thread may get trapped in the ReadFile call, and stick around until qe exits. 
	{
		if (!bsp_process)
		{
			threadDone = true;
			break;
		}

		ret = GetExitCodeProcess (bsp_process, &exitcode);
		if (!ret)
			Error ("GetExitCodeProcess failed");
		if (exitcode != STILL_ACTIVE)
		{
			Client.TaskRunning = false;
			// should be safe to try & read whatever's left in the pipe--the ReadFile call should fail when thread's parent cleans up after bsp proc

			break;//egad, this bites!  there still might be shit in the pipe
					//but, i gotta doit or the ReadFile will hang.  afaik. --ss
		}

		Success = ReadFile(Client.OutputReadPipe, buffer, sizeof(buffer)-1, &ReadAmount, NULL);
		if (!Success && (GetLastError() == ERROR_BROKEN_PIPE))
		{
			threadDone = true;
	        break;
		}

		if (Success && ReadAmount)
		{// it should be safe to keep looping--since this is a separate thread, the pipe should just break instead of hanging around
			buffer[ReadAmount] = 0;
			// now throw this into a dialog window
			bsp_Printf(buffer);
		}
	}

	ExitThread(0);
}

 
static void bsp_StartTask(char *Task)
{
	DWORD					Mode;
	SECURITY_ATTRIBUTES		saPipe;
	STARTUPINFO				si;
	BOOL					Success;
	char					*CD;
	char					FinalTask[1024],TempTask[1024],ComSpec[1024];
	DWORD					myThreadId;//id for the read'n'feed thread
	int						threadParam;

	threadParam = 1;

	Mode = PIPE_READMODE_BYTE|PIPE_WAIT;

	saPipe.nLength = sizeof(SECURITY_ATTRIBUTES);
	saPipe.lpSecurityDescriptor = NULL;
	saPipe.bInheritHandle = TRUE;

	CreatePipe(&Client.OutputReadPipe,&Client.OutputWritePipe,&saPipe,0);

	SetNamedPipeHandleState(Client.OutputReadPipe,&Mode,NULL,NULL);
	SetNamedPipeHandleState(Client.OutputWritePipe,&Mode,NULL,NULL);


    Success = DuplicateHandle(GetCurrentProcess(),Client.OutputReadPipe,
		GetCurrentProcess(),NULL,0,FALSE,DUPLICATE_SAME_ACCESS);

	if (!Success)
		Error("unable to read from child process!");

	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);

	si.wShowWindow = SW_SHOWMINNOACTIVE;
	si.hStdOutput = Client.OutputWritePipe;
	si.hStdError = Client.OutputWritePipe;
	si.dwFlags = STARTF_USESTDHANDLES|STARTF_USESHOWWINDOW;

	CD = NULL;
 
	if (Task[0] == '\"') 
		strcpy(TempTask,Task+1);
	else 
		strcpy(TempTask,Task);
	if (TempTask[strlen(TempTask)-1] == '\"')
		TempTask[strlen(TempTask)-1] = 0;

	strcpy(FinalTask,TempTask);
	strlwr(FinalTask);
	if (strstr(FinalTask,".bat"))
	{
		GetEnvironmentVariable("comspec",ComSpec,sizeof(ComSpec));
		sprintf(FinalTask,"%s /c %s",ComSpec,TempTask);
	}
	else 
		strcpy(FinalTask,TempTask);

	if (!CreateProcess(NULL,FinalTask,NULL,NULL,TRUE,0,NULL,CD,&si,&Client.pi))
	{
		Error("Could not find bsp task");
	}

//	CloseHandle(OutputWritePipe);

	Client.TaskRunning = true;
	Client.StartTime = Sys_DoubleTime();
	bsp_process = Client.pi.hProcess;

	// now create the thread to handle the bsp task output

	if (!CreateThread( NULL, // pointer to thread security attributes 
					0, // initial thread stack size, in bytes 
					(LPTHREAD_START_ROUTINE) ReadNFeed_Thread, // pointer to thread function--will start up simple read'n'feed loop
					(LPVOID) &threadParam, // argument for new thread 
					0, // creation flags 
					&myThreadId // pointer to returned thread identifier 
					))
	{
		Error("Couldn't Start up Bsp Communication");
	}


}





void bsp_CheckTask(void)
{
	BOOL					ret;
	DWORD					exitcode;

	if (!bsp_process)
		return;

	Client.ProcessTime = Sys_DoubleTime();

	ret = GetExitCodeProcess (bsp_process, &exitcode);
	if (!ret)
		Error ("GetExitCodeProcess failed");

	if (exitcode != STILL_ACTIVE)
	{
		Client.TaskRunning = false;
	}

	if (!Client.TaskRunning)//should be able to figure out whether we finished ok or not
	{
		CleanProcess(exitcode);
	}

}


/*
===================================================

  bsp window

===================================================
*/

BOOL CALLBACK BspDlgProc (
    HWND hwndDlg,	// handle to dialog box
    UINT uMsg,	// message
    WPARAM wParam,	// first message parameter
    LPARAM lParam 	// second message parameter
   )
{
	char					temppath[512];
	char					outputpath[1024];
	STARTUPINFO				si;

	switch (uMsg)
    {
	case WM_INITDIALOG:
		return FALSE;

	case WM_COMMAND: 
		switch (LOWORD(wParam)) 
		{ 
			case IDCANCEL:
				if (!bsp_process)
				{
					DestroyWindow(hwndDlg);
				}
				else // abort bsp process--warning: this is a yucky hard-coded mess.
				{
					// alert! TerminateProcess won't work with ts--will just cancel the ts process, and not the batch process!

					//yee-ha! start up a kill process just like starting up a bsp (no read'n'feed creation cuz it should be there arready)

					// check whether i'm running locally or using the task server
					strcpy (temppath, ValueForKey(g_qeglobals.d_project_entity, "rshcmd"));

					if (temppath[0]==0)// bsping locally, just smash the process and return.
					{
						TerminateProcess(bsp_process, ERROR_CANCELLED);
						return TRUE;
					}

					// clear the current process out of the way so the kill process can take over
					TerminateProcess(bsp_process, ERROR_CANCELLED);
					bsp_process = 0;

					// now create the killer process.

					// let the task server know this is a kill process
					sprintf (outputpath, "%s -kill", temppath);

					memset(&si, 0, sizeof(si));
					si.cb = sizeof(si);

					// OutputWritePipe should be valid from the old bsp_process
					si.wShowWindow = SW_SHOWMINNOACTIVE;
					si.hStdOutput = Client.OutputWritePipe;
					si.hStdError = Client.OutputWritePipe;
					si.dwFlags = STARTF_USESTDHANDLES|STARTF_USESHOWWINDOW;
 
					// actual spawning of killer proc--this will change what's in Client.pi
					if (!CreateProcess(NULL,outputpath,NULL,NULL,TRUE,0,NULL,NULL,&si,&Client.pi))
					{
						Error("Could not kill Bsp process");
					}

					Client.TaskRunning = true;
					Client.StartTime = Sys_DoubleTime();
					bsp_process = Client.pi.hProcess;
				}
				return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}




void bsp_StartBsp (char *command)
{
	char	sys[1024];
	char	batpath[1024];
	char	outputpath[1024];
	char	temppath[512];
	char	name[1024];
	FILE	*hFile;

	SetInspectorMode (W_CONSOLE);

	if (bsp_process)
	{
		Sys_Printf ("BSP is still going...\n");
		return;
	}

	GetTempPath(512, temppath);
	sprintf (outputpath, "%sjunk.txt", temppath);

	strcpy (name, currentmap);
	if (region_active)
	{
		Map_SaveFile (name, false);
		StripExtension (name);
		strcat (name, ".reg");
	}

	Map_SaveFile (name, region_active);


	QE_ExpandBspString (command, sys, name);

	Sys_ClearPrintf ();
	Sys_Printf ("======================================\nRunning bsp command...\n");
	Sys_Printf ("\n%s\n", sys);

	WriteUserLog("BSP: %s on %s", command, name);

	//
	// write qe3bsp.bat
	//
	sprintf (batpath, "%sqe3bsp.bat", temppath);
	hFile = fopen(batpath, "w");
	if (!hFile)
		Error ("Can't write to %s", batpath);
	fprintf (hFile, sys);
	fclose (hFile);

	//
	// write qe3bsp2.bat
	//
	sprintf (batpath, "%sqe3bsp2.bat", temppath);
	hFile = fopen(batpath, "w");
	if (!hFile)
		Error ("Can't write to %s", batpath);
	fprintf (hFile, "%sqe3bsp.bat", temppath);
//	fprintf (hFile, "%sqe3bsp.bat > %s", temppath, outputpath);
	fclose (hFile);

	Pointfile_Delete ();

	if (hwndBsp)
		DestroyWindow(hwndBsp);

	bsp_StartTask(batpath);
// i removed these next two lines and changed the bsp_printf to send bsp output to console
//	hwndBsp=CreateDialog(g_qeglobals.d_hInstance, (char *)IDD_BSP, g_qeglobals.d_hwndMain, BspDlgProc);
//	SetCancelButton("Abort");

//	Sleep (100);	// give the new process a chance to open its window

//	BringWindowToTop( hwndBsp );	// pop us back on top
//	SetFocus (hwndBsp);
}

