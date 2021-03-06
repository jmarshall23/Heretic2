# Microsoft Developer Studio Project File - Name="Client Effects" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Client Effects - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Client Effects.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Client Effects.mak" CFG="Client Effects - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Client Effects - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Client Effects - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Client Effects - Win32 Final" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Client Effects - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /Zi /O2 /I "../client" /I "../qcommon" /I "../game" /I "../player" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_HERETIC2_" /D "_DEVEL" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib H2Common.lib quake2.lib /nologo /base:"0x110e0000" /version:1.0 /subsystem:windows /dll /profile /debug /machine:I386 /libpath:"../Release"

!ELSEIF  "$(CFG)" == "Client Effects - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /Gi /Zi /Od /I "../client" /I "../qcommon" /I "../game" /I "../player" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_HERETIC2_" /D "_DEVEL" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib H2Common.lib quake2.lib /nologo /base:"0x110e0000" /version:1.0 /subsystem:windows /dll /debug /machine:I386 /libpath:"../Debug"
# SUBTRACT LINK32 /pdb:none /incremental:no /map

!ELSEIF  "$(CFG)" == "Client Effects - Win32 Final"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Client_Effects___Win32_Final"
# PROP BASE Intermediate_Dir "Client_Effects___Win32_Final"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Final"
# PROP Intermediate_Dir ".\Final"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /Zi /O2 /I "../client" /I "../qcommon" /I "../game" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_HERETIC2_" /FR /YX /FD /c
# ADD CPP /nologo /MT /W3 /O2 /I "../client" /I "../qcommon" /I "../game" /I "../player" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_HERETIC2_" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib H2Common.lib quake2.lib /nologo /version:1.0 /subsystem:windows /dll /profile /debug /machine:I386 /libpath:"../Release"
# ADD LINK32 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib H2Common.lib /nologo /base:"0x11110000" /version:1.0 /subsystem:windows /dll /machine:I386 /def:".\Client Effects.def" /out:"..\base/Client Effects.dll" /libpath:"../Final" /release
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Client Effects - Win32 Release"
# Name "Client Effects - Win32 Debug"
# Name "Client Effects - Win32 Final"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c"
# Begin Source File

SOURCE=".\Ambient Effects.c"
# End Source File
# Begin Source File

SOURCE=.\ce_DefaultMessageHandler.c
# End Source File
# Begin Source File

SOURCE=.\ce_DLight.c
# End Source File
# Begin Source File

SOURCE=.\ce_Message.c
# End Source File
# Begin Source File

SOURCE=".\Client Effects.c"
# End Source File
# Begin Source File

SOURCE=".\Client Entities.c"
# End Source File
# Begin Source File

SOURCE=.\fx_AmmoPickup.c
# End Source File
# Begin Source File

SOURCE=.\fx_animate.c
# End Source File
# Begin Source File

SOURCE=.\fx_assassin.c
# End Source File
# Begin Source File

SOURCE=.\fx_blood.c
# End Source File
# Begin Source File

SOURCE=.\fx_BlueRing.c
# End Source File
# Begin Source File

SOURCE=.\fx_bubbler.c
# End Source File
# Begin Source File

SOURCE=.\fx_crosshair.c
# End Source File
# Begin Source File

SOURCE=.\fx_cwatcher.c
# End Source File
# Begin Source File

SOURCE=.\fx_debris.c
# End Source File
# Begin Source File

SOURCE=.\fx_DefensePickup.c
# End Source File
# Begin Source File

SOURCE=.\fx_Dripper.c
# End Source File
# Begin Source File

SOURCE=.\fx_dust.c
# End Source File
# Begin Source File

SOURCE=.\fx_dustpuff.c
# End Source File
# Begin Source File

SOURCE=.\fx_fire.c
# End Source File
# Begin Source File

SOURCE=.\fx_firehands.c
# End Source File
# Begin Source File

SOURCE=.\fx_flamethrow.c
# End Source File
# Begin Source File

SOURCE=.\fx_flyingfist.c
# End Source File
# Begin Source File

