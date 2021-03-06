# Microsoft Developer Studio Project File - Name="game" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=game - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "game.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "game.mak" CFG="game - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "game - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "game - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "game - Win32 Final" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "game - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir "."
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir "."
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MT /W3 /Zi /O2 /I "..\qcommon" /I "..\game" /I "..\Player" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_HERETIC2_" /D "_DEVEL" /D "_GAME_DLL" /FR /Fp"" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 winmm.lib kernel32.lib user32.lib gdi32.lib H2Common.lib quake2.lib /nologo /base:"0x10F50000" /version:1.0 /subsystem:windows /dll /profile /debug /machine:I386 /out:"..\Release/gamex86.dll" /libpath:"../Release"

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir "."
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir "."
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /Gi /Zi /Od /I "..\qcommon" /I "..\game" /I "..\Player" /D "_DEBUG" /D "BUILDING_REF_GL" /D "WIN32" /D "_WINDOWS" /D "_HERETIC2_" /D "_DEVEL" /D "_GAME_DLL" /FR /Fp"" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 winmm.lib kernel32.lib user32.lib gdi32.lib H2Common.lib quake2.lib /nologo /base:"0x10F50000" /version:1.0 /subsystem:windows /dll /debug /machine:I386 /out:"..\Debug/gamex86.dll" /libpath:"../Debug"
# SUBTRACT LINK32 /incremental:no

!ELSEIF  "$(CFG)" == "game - Win32 Final"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Final"
# PROP BASE Intermediate_Dir "Final"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Final"
# PROP Intermediate_Dir ".\Final"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /Zi /O2 /I "..\qcommon" /I "..\game" /I "..\Player" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_HERETIC2_" /D "_DEVEL" /D "_GAME_DLL" /FR /Fp"" /YX /FD /c
# ADD CPP /nologo /MT /W3 /O2 /I "..\qcommon" /I "..\game" /I "..\Player" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_HERETIC2_" /D "_GAME_DLL" /Fr /Fp"" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib kernel32.lib user32.lib gdi32.lib H2Common.lib quake2.lib /nologo /version:1.0 /subsystem:windows /dll /profile /debug /machine:I386 /out:"..\Release/gamex86.dll" /libpath:"../Release"
# ADD LINK32 winmm.lib kernel32.lib user32.lib gdi32.lib H2Common.lib quake2.lib /nologo /base:"0x10F50000" /version:1.0 /subsystem:windows /dll /machine:I386 /out:"..\base/gamex86.dll" /libpath:"../Final" /release
# SUBTRACT LINK32 /profile /debug

!ENDIF 

# Begin Target

# Name "game - Win32 Release"
# Name "game - Win32 Debug"
# Name "game - Win32 Final"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\buoy.c
# End Source File
# Begin Source File

SOURCE=.\c_ai.c
# End Source File
# Begin Source File

SOURCE=.\c_corvus1.c
# End Source File
# Begin Source File

SOURCE=.\c_corvus1_anim.c
# End Source File
# Begin Source File

SOURCE=.\c_corvus2.c
# End Source File
# Begin Source File

SOURCE=.\c_corvus2_anim.c
# End Source File
# Begin Source File

SOURCE=.\c_corvus3.c
# End Source File
# Begin Source File

SOURCE=.\c_corvus3_anim.c
# End Source File
# Begin Source File

SOURCE=.\c_corvus4.c
# End Source File
# Begin Source File

SOURCE=.\c_corvus4_anim.c
# End Source File
# Begin Source File

SOURCE=.\c_corvus5.c
# End Source File
# Begin Source File

SOURCE=.\c_corvus5_anim.c
# End Source File
# Begin Source File

SOURCE=.\c_corvus6.c
# End Source File
# Begin Source File

SOURCE=.\c_corvus6_anim.c
# End Source File
# Begin Source File

SOURCE=.\c_corvus7.c
# End Source File
# Begin Source File

