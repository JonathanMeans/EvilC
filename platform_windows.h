#ifndef EVILC_PLATFORM_WINDOWS_H
#define EVILC_PLATFORM_WINDOWS_H

#include <iostream>

class PlatformWindows : public PlatformBase
{
public:
    PlatformWindows();
    void runProgram(const std::string& programName,
                    const std::vector<std::string>& arguments) const override;
};

#endif
