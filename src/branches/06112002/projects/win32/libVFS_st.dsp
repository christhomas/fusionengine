# Microsoft Developer Studio Project File - Name="libVFS_st" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libVFS_st - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libVFS_st.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libVFS_st.mak" CFG="libVFS_st - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libVFS_st - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libVFS_st - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libVFS_st - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release\libVFS"
# PROP Intermediate_Dir "Release\libVFS"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../lib/libVFS_st.lib"

!ELSEIF  "$(CFG)" == "libVFS_st - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug\libVFS"
# PROP Intermediate_Dir "Debug\libVFS"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../lib/libVFS_st_d.lib"

!ENDIF 

# Begin Target

# Name "libVFS_st - Win32 Release"
# Name "libVFS_st - Win32 Debug"
# Begin Group "VirtualFS"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\VirtualFS\VirtualFS.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\VirtualFS\VirtualFS.h
# End Source File
# End Group
# Begin Group "VFSTransport"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\VirtualFS\VFSTransport.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\VirtualFS\VFSTransport.h
# End Source File
# End Group
# Begin Group "VFSHandle"

# PROP Default_Filter ""
# Begin Group "VFSHandle_file"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\VirtualFS\VFSHandle_file.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\VirtualFS\VFSHandle_file.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\Include\VirtualFS\VFSHandle.h
# End Source File
# End Group
# Begin Group "VFSPlugin"

# PROP Default_Filter ""
# Begin Group "VFSPlugin_BIN"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\VirtualFS\VFSPlugin_BIN.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\VirtualFS\VFSPlugin_BIN.h
# End Source File
# End Group
# Begin Group "VFSPlugin_TXT"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\VirtualFS\VFSPlugin_TXT.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\VirtualFS\VFSPlugin_TXT.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\Include\VirtualFS\FileInfo.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\VirtualFS\VFSPlugin.h
# End Source File
# End Group
# Begin Group "VFSFilter"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Include\VirtualFS\VFSFilter.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\src\VirtualFS\VirtualFS_static.cpp
# End Source File
# End Target
# End Project
