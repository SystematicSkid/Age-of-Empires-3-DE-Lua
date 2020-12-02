#include <windows.h>
#include <iostream>
#include "./Core/Core.h"

ID3D11DeviceContext* pContext = NULL;
ID3D11Device* pDevice = NULL;
HWND g_hwnd;

VOID Initialize()
{
	FILE* pNewStdout = NULL;
	FILE* pNewStderr = NULL;
	FILE* pNewStdin = NULL;

	/* Initialize our output console */
	// Allocate console
	AllocConsole();
	SetConsoleTitleA("Age of Empires 3 - DE");
	// Assign `stdin`, `stout`, `stderr`
	::freopen_s(&pNewStdin, "CONIN$", "r", stdin);
	::freopen_s(&pNewStdout, "CONOUT$", "w", stdout);
	::freopen_s(&pNewStderr, "CONOUT$", "w", stderr);
	g_hwnd = (HWND)FindWindowA("Age3DE", NULL); 

	Core::Initialize();
}

BOOL WINAPI DllMain(_In_ void* _DllHandle, _In_ unsigned long _Reason, _In_opt_ void* _Reserved)
{
	if (_Reason != DLL_PROCESS_ATTACH) return FALSE;

	/* TODO: Remove usage of thread, we hijack one anyways... */
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)(Initialize), NULL, NULL, NULL);

	return TRUE;
}