SOURCE=.\c_corvus7_anim.c
# End Source File
# Begin Source File

SOURCE=.\c_corvus8.c
# End Source File
# Begin Source File

SOURCE=.\c_corvus8_anim.c
# End Source File
# Begin Source File

SOURCE=.\c_corvus9.c
# End Source File
# Begin Source File

SOURCE=.\c_corvus9_anim.c
# End Source File
# Begin Source File

SOURCE=.\c_dranor.c
# End Source File
# Begin Source File

SOURCE=.\c_dranor_anim.c
# End Source File
# Begin Source File

SOURCE=.\c_elflord.c
# End Source File
# Begin Source File

SOURCE=.\c_elflord_anim.c
# End Source File
# Begin Source File

SOURCE=.\c_morcalavin.c
# End Source File
# Begin Source File

SOURCE=.\c_morcalavin_anim.c
# End Source File
# Begin Source File

SOURCE=.\c_priestess.c
# End Source File
# Begin Source File

SOURCE=.\c_priestess2.c
# End Source File
# Begin Source File

SOURCE=.\c_priestess2_anim.c
# End Source File
# Begin Source File

SOURCE=.\c_priestess_anim.c
# End Source File
# Begin Source File

SOURCE=.\c_siernan1.c
# End Source File
# Begin Source File

SOURCE=.\c_siernan1_anim.c
# End Source File
# Begin Source File

SOURCE=.\c_siernan2.c
# End Source File
# Begin Source File

SOURCE=.\c_siernan2_anim.c
# End Source File
# Begin Source File

SOURCE=.\c_ssithrascout.c
# End Source File
# Begin Source File

SOURCE=.\c_ssithrascout_anim.c
# End Source File
# Begin Source File

SOURCE=.\c_tome.c
# End Source File
# Begin Source File

SOURCE=.\c_tome_anim.c
# End Source File
# Begin Source File

SOURCE=.\c_victimSsithra.c
# End Source File
# Begin Source File

SOURCE=.\c_victimSsithra_anim.c
# End Source File
# Begin Source File

SOURCE=.\decals.c
# End Source File
# Begin Source File

SOURCE=.\decals.h
# End Source File
# Begin Source File

SOURCE=.\ds.cpp
# End Source File
# Begin Source File

SOURCE=.\g_ai.c
# End Source File
# Begin Source File

SOURCE=.\g_breakable.c
# End Source File
# Begin Source File

SOURCE=.\g_ClassStatics.c
# End Source File
# Begin Source File

SOURCE=.\g_cmds.c
# End Source File
# Begin Source File

SOURCE=.\g_combat.c
# End Source File
# Begin Source File

SOURCE=.\g_DefaultMessageHandler.c
# End Source File
# Begin Source File

SOURCE=.\g_env.c
# End Source File
# Begin Source File

SOURCE=.\g_field.c
# End Source File
# Begin Source File

SOURCE=.\g_flamethrow.c
# End Source File
# Begin Source File

SOURCE=.\g_func.c
# End Source File
# Begin Source File

SOURCE=.\g_HitLocation.c
# End Source File
# Begin Source File

SOURCE=.\g_items.c
# End Source File
# Begin Source File

SOURCE=.\g_light.c
# End Source File
# Begin Source File

SOURCE=.\g_main.c
# End Source File
# Begin Source File

SOURCE=.\g_Message.c
# End Source File
# Begin Source File

SOURCE=.\g_misc.c
# End Source File
# Begin Source File

SOURCE=.\g_monster.c
# End Source File
# Begin Source File

SOURCE=.\g_obj.c
# End Source File
# Begin Source File

SOURCE=.\g_phys.c
# End Source File
# Begin Source File

SOURCE=.\g_Physics.c
# End Source File
# Begin Source File

SOURCE=.\g_ResourceManagers.c
# End Source File
# Begin Source File

SOURCE=.\g_rope.c
# End Source File
# Begin Source File

SOURCE=.\g_save.c
# End Source File
# Begin Source File

