#ifndef EVILC_PLATFORM_WINDOWS_H
#define EVILC_PLATFORM_WINDOWS_H
#include <Windows.h>

#include <iostream>

void runProgram(const std::string& programName,
                const std::vector<std::string>& arguments)
{
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    // set the size of the structures
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // TODO: Error checking
    char windowsArguments[100];
    strcpy(windowsArguments, programName.c_str());
    strcat(windowsArguments, " ");
    for (const auto& argument : arguments)
    {
        strcat(windowsArguments, argument.c_str());
        strcat(windowsArguments, " ");
    }

    // start the program up
    if (!CreateProcessA(NULL,
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
        printf("CreateProcess failed (%d).\n", GetLastError());
        return;
    }
    WaitForSingleObject(pi.hProcess, INFINITE);
    // Close process and thread handles.
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}
#endif
