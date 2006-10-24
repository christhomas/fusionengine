# Microsoft Developer Studio Project File - Name="libGraphics" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libGraphics - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libGraphics.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libGraphics.mak" CFG="libGraphics - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libGraphics - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libGraphics - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libGraphics - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release\libGraphics"
# PROP Intermediate_Dir "Release\libGraphics"
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
# ADD LIB32 /nologo /out:"../../lib/libGraphics.lib"

!ELSEIF  "$(CFG)" == "libGraphics - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug\libGraphics"
# PROP Intermediate_Dir "Debug\libGraphics"
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
# ADD LIB32 /nologo /out:"../../lib/libGraphics_d.lib"

!ENDIF 

# Begin Target

# Name "libGraphics - Win32 Release"
# Name "libGraphics - Win32 Debug"
# Begin Group "Mesh Classes"

# PROP Default_Filter ""
# Begin Group "Entity"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\Mesh\Entity.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\Mesh\Entity.h
# End Source File
# End Group
# Begin Group "MeshDB"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\Mesh\MeshDB.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\Mesh\MeshDB.h
# End Source File
# End Group
# Begin Group "Overlay"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\Mesh\Overlay.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\Mesh\Overlay.h
# End Source File
# End Group
# Begin Group "Mesh"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\Mesh\Mesh.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\Mesh\Mesh.h
# End Source File
# End Group
# End Group
# Begin Group "UserInterface"

# PROP Default_Filter ""
# Begin Group "Components"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\gui\Button.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\GUI\ComponentLibrary.h
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\Components.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\Container.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\GUI\GUIComponents.h
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\ProgressBar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\Textbox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\Window.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\src\gui\gui.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\gui.h
# End Source File
# Begin Source File

SOURCE=..\..\src\gui\guiDB.cpp
# End Source File
# End Group
# Begin Group "SceneGraph"

# PROP Default_Filter ""
# Begin Group "Collision"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\Graphics\Collision.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\Graphics\Collision.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphics\Collision_AABB2D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphics\Collision_OBB2D.cpp
# End Source File
# End Group
# Begin Group "Camera"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\Graphics\Camera.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\Graphics\Camera.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\src\Graphics\SceneGraph.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\Graphics\SceneGraph.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Graphics\SceneGraphDB.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\Graphics\SceneGraphDB.h
# End Source File
# End Group
# Begin Group "FontDB"

# PROP Default_Filter ""
# Begin Group "Fonts"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\Font\BitmapFont.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\Font\BitmapFont.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Font\TextureFont.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\Font\TextureFont.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\src\Font\FontDB.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\IFont.h
# End Source File
# End Group
# Begin Source File

SOURCE=V:\Fusion\Include\IGraphics.h
# End Source File
# Begin Source File

SOURCE=V:\Fusion\Include\Maths.h
# End Source File
# End Target
# End Project
