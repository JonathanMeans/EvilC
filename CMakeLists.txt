cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set the project name
project(EvilC)

# add the executable
add_library(EvilCLib STATIC "platform.h" "platform_windows.h" platform_macos.h "platform_windows.cpp" "Platform.cpp" "platform_macos.cpp" "backend.h" "backend.cpp" "file_utils.cpp" "ArgumentParser.cpp" "lexer.cpp")
target_include_directories(EvilCLib PUBLIC ${CMAKE_CURRENT_LIST_DIR})
add_executable(EvilC main.cpp "backend.h" "backend.cpp")
target_link_libraries(EvilC EvilCLib)

enable_testing()

add_subdirectory(tests)