// TemplateBot.cpp : Defines the entry point for the DLL application.
//

#include "BotDLL.h"
#include <windows.h>

BOOL APIENTRY DllMain( HANDLE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
{
	return TRUE;
}

TEMPLATEBOT_API ClientBot * CreateBot(char *name,GameServer *server,int key)
{
	return new SimpleTank2(name,server,key);
}