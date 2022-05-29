cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set the project name
project(EvilC)

# add the executable
add_executable(EvilC main.cpp "platform.h" "platform_windows.h" platform_macos.h)

enable_testing()

add_subdirectory(tests)