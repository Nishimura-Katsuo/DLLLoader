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
#include <io.h>
#include <fcntl.h>

using std::cout;
using std::wcout;
using std::cin;
using std::wcin;
using std::cerr;
using std::wcerr;

void OpenConsole() {
    //Create a console for this application
    AllocConsole();

    // Get STDOUT handle
    HANDLE ConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    int SystemOutput = _open_osfhandle(intptr_t(ConsoleOutput), _O_TEXT);
    FILE* COutputHandle = _fdopen(SystemOutput, "w");

    // Get STDERR handle
    HANDLE ConsoleError = GetStdHandle(STD_ERROR_HANDLE);
    int SystemError = _open_osfhandle(intptr_t(ConsoleError), _O_TEXT);
    FILE* CErrorHandle = _fdopen(SystemError, "w");

    // Get STDIN handle
    HANDLE ConsoleInput = GetStdHandle(STD_INPUT_HANDLE);
    int SystemInput = _open_osfhandle(intptr_t(ConsoleInput), _O_TEXT);
    FILE* CInputHandle = _fdopen(SystemInput, "r");

    //make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog point to console as well
    std::ios::sync_with_stdio(true);

    // Redirect the CRT standard input, output, and error handles to the console
    freopen_s(&CInputHandle, "CONIN$", "r", stdin);
    freopen_s(&COutputHandle, "CONOUT$", "w", stdout);
    freopen_s(&CErrorHandle, "CONOUT$", "w", stderr);

    //Clear the error state for each of the C++ standard stream objects. We need to do this, as
    //attempts to access the standard streams before they refer to a valid target will cause the
    //iostream objects to enter an error state. In versions of Visual Studio after 2005, this seems
    //to always occur during startup regardless of whether anything has been read from or written to
    //the console or not.
    std::wcout.clear();
    std::cout.clear();
    std::wcerr.clear();
    std::cerr.clear();
    std::wcin.clear();
    std::cin.clear();
}

void LoadDLL(LPWSTR moduleName) {
    if (LoadLibraryW(moduleName)) {
        std::wcout << moduleName << " loaded!" << std::endl;
    }
    else {
        std::wcout << moduleName << " failed to load: " << GetLastError() << std::endl;
    }
}

LPCWSTR consoleFlag = L"-console";
LPCWSTR loadDLLFlag = L"-loaddll";
LPCWSTR failloadFlag = L"-fail";

bool fail = false;

void CheckParams() {
    int argc = 0;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);

    for (int c = 0; c < argc; c++) {
        if (wcscmp(argv[c], consoleFlag) == 0) {
            OpenConsole();
            break;
        }
    }

    for (int c = 0; c < argc; c++) {
        if (!wcscmp(argv[c], loadDLLFlag) && ++c < argc) {
            LoadDLL(argv[c]);
        }
    }

    for (int c = 0; c < argc; c++) {
        if (!wcscmp(argv[c], failloadFlag) && ++c < argc) {
            fail = true;
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
    return !fail;
}
