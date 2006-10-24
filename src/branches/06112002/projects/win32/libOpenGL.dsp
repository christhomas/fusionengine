# Microsoft Developer Studio Project File - Name="libOpenGL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=libOpenGL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libOpenGL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libOpenGL.mak" CFG="libOpenGL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libOpenGL - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "libOpenGL - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libOpenGL - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release\libOpenGL"
# PROP Intermediate_Dir "Release\libOpenGL"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBOPENGL_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBOPENGL_EXPORTS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 libFusion.lib libGraphics.lib libMaths.lib opengl32.lib glu32.lib gdi32.lib user32.lib /nologo /dll /machine:I386 /out:"../../bin/libOpenGL.dll"
# SUBTRACT LINK32 /profile

!ELSEIF  "$(CFG)" == "libOpenGL - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug\libOpenGL"
# PROP Intermediate_Dir "Debug\libOpenGL"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBOPENGL_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBOPENGL_EXPORTS" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 libFusion_d.lib libGraphics_d.lib libMaths_d.lib opengl32.lib glu32.lib gdi32.lib user32.lib /nologo /dll /debug /machine:I386 /out:"../../bin/libOpenGL_d.dll" /pdbtype:sept
# SUBTRACT LINK32 /profile

!ENDIF 

# Begin Target

# Name "libOpenGL - Win32 Release"
# Name "libOpenGL - Win32 Debug"
# Begin Group "DLL Source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\libOpenGL.cpp
# End Source File
# Begin Source File

SOURCE=..\src\libOpenGL.h
# End Source File
# End Group
# Begin Group "OpenGL 1.2"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\OpenGL\OpenGL12.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\OpenGL\OpenGL12.h
# End Source File
# End Group
# Begin Group "OGLGraphics"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Include\Graphics\IGraphics.h
# End Source File
# Begin Source File

SOURCE=..\..\src\OpenGL\OGLGraphics.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\OpenGL\OGLGraphics.h
# End Source File
# End Group
# Begin Group "WGLGraphics"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\arch\Win32\OpenGL\WGLGraphics.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\arch\Win32\OpenGL\WGLGraphics.h
# End Source File
# End Group
# Begin Group "Window"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Include\Graphics\IWindowDevice.h
# End Source File
# Begin Source File

SOURCE=..\..\src\arch\Win32\OpenGL\WGLDevice.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\arch\Win32\OpenGL\WGLDevice.h
# End Source File
# Begin Source File

SOURCE=..\..\src\arch\Win32\Win32Device.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\arch\Win32\Win32Device.h
# End Source File
# End Group
# Begin Group "Texture"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Include\Graphics\ITexture.h
# End Source File
# Begin Source File

SOURCE=..\..\src\OpenGL\OGLImageTexture.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\OpenGL\OGLImageTexture.h
# End Source File
# Begin Source File

SOURCE=..\..\src\OpenGL\OGLProceduralTexture.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\OpenGL\OGLProceduralTexture.h
# End Source File
# Begin Source File

SOURCE=..\..\src\OpenGL\OGLTexture.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\OpenGL\OGLTexture.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Graphics\Pixel.h
# End Source File
# End Group
# Begin Group "VertexBuffer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Include\Graphics\IVertexBuffer.h
# End Source File
# Begin Source File

SOURCE=..\..\src\OpenGL\OGLDynamicVB.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\OpenGL\OGLDynamicVB.h
# End Source File
# Begin Source File

SOURCE=..\..\src\OpenGL\OGLStaticVB.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\OpenGL\OGLStaticVB.h
# End Source File
# End Group
# Begin Group "Shader"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Include\Graphics\IShader.h
# End Source File
# Begin Source File

SOURCE=..\..\src\OpenGL\OGLShader.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\OpenGL\OGLShader.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\Include\OpenGL\DriverData.h
# End Source File
# End Target
# End Project
