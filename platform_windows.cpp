#if defined(WIN32) || defined(_WIN32)

#include <Windows.h>

#include <numeric>
#include <vector>

#include "platform.h"

PlatformWindows::PlatformWindows() : PlatformBase("a.exe")
{
}

void PlatformWindows::runProgram(
        const std::string& programName,
        const std::vector<std::string>& arguments) const
{
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    // set the size of the structures
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    auto appendArgument = [](const std::string& initial,
                             const std::string& appending) {
        return initial + " " + appending;
    };
    std::string fullArgumentString = std::accumulate(arguments.begin(),
                                                     arguments.end(),
                                                     programName,
                                                     appendArgument);
    std::vector<char> windowsArguments(fullArgumentString.size() + 1);
    strcpy_s(windowsArguments.data(),
             windowsArguments.size(),
             fullArgumentString.c_str());
    windowsArguments[windowsArguments.size() - 1] = NULL;

    // start the program up
    if (!CreateProcessA(NULL,
                        windowsArguments.data(), // Command line
                        NULL, // Process handle not inheritable
                        NULL, // Thread handle not inheritable
                        FALSE, // Set handle inheritance to FALSE
                        CREATE_NEW_CONSOLE, // Opens file in a separate console
                        NULL, // Use parent's environment block
                        NULL, // Use parent's starting directory
                        &si, // Pointer to STARTUPINFO structure
                        &pi // Pointer to PROCESS_INFORMATION structure
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
