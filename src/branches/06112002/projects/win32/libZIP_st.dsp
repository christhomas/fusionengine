# Microsoft Developer Studio Project File - Name="libZIP_st" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libZIP_st - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libZIP_st.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libZIP_st.mak" CFG="libZIP_st - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libZIP_st - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libZIP_st - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libZIP_st - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release\libZIP"
# PROP Intermediate_Dir "Release\libZIP"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "v:\fusion\external\include\zlib" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../lib/libZIP_st.lib"

!ELSEIF  "$(CFG)" == "libZIP_st - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug\libZIP"
# PROP Intermediate_Dir "Debug\libZIP"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "v:\fusion\external\include\zlib" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../lib/libZIP_st_d.lib"

!ENDIF 

# Begin Target

# Name "libZIP_st - Win32 Release"
# Name "libZIP_st - Win32 Debug"
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
# Begin Group "VFSHandle_ZIP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\VirtualFS\VFSHandle_ZIP.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\VirtualFS\VFSHandle_ZIP.h
# End Source File
# End Group
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
# Begin Group "zlib"

# PROP Default_Filter ""
# Begin Group "src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\External\src\zlib\adler32.c
# End Source File
# Begin Source File

SOURCE=..\..\External\src\zlib\compress.c
# End Source File
# Begin Source File

SOURCE=..\..\External\src\zlib\crc32.c
# End Source File
# Begin Source File

SOURCE=..\..\External\src\zlib\deflate.c
# End Source File
# Begin Source File

SOURCE=..\..\External\src\zlib\gvmat32c.c
# End Source File
# Begin Source File

SOURCE=..\..\External\src\zlib\gzio.c
# End Source File
# Begin Source File

SOURCE=..\..\External\src\zlib\infblock.c
# End Source File
# Begin Source File

SOURCE=..\..\External\src\zlib\infcodes.c
# End Source File
# Begin Source File

SOURCE=..\..\External\src\zlib\inffast.c
# End Source File
# Begin Source File

SOURCE=..\..\External\src\zlib\inflate.c
# End Source File
# Begin Source File

SOURCE=..\..\External\src\zlib\inftrees.c
# End Source File
# Begin Source File

SOURCE=..\..\External\src\zlib\infutil.c
# End Source File
# Begin Source File

SOURCE=..\..\External\src\zlib\maketree.c
# End Source File
# Begin Source File

SOURCE=..\..\External\src\zlib\trees.c
# End Source File
# Begin Source File

SOURCE=..\..\External\src\zlib\uncompr.c
# End Source File
# Begin Source File

SOURCE=..\..\External\src\zlib\zutil.c
# End Source File
# End Group
# Begin Group "headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\External\include\zlib\deflate.h
# End Source File
# Begin Source File

SOURCE=..\..\External\include\zlib\infblock.h
# End Source File
# Begin Source File

SOURCE=..\..\External\include\zlib\infcodes.h
# End Source File
# Begin Source File

SOURCE=..\..\External\include\zlib\inffast.h
# End Source File
# Begin Source File

SOURCE=..\..\External\include\zlib\inffixed.h
# End Source File
# Begin Source File

SOURCE=..\..\External\include\zlib\inftrees.h
# End Source File
# Begin Source File

SOURCE=..\..\External\include\zlib\infutil.h
# End Source File
# Begin Source File

SOURCE=..\..\External\include\zlib\trees.h
# End Source File
# Begin Source File

SOURCE=..\..\External\include\zlib\zconf.h
# End Source File
# Begin Source File

SOURCE=..\..\External\include\zlib\zlib.h
# End Source File
# Begin Source File

SOURCE=..\..\External\include\zlib\zutil.h
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=..\..\src\VirtualFS\VFSHandle_ZIP_Static.cpp
# End Source File
# End Target
# End Project
