echo "--> Deleting dll/log files"
del exe\system\*.dll

echo "--> Copying the engine"
copy c:\data\code\Fusion\bin\libDI8*.dll				exe\system
copy c:\data\code\Fusion\bin\libOpenGL*.dll			exe\system
copy c:\data\code\Fusion\bin\libFMOD*.dll				exe\system

echo "--> Copying the VFS"
copy c:\data\code\Fusion\bin\libTGA*.dll				exe\system
copy c:\data\code\Fusion\bin\libVFS*.dll				exe\system

echo "--> Copying External libraries"
copy c:\data\code\Fusion\External\bin\fmod.dll	exe