SOURCE=.\fx_Fountain.c
# End Source File
# Begin Source File

SOURCE=.\fx_halo.c
# End Source File
# Begin Source File

SOURCE=.\fx_HealthPickup.c
# End Source File
# Begin Source File

SOURCE=.\fx_HellStaff.c
# End Source File
# Begin Source File

SOURCE=.\fx_hitpuff.c
# End Source File
# Begin Source File

SOURCE=.\fx_hpproj.c
# End Source File
# Begin Source File

SOURCE=.\fx_insectstaff.c
# End Source File
# Begin Source File

SOURCE=.\fx_lensflare.c
# End Source File
# Begin Source File

SOURCE=.\fx_lightning.c
# End Source File
# Begin Source File

SOURCE=.\fx_Maceball.c
# End Source File
# Begin Source File

SOURCE=.\fx_magicmissile.c
# End Source File
# Begin Source File

SOURCE=.\fx_meteorbarrier.c
# End Source File
# Begin Source File

SOURCE=.\fx_mist.c
# End Source File
# Begin Source File

SOURCE=.\fx_mork.c
# End Source File
# Begin Source File

SOURCE=.\fx_Morph.c
# End Source File
# Begin Source File

SOURCE=.\fx_objects.c
# End Source File
# Begin Source File

SOURCE=.\fx_pespell.c
# End Source File
# Begin Source File

SOURCE=.\fx_Phoenix.c
# End Source File
# Begin Source File

SOURCE=.\fx_pickup.c
# End Source File
# Begin Source File

SOURCE=.\fx_pickuppuzzle.c
# End Source File
# Begin Source File

SOURCE=.\fx_PlagueMist.c
# End Source File
# Begin Source File

SOURCE=.\fx_PlagueMistExplode.c
# End Source File
# Begin Source File

SOURCE=.\fx_portal.c
# End Source File
# Begin Source File

SOURCE=.\fx_quake.c
# End Source File
# Begin Source File

SOURCE=.\fx_RedRain.c
# End Source File
# Begin Source File

SOURCE=.\fx_redrainglow.c
# End Source File
# Begin Source File

SOURCE=.\fx_remotecamera.c
# End Source File
# Begin Source File

SOURCE=.\fx_Ripples.c
# End Source File
# Begin Source File

SOURCE=.\fx_rope.c
# End Source File
# Begin Source File

SOURCE=.\fx_scorchmark.c
# End Source File
# Begin Source File

SOURCE=.\fx_shadow.c
# End Source File
# Begin Source File

SOURCE=.\fx_Shield.c
# End Source File
# Begin Source File

SOURCE=.\fx_shrine.c
# End Source File
# Begin Source File

SOURCE=.\fx_smoke.c
# End Source File
# Begin Source File

SOURCE=.\fx_sound.c
# End Source File
# Begin Source File

SOURCE=.\fx_sparks.c
# End Source File
# Begin Source File

SOURCE=.\fx_spellchange.c
# End Source File
# Begin Source File

SOURCE=.\fx_spellhands.c
# End Source File
# Begin Source File

SOURCE=.\fx_sphereofannihlation.c
# End Source File
# Begin Source File

SOURCE=.\fx_spoo.c
# End Source File
# Begin Source File

SOURCE=.\fx_ssarrow.c
# End Source File
# Begin Source File

SOURCE=.\fx_ssithra.c
# End Source File
# Begin Source File

SOURCE=.\fx_staff.c
# End Source File
# Begin Source File

SOURCE=.\fx_tbeast.c
# End Source File
# Begin Source File

SOURCE=.\fx_teleport.c
# End Source File
# Begin Source File

SOURCE=.\fx_tome.c
# End Source File
# Begin Source File

SOURCE=.\fx_tornado.c
# End Source File
# Begin Source File

SOURCE=.\fx_Wall.c
# End Source File
# Begin Source File

SOURCE=.\fx_waterentrysplash.c
# End Source File
# Begin Source File

