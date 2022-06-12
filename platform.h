#ifndef EVILC_PLATFORM_H
#define EVILC_PLATFORM_H

#include <string>
#include <vector>

class PlatformBase
{
public:
    explicit PlatformBase(const char* defaultOutputFilename);

    virtual ~PlatformBase() = default;

    virtual void runProgram(
            const std::string& programName,
            const std::vector<std::string>& arguments) const = 0;

    const char* const defaultOutputFilename;
};

#if defined(WIN32) || defined(_WIN32)
class PlatformWindows;
#define Platform PlatformWindows
#include "platform_windows.h"
#elif defined(__APPLE__)
class PlatformMacOS;
#define Platform PlatformMacOS
#include "platform_macos.h"
#endif

#endif
