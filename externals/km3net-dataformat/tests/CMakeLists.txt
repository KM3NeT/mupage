add_executable(head-behavior-test test_head.cc)
target_link_libraries(head-behavior-test PRIVATE km3net::dataformat)

add_test(NAME head
 COMMAND $<TARGET_FILE:head-behavior-test>)

set_tests_properties(head PROPERTIES
  ENVIRONMENT "LD_LIBRARY_PATH=$<TARGET_FILE_DIR:km3net-dataformat>"
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

add_test(NAME load
        COMMAND $<TARGET_FILE:ROOT::root> -b -q ${CMAKE_CURRENT_SOURCE_DIR}/load.C)
set_tests_properties(load PROPERTIES
        ENVIRONMENT "LD_LIBRARY_PATH=$<TARGET_FILE_DIR:km3net-dataformat>;ROOT_INCLUDE_PATH=${PROJECT_BINARY_DIR}"
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