SOURCE=.\g_shrine.c
# End Source File
# Begin Source File

SOURCE=.\g_Skeletons.c
# End Source File
# Begin Source File

SOURCE=.\g_spawn.c
# End Source File
# Begin Source File

SOURCE=.\g_spawnf.c
# End Source File
# Begin Source File

SOURCE=.\g_svcmds.c
# End Source File
# Begin Source File

SOURCE=.\g_target.c
# End Source File
# Begin Source File

SOURCE=.\g_trigger.c
# End Source File
# Begin Source File

SOURCE=.\g_utils.c
# End Source File
# Begin Source File

SOURCE=.\g_waterfx.c
# End Source File
# Begin Source File

SOURCE=.\g_weapon.c
# End Source File
# Begin Source File

SOURCE=.\m_assassin.c
# End Source File
# Begin Source File

SOURCE=.\m_assassin_anim.c
# End Source File
# Begin Source File

SOURCE=.\m_beast.c
# End Source File
# Begin Source File

SOURCE=.\m_beast_anim.c
# End Source File
# Begin Source File

SOURCE=.\m_bee.c
# End Source File
# Begin Source File

SOURCE=.\m_chicken.c

!IF  "$(CFG)" == "game - Win32 Release"

# ADD CPP /O2 /Yc""

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

# ADD CPP /Yc

!ELSEIF  "$(CFG)" == "game - Win32 Final"

# ADD BASE CPP /O2 /Yc""
# ADD CPP /O2 /Yc""

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\m_chicken_anim.c
# ADD CPP /Yc
# End Source File
# Begin Source File

SOURCE=.\m_elflord.c

!IF  "$(CFG)" == "game - Win32 Release"

# ADD CPP /O2 /Yc""

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

# ADD CPP /Yc

!ELSEIF  "$(CFG)" == "game - Win32 Final"

# ADD BASE CPP /O2 /Yc""
# ADD CPP /O2 /Yc""

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\m_elflord_anims.c
# ADD CPP /Yc
# End Source File
# Begin Source File

SOURCE=.\m_fish.c

!IF  "$(CFG)" == "game - Win32 Release"

# ADD CPP /O2 /Yc""

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

# ADD CPP /Yc

!ELSEIF  "$(CFG)" == "game - Win32 Final"

# ADD BASE CPP /O2 /Yc""
# ADD CPP /O2 /Yc""

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\m_fish_anim.c
# ADD CPP /Yc
# End Source File
# Begin Source File

SOURCE=.\m_FMtest.c
# End Source File
# Begin Source File

SOURCE=.\m_gkrokon.c

!IF  "$(CFG)" == "game - Win32 Release"

# ADD CPP /O2 /Yc""

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

# ADD CPP /Yc

!ELSEIF  "$(CFG)" == "game - Win32 Final"

# ADD BASE CPP /O2 /Yc""
# ADD CPP /O2 /Yc""

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\m_gkrokon_anim.c
# ADD CPP /Yc
# End Source File
# Begin Source File

SOURCE=.\m_gorgon.c

!IF  "$(CFG)" == "game - Win32 Release"

# ADD CPP /O2 /Yc""

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

# ADD CPP /Yc

!ELSEIF  "$(CFG)" == "game - Win32 Final"

# ADD BASE CPP /O2 /Yc""
# ADD CPP /O2 /Yc""

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\m_gorgon_anim.c
# ADD CPP /Yc
# End Source File
# Begin Source File

SOURCE=.\m_harpy.c

!IF  "$(CFG)" == "game - Win32 Release"

# ADD CPP /O2 /Yc""

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

# ADD CPP /Yc

!ELSEIF  "$(CFG)" == "game - Win32 Final"

# ADD BASE CPP /O2 /Yc""
# ADD CPP /O2 /Yc""

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\m_harpy_anim.c
# ADD CPP /Yc
# End Source File
# Begin Source File

SOURCE=.\m_imp.c
# End Source File
# Begin Source File

