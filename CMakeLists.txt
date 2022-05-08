cmake_minimum_required(VERSION 3.10)

# set the project name
project(EvilC)

# add the executable
add_executable(EvilC main.cpp "tests/unittests/test_smoke.cpp")

enable_testing()

add_subdirectory(tests)