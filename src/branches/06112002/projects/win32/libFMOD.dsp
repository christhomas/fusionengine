# Microsoft Developer Studio Project File - Name="libFMOD" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=libFMOD - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libFMOD.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libFMOD.mak" CFG="libFMOD - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libFMOD - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "libFMOD - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libFMOD - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release\libFMOD"
# PROP Intermediate_Dir "Release\libFMOD"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBFMOD_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBFMOD_EXPORTS" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 libFusion.lib fmod.lib /nologo /dll /machine:I386 /out:"../../bin/libFMOD.dll"
# SUBTRACT LINK32 /profile

!ELSEIF  "$(CFG)" == "libFMOD - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug\libFMOD"
# PROP Intermediate_Dir "Debug\libFMOD"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBFMOD_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBFMOD_EXPORTS" /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 libFusion_d.lib fmod.lib /nologo /dll /debug /machine:I386 /out:"../../bin/libFMOD_d.dll" /pdbtype:sept
# SUBTRACT LINK32 /profile

!ENDIF 

# Begin Target

# Name "libFMOD - Win32 Release"
# Name "libFMOD - Win32 Debug"
# Begin Group "DLL Source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\libFMOD.cpp
# End Source File
# Begin Source File

SOURCE=..\src\libFMOD.h
# End Source File
# End Group
# Begin Group "FMODSound"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\Sound\FMODSound.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\Sound\FMODSound.h
# End Source File
# Begin Source File

SOURCE=V:\Fusion\Include\ISound.h
# End Source File
# End Group
# Begin Group "Soundbuffers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\Sound\FMODModuleBuffer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\Sound\FMODModuleBuffer.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Sound\FMODSampleBuffer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\Sound\FMODSampleBuffer.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Sound\FMODStreamBuffer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\Sound\FMODStreamBuffer.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Sound\ISoundbuffer.h
# End Source File
# End Group
# End Target
# End Project
