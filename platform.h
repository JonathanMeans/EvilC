#ifndef EVILC_PLATFORM_H
#define EVILC_PLATFORM_H

#if defined(WIN32) || defined(_WIN32)
#include "platform_windows.h"
#elif defined(__APPLE__)
#include "platform_macos.h"
#endif

#endif
