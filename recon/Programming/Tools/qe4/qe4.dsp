# Microsoft Developer Studio Project File - Name="qe3" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=qe3 - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "qe4.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "qe4.mak" CFG="qe3 - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "qe3 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "qe3 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "qe3 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /Zi /O2 /I "..\common" /I "..\..\gamecode\qcommon" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "WIN_ERROR" /Fr /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 comctl32.lib opengl32.lib glu32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386

!ELSEIF  "$(CFG)" == "qe3 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\common" /I "..\..\gamecode\qcommon" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "WIN_ERROR" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 comctl32.lib opengl32.lib glu32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /profile /map /debug /machine:I386

!ENDIF 

# Begin Target

# Name "qe3 - Win32 Release"
# Name "qe3 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\brush.c
# End Source File
# Begin Source File

SOURCE=.\brush.h
# End Source File
# Begin Source File

SOURCE=.\camera.c
# End Source File
# Begin Source File

SOURCE=.\clip.c
# End Source File
# Begin Source File

SOURCE=..\common\cmdlib.c
# End Source File
# Begin Source File

SOURCE=.\csg.c
# End Source File
# Begin Source File

SOURCE=.\drag.c
# End Source File
# Begin Source File

SOURCE=.\eclass.c
# End Source File
# Begin Source File

SOURCE=.\entity.c
# End Source File
# Begin Source File

SOURCE=..\common\lbmlib.c
# End Source File
# Begin Source File

SOURCE=.\map.c
# End Source File
# Begin Source File

SOURCE=..\common\mathlib.c
# End Source File
# Begin Source File

SOURCE=.\mru.c
# End Source File
# Begin Source File

SOURCE=.\parse.c
# End Source File
# Begin Source File

SOURCE=.\points.c
# End Source File
# Begin Source File

SOURCE=.\qe3.c
# End Source File
# Begin Source File

SOURCE=.\qe3.h
# End Source File
# Begin Source File

SOURCE=..\common\qfiles.c
# End Source File
# Begin Source File

SOURCE=..\common\scriplib.c
# End Source File
# Begin Source File

SOURCE=.\select.c
# End Source File
# Begin Source File

SOURCE=.\textmod.c
# End Source File
# Begin Source File

SOURCE=.\textures.c
# End Source File
# Begin Source File

SOURCE=.\tower.c
# End Source File
# Begin Source File

SOURCE=.\undo.c
# End Source File
# Begin Source File

SOURCE=.\vertsel.c
# End Source File
# Begin Source File

SOURCE=.\win_bsp.c
# End Source File
# Begin Source File

SOURCE=.\win_cam.c
# End Source File
# Begin Source File

SOURCE=.\win_dlg.c
# End Source File
# Begin Source File

SOURCE=.\win_ent.c
# End Source File
# Begin Source File

SOURCE=.\win_main.c
# End Source File
# Begin Source File

SOURCE=.\win_qe3.c
# End Source File
# Begin Source File

SOURCE=.\win_xy.c
# End Source File
# Begin Source File

SOURCE=.\win_z.c
# End Source File
# Begin Source File

SOURCE=.\xy.c
# End Source File
# Begin Source File

SOURCE=.\z.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\camera.h
# End Source File
# Begin Source File

SOURCE=.\clip.h
# End Source File
# Begin Source File

SOURCE=..\common\cmdlib.h
# End Source File
# Begin Source File

SOURCE=.\entity.h
# End Source File
# Begin Source File

SOURCE=.\entityw.h
# End Source File
# Begin Source File

SOURCE=.\glingr.h
# End Source File
# Begin Source File

SOURCE=..\common\lbmlib.h
# End Source File
# Begin Source File

SOURCE=.\map.h
# End Source File
# Begin Source File

SOURCE=..\common\mathlib.h
# End Source File
# Begin Source File

SOURCE=.\mru.h
# End Source File
# Begin Source File

SOURCE=.\parse.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\q_typedef.h
# End Source File
# Begin Source File

SOURCE=.\qedefs.h
# End Source File
# Begin Source File

SOURCE=..\common\qfiles.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=..\common\scriplib.h
# End Source File
# Begin Source File

SOURCE=.\select.h
# End Source File
# Begin Source File

SOURCE=.\textures.h
# End Source File
# Begin Source File

SOURCE=.\tower.h
# End Source File
# Begin Source File

SOURCE=.\undo.h
# End Source File
# Begin Source File

SOURCE=.\win_bsp.h
# End Source File
# Begin Source File

SOURCE=.\xy.h
# End Source File
# Begin Source File

SOURCE=.\z.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\q.bmp
# End Source File
# Begin Source File

SOURCE=.\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\win_qe3.rc
# End Source File
# End Group
# End Target
# End Project
