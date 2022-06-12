#ifndef EVILC_PLATFORM_H
#define EVILC_PLATFORM_H

#if defined(WIN32) || defined(_WIN32)
#include "platform_windows.h"
#elif defined(__APPLE__)
#include "platform_macos.h"
#endif

#if defined(WIN32) || defined(_WIN32)
const char* DEFAULT_OUTPUT_FILENAME = "a.exe";
#elif defined(__APPLE__)
const char* DEFAULT_OUTPUT_FILENAME = "a.out";
#endif

#endif