SOURCE=.\m_imp_anim.c
# End Source File
# Begin Source File

SOURCE=.\m_morcalavin.c
# End Source File
# Begin Source File

SOURCE=.\m_morcalavin_anim.c
# End Source File
# Begin Source File

SOURCE=.\m_mother.c
# End Source File
# Begin Source File

SOURCE=.\m_mother_anim.c
# End Source File
# Begin Source File

SOURCE=.\m_move.c
# End Source File
# Begin Source File

SOURCE=.\m_mssithra.c
# End Source File
# Begin Source File

SOURCE=.\m_mssithra_anim.c
# End Source File
# Begin Source File

SOURCE=.\m_ogle.c
# End Source File
# Begin Source File

SOURCE=.\m_ogle_anim.c
# End Source File
# Begin Source File

SOURCE=.\m_plagueElf.c

!IF  "$(CFG)" == "game - Win32 Release"

# ADD CPP /O2 /Yc""

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

# ADD CPP /Yc

!ELSEIF  "$(CFG)" == "game - Win32 Final"

# ADD BASE CPP /O2 /Yc""
# ADD CPP /O2 /Yc""

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\m_plagueElf_anim.c
# ADD CPP /Yc
# End Source File
# Begin Source File

SOURCE=.\m_plagueSsithra.c

!IF  "$(CFG)" == "game - Win32 Release"

# ADD CPP /O2 /Yc""

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

# ADD CPP /Yc

!ELSEIF  "$(CFG)" == "game - Win32 Final"

# ADD BASE CPP /O2 /Yc""
# ADD CPP /O2 /Yc""

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\m_plagueSsithra_anim.c
# ADD CPP /Yc
# End Source File
# Begin Source File

SOURCE=.\m_priestess.c
# End Source File
# Begin Source File

SOURCE=.\m_priestess_anim.c
# End Source File
# Begin Source File

SOURCE=.\m_rat.c

!IF  "$(CFG)" == "game - Win32 Release"

# ADD CPP /O2 /Yc""

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

# ADD CPP /Yc

!ELSEIF  "$(CFG)" == "game - Win32 Final"

# ADD BASE CPP /O2 /Yc""
# ADD CPP /O2 /Yc""

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\m_rat_anim.c
# ADD CPP /Yc
# End Source File
# Begin Source File

SOURCE=.\m_seraph.c
# End Source File
# Begin Source File

SOURCE=.\m_seraph_anim.c
# End Source File
# Begin Source File

SOURCE=.\m_seraph_guard.c
# End Source File
# Begin Source File

SOURCE=.\m_seraph_guard_anim.c
# End Source File
# Begin Source File

SOURCE=.\m_spreader.c

!IF  "$(CFG)" == "game - Win32 Release"

# ADD CPP /O2 /Yc""

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

# ADD CPP /Yc

!ELSEIF  "$(CFG)" == "game - Win32 Final"

# ADD BASE CPP /O2 /Yc""
# ADD CPP /O2 /Yc""

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\m_spreader_anim.c
# ADD CPP /Yc
# End Source File
# Begin Source File

SOURCE=.\m_spreadermist.c
# End Source File
# Begin Source File

SOURCE=.\m_stats.c
# End Source File
# Begin Source File

SOURCE=.\m_tcheckrik.c
# End Source File
# Begin Source File

SOURCE=.\m_tcheckrik_anim.c
# End Source File
# Begin Source File

SOURCE=.\m_tcheckrik_spells.c
# End Source File
# Begin Source File

SOURCE=..\qcommon\Message.c
# End Source File
# Begin Source File

SOURCE=.\mg_ai.c
# End Source File
# Begin Source File

SOURCE=.\mg_guide.c
# End Source File
# Begin Source File

SOURCE=.\p_client.c
# End Source File
# Begin Source File

SOURCE=..\qcommon\p_dll.c
# End Source File
# Begin Source File

