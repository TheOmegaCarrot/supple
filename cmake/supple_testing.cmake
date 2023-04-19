include(supple_compiler_flags)

function(supple_add_test_must_not_compile input_test_file)
  # remove parent directory path and extension
  get_filename_component(filename ${input_test_file} NAME_WLE)
  get_filename_component(raw_path ${input_test_file} DIRECTORY)

  string(REGEX REPLACE ".*/cpp/tests/src/" "" shortened_path ${raw_path})

  string(REPLACE "/" "_" nearly_corrected_path ${shortened_path})
  string(REGEX REPLACE "^_" "" corrected_path ${nearly_corrected_path})

  foreach(numeric_standard ${SUPPLE_TEST_STANDARDS})
    set(test_exe_name ${corrected_path}_${filename}_${numeric_standard})

    add_executable(${test_exe_name} EXCLUDE_FROM_ALL ${input_test_file})

    target_link_libraries(${test_exe_name} PRIVATE supple_compiler_flags)

    target_include_directories(
      ${test_exe_name} PRIVATE ${SUPPLE_TOP_DIR}/cpp/include/supple/core)

    set_target_properties(${test_exe_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                                      ${SUPPLE_TEST_BIN_DIR})

    # remove test_ prefix for prettier test output
    string(REPLACE "test_" "" test_name ${test_exe_name})

    add_test(
      NAME ${test_name}
      COMMAND ${CMAKE_COMMAND} --build . --target ${test_exe_name} --config $<CONFIGURATION>
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
      )

    set_tests_properties(
      ${test_name}
      PROPERTIES WILL_FAIL TRUE
      )

  endforeach()

endfunction()

function(supple_add_test input_test_file)
  # remove parent directory path and extension
  get_filename_component(filename ${input_test_file} NAME_WLE)
  get_filename_component(raw_path ${input_test_file} DIRECTORY)

  string(REGEX REPLACE ".*/cpp/tests/src/" "" shortened_path ${raw_path})

  string(REPLACE "/" "_" nearly_corrected_path ${shortened_path})
  string(REGEX REPLACE "^_" "" corrected_path ${nearly_corrected_path})

  foreach(numeric_standard ${SUPPLE_TEST_STANDARDS})
    set(test_exe_name ${corrected_path}_${filename}_${numeric_standard})

    add_executable(${test_exe_name} ${input_test_file})

    target_link_libraries(${test_exe_name} PRIVATE supple_compiler_flags)

    target_include_directories(
      ${test_exe_name} PRIVATE ${SUPPLE_TOP_DIR}/cpp/include/supple/core
                               ${SUPPLE_TOP_DIR}/cpp/include/supple/testing)

    set_target_properties(${test_exe_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                                      ${SUPPLE_TEST_BIN_DIR})

    # remove test_ prefix for prettier test output
    string(REPLACE "test_" "" test_name ${test_exe_name})

    add_test(
      NAME ${test_name}
      COMMAND ${test_exe_name}
      WORKING_DIRECTORY ${SUPPLE_TEST_BIN_DIR})
  endforeach()

endfunction()

function(supple_test_handling)
  set(SUPPLE_TEST_BIN_DIR ${CMAKE_CURRENT_BINARY_DIR}/tests)

  # find all test source files

  # make ctest give useful output to console on test failure
  list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure" "--progress")

  supple_status_message("Testing C++17")
  list(APPEND SUPPLE_TEST_STANDARDS "17")
  if(SUPPLE_FULL_TESTS)
    if(NOT SUPPLE_OMIT_20)
      supple_status_message("Testing C++20")
      list(APPEND SUPPLE_TEST_STANDARDS "20")
    endif()
    if(NOT SUPPLE_OMIT_23)
      supple_status_message("Testing C++23")
      list(APPEND SUPPLE_TEST_STANDARDS "23")
    endif()
  endif()

  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/cpp)

endfunction()
