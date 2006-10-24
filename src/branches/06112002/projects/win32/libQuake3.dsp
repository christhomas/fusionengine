# Microsoft Developer Studio Project File - Name="libQuake3" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=libQuake3 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libQuake3.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libQuake3.mak" CFG="libQuake3 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libQuake3 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "libQuake3 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libQuake3 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release\libQuake3"
# PROP Intermediate_Dir "Release\libQuake3"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBQUAKE3_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBQUAKE3_EXPORTS" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 libjpeg.lib /nologo /dll /machine:I386 /out:"../../bin/libQuake3.dll"
# SUBTRACT LINK32 /profile

!ELSEIF  "$(CFG)" == "libQuake3 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug\libQuake3"
# PROP Intermediate_Dir "Debug\libQuake3"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBQUAKE3_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBQUAKE3_EXPORTS" /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 libjpeg_d.lib /nologo /dll /debug /machine:I386 /out:"../../bin/libQuake3_d.dll" /pdbtype:sept
# SUBTRACT LINK32 /profile

!ENDIF 

# Begin Target

# Name "libQuake3 - Win32 Release"
# Name "libQuake3 - Win32 Debug"
# Begin Group "DLL Source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\libQuake3.cpp
# End Source File
# Begin Source File

SOURCE=..\src\libQuake3.h
# End Source File
# End Group
# Begin Group "VFSPlugin_Q3BSP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\VirtualFS\VFSPlugin_Q3BSP.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\VirtualFS\VFSPlugin_Q3BSP.h
# End Source File
# End Group
# Begin Group "VFSPlugin_TGA"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\VirtualFS\VFSPlugin_TGA.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\VirtualFS\VFSPlugin_TGA.h
# End Source File
# End Group
# Begin Group "VFSPlugin_JPEG"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\VirtualFS\VFSPlugin_JPEG.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\VirtualFS\VFSPlugin_JPEG.h
# End Source File
# End Group
# End Target
# End Project