SOURCE=.\p_funcs.c
# End Source File
# Begin Source File

SOURCE=.\p_hud.c
# End Source File
# Begin Source File

SOURCE=.\p_item.c
# End Source File
# Begin Source File

SOURCE=.\p_view.c
# End Source File
# Begin Source File

SOURCE=.\Script.c
# End Source File
# Begin Source File

SOURCE=..\qcommon\Skeletons.c
# End Source File
# Begin Source File

SOURCE=.\spl_blast.c
# End Source File
# Begin Source File

SOURCE=.\spl_BlueRing.c
# End Source File
# Begin Source File

SOURCE=.\spl_flyingfist.c
# End Source File
# Begin Source File

SOURCE=.\spl_HellStaff.c
# End Source File
# Begin Source File

SOURCE=.\spl_maceballs.c
# End Source File
# Begin Source File

SOURCE=.\spl_magicmissile.c
# End Source File
# Begin Source File

SOURCE=.\spl_meteorbarrier.c
# End Source File
# Begin Source File

SOURCE=.\spl_morph.c
# End Source File
# Begin Source File

SOURCE=.\spl_Phoenix.c
# End Source File
# Begin Source File

SOURCE=.\spl_powerup.c
# End Source File
# Begin Source File

SOURCE=.\spl_RedRain.c
# End Source File
# Begin Source File

SOURCE=.\spl_ripper.c
# End Source File
# Begin Source File

SOURCE=.\spl_shield.c
# End Source File
# Begin Source File

SOURCE=.\spl_sphereofannihlation.c
# End Source File
# Begin Source File

SOURCE=.\spl_teleport.c
# End Source File
# Begin Source File

SOURCE=.\spl_tornado.c
# End Source File
# Begin Source File

SOURCE=.\spl_wall.c
# End Source File
# Begin Source File

SOURCE=.\Utilities.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=..\qcommon\Angles.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\ArrayedList.h
# End Source File
# Begin Source File

SOURCE=.\buoy.h
# End Source File
# Begin Source File

SOURCE=.\c_ai.h
# End Source File
# Begin Source File

SOURCE=.\c_corvus1.h
# End Source File
# Begin Source File

SOURCE=.\c_corvus1_anim.h
# End Source File
# Begin Source File

SOURCE=.\c_corvus2.h
# End Source File
# Begin Source File

SOURCE=.\c_corvus2_anim.h
# End Source File
# Begin Source File

SOURCE=.\c_corvus3.h
# End Source File
# Begin Source File

SOURCE=.\c_corvus3_anim.h
# End Source File
# Begin Source File

SOURCE=.\c_corvus4.h
# End Source File
# Begin Source File

SOURCE=.\c_corvus4_anim.h
# End Source File
# Begin Source File

SOURCE=.\c_corvus5.h
# End Source File
# Begin Source File

SOURCE=.\c_corvus5_anim.h
# End Source File
# Begin Source File

SOURCE=.\c_corvus6.h
# End Source File
# Begin Source File

SOURCE=.\c_corvus6_anim.h
# End Source File
# Begin Source File

SOURCE=.\c_corvus7.h
# End Source File
# Begin Source File

SOURCE=.\c_corvus7_anim.h
# End Source File
# Begin Source File

SOURCE=.\c_corvus8.h
# End Source File
# Begin Source File

SOURCE=.\c_corvus8_anim.h
# End Source File
# Begin Source File

SOURCE=.\c_corvus9.h
# End Source File
# Begin Source File

SOURCE=.\c_corvus9_anim.h
# End Source File
# Begin Source File

SOURCE=.\c_Dranor.h
# End Source File
# Begin Source File

SOURCE=.\c_dranor_anim.h
# End Source File
# Begin Source File

SOURCE=.\c_elflord.h
# End Source File
# Begin Source File

SOURCE=.\c_elflord_anim.h
# End Source File
# Begin Source File

SOURCE=.\c_morcalavin.h
# End Source File
# Begin Source File

