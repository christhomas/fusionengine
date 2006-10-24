# Microsoft Developer Studio Project File - Name="libInput" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libInput - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libInput.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libInput.mak" CFG="libInput - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libInput - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libInput - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libInput - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release\libInput"
# PROP Intermediate_Dir "Release\libInput"
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
# ADD LIB32 /nologo /out:"../../lib/libInput.lib"

!ELSEIF  "$(CFG)" == "libInput - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug\libInput"
# PROP Intermediate_Dir "Debug\libInput"
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
# ADD LIB32 /nologo /out:"../../lib/libInput_d.lib"

!ENDIF 

# Begin Target

# Name "libInput - Win32 Release"
# Name "libInput - Win32 Debug"
# Begin Group "InputDeviceDB"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\Input\InputDeviceDB.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\InputDeviceDB.h
# End Source File
# End Group
# Begin Group "Supported Devices"

# PROP Default_Filter ""
# Begin Group "Joystick"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\Input\InputJoystick.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\Input\InputJoystick.h
# End Source File
# End Group
# Begin Group "Keyboard"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\Input\InputKeyboard.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\Input\InputKeyboard.h
# End Source File
# End Group
# Begin Group "Mouse"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\Input\InputMouse.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Include\Input\InputMouse.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\Include\Input\IInputDevice.h
# End Source File
# End Group
# Begin Group "Misc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Include\Input\ButtonCodes.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Input\InputEvent.h
# End Source File
# End Group
# End Target
# End Project
