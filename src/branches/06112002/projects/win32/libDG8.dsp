# Microsoft Developer Studio Project File - Name="libDG8" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=libDG8 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libDG8.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libDG8.mak" CFG="libDG8 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libDG8 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "libDG8 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libDG8 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release\libDG8"
# PROP Intermediate_Dir "Release\libDG8"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBDG8_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBDG8_EXPORTS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 libFusion.lib libGraphics.lib libMaths.lib d3d8.lib d3dx8.lib dxguid.lib user32.lib /nologo /dll /machine:I386 /out:"../../bin/libDG8.dll"

!ELSEIF  "$(CFG)" == "libDG8 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug\libDG8"
# PROP Intermediate_Dir "Debug\libDG8"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBDG8_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBDG8_EXPORTS" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 libFusion_d.lib libGraphics_d.lib libMaths_d.lib d3d8.lib d3dx8.lib dxguid.lib user32.lib /nologo /dll /debug /machine:I386 /out:"../../bin/libDG8_d.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "libDG8 - Win32 Release"
# Name "libDG8 - Win32 Debug"
# Begin Group "DLL Source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\libDG8.cpp
# End Source File
# Begin Source File

SOURCE=.\src\libDG8.h
# End Source File
# End Group
# Begin Group "Graphics"

# PROP Default_Filter ""
# Begin Source File

SOURCE=C:\Data\Code\Fusion\src\arch\Win32\DirectX\DG8Graphics.cpp
# End Source File
# Begin Source File

SOURCE=C:\Data\Code\Fusion\Include\arch\Win32\DirectX\DG8Graphics.h
# End Source File
# Begin Source File

SOURCE=C:\Data\Code\Fusion\Include\IGraphics.h
# End Source File
# End Group
# Begin Group "Win32Window"

# PROP Default_Filter ""
# Begin Source File

SOURCE=C:\Data\Code\Fusion\src\arch\Win32\Win32Device.cpp
# End Source File
# Begin Source File

SOURCE=C:\Data\Code\Fusion\Include\arch\Win32\Win32Device.h
# End Source File
# End Group
# Begin Group "VertexBuffer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\arch\Win32\DirectX\DG8DynamicVB.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\arch\Win32\DirectX\DG8DynamicVB.h
# End Source File
# Begin Source File

SOURCE=..\..\src\arch\Win32\DirectX\DG8StaticVB.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\arch\Win32\DirectX\DG8StaticVB.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Graphics\IVertexBuffer.h
# End Source File
# End Group
# End Target
# End Project
