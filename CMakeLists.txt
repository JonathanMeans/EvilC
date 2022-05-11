cmake_minimum_required(VERSION 3.10)

# set the project name
project(EvilC)

# add the executable
add_executable(EvilC main.cpp)

enable_testing()

add_subdirectory(tests)