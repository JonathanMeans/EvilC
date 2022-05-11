add_subdirectory(unittests)
add_test(NAME E2ETests COMMAND python3 ${CMAKE_CURRENT_LIST_DIR}/e2e/run_e2e_tests.py $<TARGET_FILE:EvilC>)