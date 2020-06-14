/**
 * Load dlls from inside a program.
 *
 * You can name this something in the windows search path to have it load automatically,
 *  or inject this from outside if you don't mind doing that.
 */
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <shellapi.h>
#include <cstdio>
#include <cwchar>
#include <iostream>

using std::wcout;
using std::endl;

void OpenConsole() {
    AllocConsole();
    FILE* fDummy;
    freopen_s(&fDummy, "CONIN$", "r", stdin);
    freopen_s(&fDummy, "CONOUT$", "w", stderr);
    freopen_s(&fDummy, "CONOUT$", "w", stdout);
}

void LoadDLL(LPWSTR moduleName) {
    LoadLibraryW(moduleName);
}

LPCWSTR consoleFlag = L"-console";
LPCWSTR loadDLLFlag = L"-loaddll";

void CheckParams() {
    int argc = 0;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);

    for (int c = 0; c < argc; c++) {
        if (wcscmp(argv[c], consoleFlag) == 0) {
            OpenConsole();
        }
    }

    for (int c = 0; c < argc; c++) {
        if (!wcscmp(argv[c], loadDLLFlag) && ++c < argc) {
            LoadDLL(argv[c]);
        }
    }
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CheckParams();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