SOURCE=.\c_morcalavin_anim.h
# End Source File
# Begin Source File

SOURCE=.\c_priestess.h
# End Source File
# Begin Source File

SOURCE=.\c_priestess2.h
# End Source File
# Begin Source File

SOURCE=.\c_priestess2_anim.h
# End Source File
# Begin Source File

SOURCE=.\c_priestess_anim.h
# End Source File
# Begin Source File

SOURCE=.\c_siernan1.h
# End Source File
# Begin Source File

SOURCE=.\c_siernan1_anim.h
# End Source File
# Begin Source File

SOURCE=.\c_siernan2.h
# End Source File
# Begin Source File

SOURCE=.\c_siernan2_anim.h
# End Source File
# Begin Source File

SOURCE=.\c_ssithrascout.h
# End Source File
# Begin Source File

SOURCE=.\c_ssithrascout_anim.h
# End Source File
# Begin Source File

SOURCE=.\c_tome.h
# End Source File
# Begin Source File

SOURCE=.\c_tome_anim.h
# End Source File
# Begin Source File

SOURCE=.\c_victimSsithra.h
# End Source File
# Begin Source File

SOURCE=.\c_victimSsithra_anim.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\cl_strings.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\compfmod.h
# End Source File
# Begin Source File

SOURCE=.\ds.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\EffectFlags.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\FX.h
# End Source File
# Begin Source File

SOURCE=.\g_BoundingForm.h
# End Source File
# Begin Source File

SOURCE=.\g_ClassStatics.h
# End Source File
# Begin Source File

SOURCE=.\g_DefaultMessageHandler.h
# End Source File
# Begin Source File

SOURCE=.\g_Edict.h
# End Source File
# Begin Source File

SOURCE=.\g_HitLocation.h
# End Source File
# Begin Source File

SOURCE=.\g_items.h
# End Source File
# Begin Source File

SOURCE=.\g_itemstats.h
# End Source File
# Begin Source File

SOURCE=.\g_local.h
# End Source File
# Begin Source File

SOURCE=.\g_Message.h
# End Source File
# Begin Source File

SOURCE=.\g_misc.h
# End Source File
# Begin Source File

SOURCE=.\g_monster.h
# End Source File
# Begin Source File

SOURCE=.\g_NewSystem.h
# End Source File
# Begin Source File

SOURCE=.\g_Physics.h
# End Source File
# Begin Source File

SOURCE=.\g_playstats.h
# End Source File
# Begin Source File

SOURCE=.\g_Skeleton.h
# End Source File
# Begin Source File

SOURCE=.\g_Skeletons.h
# End Source File
# Begin Source File

SOURCE=.\g_teleport.h
# End Source File
# Begin Source File

SOURCE=.\g_Typedef.h
# End Source File
# Begin Source File

SOURCE=.\g_volume_effect.h
# End Source File
# Begin Source File

SOURCE=.\g_weapon.h
# End Source File
# Begin Source File

SOURCE=.\game.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\GenericUnions.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\H2Common.h
# End Source File
# Begin Source File

SOURCE=.\ICScript.h
# End Source File
# Begin Source File

SOURCE=.\list.h
# End Source File
# Begin Source File

SOURCE=.\m_actor.h
# End Source File
# Begin Source File

SOURCE=.\m_assassin.h
# End Source File
# Begin Source File

SOURCE=.\m_assassin_anim.h
# End Source File
# Begin Source File

SOURCE=.\m_beast.h
# End Source File
# Begin Source File

SOURCE=.\m_beast_anim.h
# End Source File
# Begin Source File

SOURCE=.\m_chicken.h
# End Source File
# Begin Source File

SOURCE=.\m_chicken_anim.h
# End Source File
# Begin Source File

SOURCE=.\m_elf.h
# End Source File
# Begin Source File

SOURCE=.\m_elflord.h
# End Source File
# Begin Source File

SOURCE=.\m_elflord_anims.h
# End Source File
# Begin Source File