SOURCE=.\fx_waterwake.c
# End Source File
# Begin Source File

SOURCE=.\fx_WeaponPickup.c
# End Source File
# Begin Source File

SOURCE=".\Generic Character Effects.c"
# End Source File
# Begin Source File

SOURCE=".\Generic Weapon Effects.c"
# End Source File
# Begin Source File

SOURCE=".\Item Effects.c"
# End Source File
# Begin Source File

SOURCE=.\LevelMaps.c
# End Source File
# Begin Source File

SOURCE=.\LightStyles.c
# End Source File
# Begin Source File

SOURCE=.\Main.c
# End Source File
# Begin Source File

SOURCE=..\qcommon\Message.c
# End Source File
# Begin Source File

SOURCE="..\qcommon\NetMsg Read.c"
# End Source File
# Begin Source File

SOURCE=.\Particle.c
# End Source File
# Begin Source File

SOURCE=".\Player Effects.c"
# End Source File
# Begin Source File

SOURCE=.\TestEffect.c
# End Source File
# Begin Source File

SOURCE=.\Utilities.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=".\ambient effects.h"
# End Source File
# Begin Source File

SOURCE=..\qcommon\Angles.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\anorms.h
# End Source File
# Begin Source File

SOURCE=..\client\cdaudio.h
# End Source File
# Begin Source File

SOURCE=.\ce_DefaultMessageHandler.h
# End Source File
# Begin Source File

SOURCE=.\ce_DLight.h
# End Source File
# Begin Source File

SOURCE=.\ce_Message.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\cl_strings.h
# End Source File
# Begin Source File

SOURCE=".\Client Effects.h"
# End Source File
# Begin Source File

SOURCE=".\Client Entities.h"
# End Source File
# Begin Source File

SOURCE=..\client\client.h
# End Source File
# Begin Source File

SOURCE=..\client\console.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\EffectFlags.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\FX.h
# End Source File
# Begin Source File

SOURCE=.\fx_debris.h
# End Source File
# Begin Source File

SOURCE=.\fx_dustpuff.h
# End Source File
# Begin Source File

SOURCE=..\game\g_items.h
# End Source File
# Begin Source File

SOURCE=..\game\g_itemstats.h
# End Source File
# Begin Source File

SOURCE=..\game\g_PhysicsInfo.h
# End Source File
# Begin Source File

SOURCE=..\game\g_playstats.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\GenericUnions.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\H2Common.h
# End Source File
# Begin Source File

SOURCE=..\client\input.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\items.h
# End Source File
# Begin Source File

SOURCE=..\client\keys.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\LevelMaps.h
# End Source File
# Begin Source File

SOURCE=..\game\m_elf.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\Matrix.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\Message.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\motion.h
# End Source File
# Begin Source File

SOURCE=.\Particle.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\Placement.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\PrimitiveDisplayHack.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\q_ClientServer.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\q_Physics.h
# End Source File
# Begin Source File

SOURCE=..\game\q_shared.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\q_Sprite.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\q_Surface.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\q_Typedef.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\qcommon.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\qfiles.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\random.h
# End Source File
# Begin Source File

SOURCE=..\client\ref.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\Reference.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\ResourceManager.h
# End Source File
# Begin Source File

SOURCE=..\client\screen.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\SinglyLinkedList.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\Skeletons.h
# End Source File
# Begin Source File

SOURCE=..\client\sound.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\timing.h
# End Source File
# Begin Source File

SOURCE=.\Utilities.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\Vector.h
# End Source File
# Begin Source File

SOURCE=..\client\vid.h
# End Source File
# End Group
# Begin Group "Resources"

# PROP Default_Filter "def"
# Begin Source File

SOURCE=".\Client Effects.def"

!IF  "$(CFG)" == "Client Effects - Win32 Release"

!ELSEIF  "$(CFG)" == "Client Effects - Win32 Debug"

!ELSEIF  "$(CFG)" == "Client Effects - Win32 Final"

# PROP Exclude_From_Build 1

!ENDIF 

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
