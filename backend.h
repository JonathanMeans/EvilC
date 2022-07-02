#ifndef EVILC_BACKEND_H
#define EVILC_BACKEND_H

#include <filesystem>

#include "platform.h"

void generateExecutableFromBytecode(const Platform& platform,
                                    std::filesystem::path llvmIrPath,
                                    std::filesystem::path outputPath);
#endif
