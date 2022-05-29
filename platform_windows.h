#ifndef EVILC_PLATFORM_WINDOWS_H
#define EVILC_PLATFORM_WINDOWS_H
#include <Windows.h>

#include <iostream>

void runProgram(const std::string& programName, std::string arguments)
{
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    // set the size of the structures
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    char windowsArguments[100];
    strcpy(windowsArguments, arguments.c_str());

    // start the program up
    if (!CreateProcessA(programName.c_str(),  // the path
                        windowsArguments,  // Command line
                        NULL,  // Process handle not inheritable
                        NULL,  // Thread handle not inheritable
                        FALSE,  // Set handle inheritance to FALSE
                        CREATE_NEW_CONSOLE,  // Opens file in a separate console
                        NULL,  // Use parent's environment block
                        NULL,  // Use parent's starting directory
                        &si,  // Pointer to STARTUPINFO structure
                        &pi  // Pointer to PROCESS_INFORMATION structure
                        ))
    {
        std::cerr << "CreateProcess failed\n";
        return;
    }
    WaitForSingleObject(pi.hProcess, INFINITE);
    // Close process and thread handles.
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}
#endif
