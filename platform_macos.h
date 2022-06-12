
#ifndef EVILC_PLATFORM_MACOS_H
#define EVILC_PLATFORM_MACOS_H

#include <iostream>
#include <vector>

class PlatformMacOS : public PlatformBase
{
public:
    PlatformMacOS();

    void runProgram(const std::string& programName,
                    const std::vector<std::string>& arguments) const override;
};

void runProgram(const std::string& programName,
                const std::vector<std::string>& arguments)
{
}
#endif  // EVILC_PLATFORM_MACOS_H
