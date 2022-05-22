include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(
    unittests "test_smoke.cpp")

enable_testing()

target_link_libraries(unittests gtest gtest_main)
target_include_directories(unittests PUBLIC ${gtest_SOURCE_DIR} ${gtest_SOURCE_DIR}/include)
include(GoogleTest)
gtest_discover_tests(unittests)
add_test(unittests unittests)

