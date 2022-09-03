find_package (Python COMPONENTS Interpreter)
if (NOT ${Python_FOUND})
    message(FATAL_ERROR "Python Interpreter not found.")
endif()

file(GENERATE 
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/e2e_config.py" 
    CONTENT "compiler_binary=\"$<TARGET_FILE:EvilC>\"\ntest_input_dir=\"${CMAKE_CURRENT_BINARY_DIR}\"")

configure_file(${CMAKE_CURRENT_LIST_DIR}/run_e2e_tests.py
    ${CMAKE_CURRENT_BINARY_DIR}/run_e2e_tests.py COPYONLY)

add_test(NAME E2ETests 
         COMMAND ${Python_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/run_e2e_tests.py)