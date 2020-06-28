# DLLLoader
A DLL Loading assistant.

This utility is intended for use with Diablo 2, but is not specific to it. It's purpose is to load dlls into the process without using strange injection techniques.

It enables these command line options:

---

`-loaddll "C:\path\to\your\target.dll"`

*This option loads the target dll using 'LoadLibraryW'*

---

`-console`

*Opens a console window to allow stdout/stdin functions. This is usually only useful when debugging.*

---

`-fail`

*Forces the DLL Loader to report that it failed to load (keeps the loader itself from staying loaded in memory). This may not be useful.*

---

Compile this from source, or download the latest version from the releases tab at [https://github.com/Nishimura-Katsuo/DLLLoader/releases]. Then copy the resulting dll file into the game directory, and keep the name 'dbghelp.dll'.

To compile this from source, use Visual Studio 2019 and compile it for either x86 or x64. Diablo 2 uses x86 builds.