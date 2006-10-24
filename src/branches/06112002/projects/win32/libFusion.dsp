# Microsoft Developer Studio Project File - Name="libFusion" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libFusion - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libFusion.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libFusion.mak" CFG="libFusion - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libFusion - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libFusion - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libFusion - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release\libFusion"
# PROP Intermediate_Dir "Release\libFusion"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../lib/libFusion.lib"

!ELSEIF  "$(CFG)" == "libFusion - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug\libFusion"
# PROP Intermediate_Dir "Debug\libFusion"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../lib/libFusion_d.lib"

!ENDIF 

# Begin Target

# Name "libFusion - Win32 Release"
# Name "libFusion - Win32 Debug"
# Begin Group "ModuleDB"

# PROP Default_Filter ""
# Begin Group "Module"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Include\IModule.h
# End Source File
# Begin Source File

SOURCE=..\..\src\arch\Win32\Win32Module.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\arch\Win32\Win32Module.h
# End Source File
# End Group
# Begin Source File

SOURCE=V:\Fusion\Include\IModuleDB.h
# End Source File
# Begin Source File

SOURCE=..\..\src\arch\Win32\Win32ModuleDB.cpp
# End Source File
# Begin Source File

SOURCE=V:\Fusion\Include\arch\Win32\Win32ModuleDB.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\src\Fusion.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\Fusion.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\FusionSubsystem.h
# End Source File
# End Target
# End Project
