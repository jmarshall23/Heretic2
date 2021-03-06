# Microsoft Developer Studio Project File - Name="Player" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Player - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Player.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Player.mak" CFG="Player - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Player - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Player - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Player - Win32 Final" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Player - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Release"
# PROP Intermediate_Dir ".\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /Zi /O2 /I "../qcommon" /I "../game" /I "../client" /I "../server" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "PLAYER_DLL" /D "_DEVEL" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib H2Common.lib /nologo /base:"0x110d0000" /version:1.0 /subsystem:windows /dll /profile /debug /machine:I386 /libpath:"..\Release"

!ELSEIF  "$(CFG)" == "Player - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /Gi /ZI /Od /I "../qcommon" /I "../game" /I "../client" /I "../server" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "PLAYER_DLL" /D "_DEVEL" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib H2Common.lib /nologo /base:"0x110d0000" /version:1.0 /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept /libpath:"..\Debug"

!ELSEIF  "$(CFG)" == "Player - Win32 Final"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Player___Win32_Final"
# PROP BASE Intermediate_Dir "Player___Win32_Final"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Final"
# PROP Intermediate_Dir ".\Final"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /Zi /O2 /I "./qcommon" /I "./game" /I "./client" /I "./server" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "PLAYER_DLL" /FR".\Release/" /YX /FD /c
# ADD CPP /nologo /MT /W3 /O2 /I "../qcommon" /I "../game" /I "../client" /I "../server" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "PLAYER_DLL" /Fr /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib H2Common.lib quake2.lib /nologo /version:1.0 /subsystem:windows /dll /profile /debug /machine:I386 /libpath:"..\Release"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:"0x110d0000" /version:1.0 /subsystem:windows /dll /machine:I386 /out:"..\base/Player.dll" /libpath:"..\Final" /release
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Player - Win32 Release"
# Name "Player - Win32 Debug"
# Name "Player - Win32 Final"
# Begin Group "Source Files"

# PROP Default_Filter "*.c *.cpp"
# Begin Source File

SOURCE=.\main.c
# End Source File
# Begin Source File

SOURCE=.\p_actions.c
# End Source File
# Begin Source File

SOURCE=.\p_anim_branch.c
# End Source File
# Begin Source File

SOURCE=.\p_anim_data.c
# End Source File
# Begin Source File

SOURCE=.\p_animactor.c
# End Source File
# Begin Source File

SOURCE=.\p_anims.c
# End Source File
# Begin Source File

SOURCE=.\p_chicken.c
# End Source File
# Begin Source File

SOURCE=.\p_chicken_anim.c
# End Source File
# Begin Source File

SOURCE=.\p_ctrl.c
# End Source File
# Begin Source File

SOURCE=.\p_items.c
# End Source File
# Begin Source File

SOURCE=.\p_main.c
# End Source File
# Begin Source File

SOURCE=.\p_weapon.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=..\client\client.h
# End Source File
# Begin Source File

SOURCE=.\p_actions.h
# End Source File
# Begin Source File

SOURCE=.\p_anim_branch.h
# End Source File
# Begin Source File

SOURCE=.\p_anim_data.h
# End Source File
# Begin Source File

SOURCE=.\p_animactor.h
# End Source File
# Begin Source File

SOURCE=.\p_anims.h
# End Source File
# Begin Source File

SOURCE=.\p_chicken.h
# End Source File
# Begin Source File

SOURCE=.\p_ctrl.h
# End Source File
# Begin Source File

SOURCE=.\p_items.h
# End Source File
# Begin Source File

SOURCE=.\p_main.h
# End Source File
# Begin Source File

SOURCE=.\P_NewMove.h
# End Source File
# Begin Source File

SOURCE=.\p_types.h
# End Source File
# Begin Source File

SOURCE=.\p_weapon.h
# End Source File
# Begin Source File

SOURCE=.\Player.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\Final\H2Common.lib
# End Source File
# Begin Source File

SOURCE=..\Final\quake2.lib
# End Source File
# End Target
# End Project