SOURCE=.\m_fish.h
# End Source File
# Begin Source File

SOURCE=.\m_fish_anim.h
# End Source File
# Begin Source File

SOURCE=.\m_gkrokon.h
# End Source File
# Begin Source File

SOURCE=.\m_gkrokon_anim.h
# End Source File
# Begin Source File

SOURCE=.\m_gorgon.h
# End Source File
# Begin Source File

SOURCE=.\m_gorgon_anim.h
# End Source File
# Begin Source File

SOURCE=.\m_harpy.h
# End Source File
# Begin Source File

SOURCE=.\m_harpy_anim.h
# End Source File
# Begin Source File

SOURCE=.\m_imp.h
# End Source File
# Begin Source File

SOURCE=.\m_imp_anim.h
# End Source File
# Begin Source File

SOURCE=.\m_morcalavin.h
# End Source File
# Begin Source File

SOURCE=.\m_morcalavin_anim.h
# End Source File
# Begin Source File

SOURCE=.\m_mother.h
# End Source File
# Begin Source File

SOURCE=.\m_mother_anim.h
# End Source File
# Begin Source File

SOURCE=.\m_mssithra.h
# End Source File
# Begin Source File

SOURCE=.\m_mssithra_anim.h
# End Source File
# Begin Source File

SOURCE=.\m_ogle.h
# End Source File
# Begin Source File

SOURCE=.\m_ogle_anim.h
# End Source File
# Begin Source File

SOURCE=.\m_plagueElf.h
# End Source File
# Begin Source File

SOURCE=.\m_plagueElf_anim.h
# End Source File
# Begin Source File

SOURCE=.\m_plagueSsithra.h
# End Source File
# Begin Source File

SOURCE=.\m_plagueSsithra_anim.h
# End Source File
# Begin Source File

SOURCE=.\m_player.h
# End Source File
# Begin Source File

SOURCE=.\m_priestess.h
# End Source File
# Begin Source File

SOURCE=.\m_priestess_anim.h
# End Source File
# Begin Source File

SOURCE=.\m_rat.h
# End Source File
# Begin Source File

SOURCE=.\m_rat_anim.h
# End Source File
# Begin Source File

SOURCE=.\m_seraph.h
# End Source File
# Begin Source File

SOURCE=.\m_seraph_anim.h
# End Source File
# Begin Source File

SOURCE=.\m_seraph_guard.h
# End Source File
# Begin Source File

SOURCE=.\m_seraph_guard_anim.h
# End Source File
# Begin Source File

SOURCE=.\m_spreader.h
# End Source File
# Begin Source File

SOURCE=.\m_spreader_anim.h
# End Source File
# Begin Source File

SOURCE=.\m_spreaderbomb_anim.h
# End Source File
# Begin Source File

SOURCE=.\m_spreadermist.h
# End Source File
# Begin Source File

SOURCE=.\m_stats.h
# End Source File
# Begin Source File

SOURCE=.\m_tcheckrik.h
# End Source File
# Begin Source File

SOURCE=.\m_tcheckrik_anim.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\Matrix.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\Message.h
# End Source File
# Begin Source File

SOURCE=.\mg_guide.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\p_actions2.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\p_anim_branch2.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\p_anim_data2.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\p_animactor.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\p_anims2.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\p_chicken.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\p_ctrl2.h
# End Source File
# Begin Source File

SOURCE=.\p_funcs.h
# End Source File
# Begin Source File

SOURCE=.\p_item.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\p_main2.h
# End Source File
# Begin Source File

SOURCE=..\Player\P_NewMove.h
# End Source File
# Begin Source File

SOURCE=.\pcode.h
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

SOURCE=.\q_shared.h
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

SOURCE=..\qcommon\Reference.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\ResourceManager.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\SinglyLinkedList.h
# End Source File
# Begin Source File

SOURCE=..\qcommon\Skeletons.h
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

SOURCE=.\waypoint.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\game.def
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
