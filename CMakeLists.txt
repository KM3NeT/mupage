cmake_minimum_required(VERSION 3.27)

list(APPEND CMAKE_MESSAGE_CONTENT mupage)

project(km3net-mupage LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(KM3NeTCMakeSetup
  GIT_REPOSITORY git@git.km3net.de:common/KM3NeTCMakeSetup
  GIT_TAG 98ada9547db57178c5c71a30d45ea771657d7f50) # v1.1
FetchContent_MakeAvailable(KM3NeTCMakeSetup)
KM3NeTCMakeSetup(NO_LIB64)

option(KM3NET_MUPAGE_ENABLE_ANTARES "use antares-dataformat" TRUE)
option(KM3NET_MUPAGE_ENABLE_KM3NET "use km3net-dataformat" TRUE)
option(KM3NET_MUPAGE_ENABLE_TESTING "enable testing" ${PROJECT_IS_TOP_LEVEL})
option(KM3NET_MUPAGE_STANDALONE "install libraries from dependencies alongside our libraries" OFF)

if(KM3NET_MUPAGE_ENABLE_TESTING)
  include(CTest)
endif()

include(dependencies.cmake)

add_executable(km3net-mupage.exe)

target_sources(km3net-mupage.exe PRIVATE
  src/Date.cc
  src/EventTime.cc
  src/Muons.cc
  src/OutputWriter.cc
  src/OutputWriterNative.cc
  src/OutputWriterANTARES.cc
  src/OutputWriterKM3NeT.cc
  src/Parameters.cc
  src/Vector.cc
  src/Random.cc
  src/mupage.cc
  )

set_target_properties(km3net-mupage.exe PROPERTIES OUTPUT_NAME mupage.exe)

if(TARGET km3net::dataformat)
                        target_link_libraries(km3net-mupage.exe
                        PUBLIC
                        ROOT::RIO
                        ROOT::Cling
                        ROOT::TreePlayer
                        km3net::dataformat)
endif()
target_include_directories(km3net-mupage.exe PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>)

find_package(Git)
if(Git_FOUND)
  set(KM3NET_MUPAGE_GIT_VERSION "unknown")
  set(KM3NET_MUPAGE_GIT_DATE "unknown")
  execute_process(
    COMMAND ${GIT_EXECUTABLE} --no-pager describe --tags --always
    RESULT_VARIABLE result
    OUTPUT_VARIABLE KM3NET_MUPAGE_GIT_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
  execute_process(
    COMMAND ${GIT_EXECUTABLE} show -s --format=%cI HEAD
    RESULT_VARIABLE result
    OUTPUT_VARIABLE KM3NET_MUPAGE_GIT_DATE
    OUTPUT_STRIP_TRAILING_WHITESPACE
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
  target_compile_definitions(km3net-mupage.exe
    PRIVATE
    GIT_VERSION="${KM3NET_MUPAGE_GIT_VERSION}"
    BUILD_DATE="${KM3NET_MUPAGE_GIT_DATE}")
endif()

if(KM3NET_MUPAGE_ENABLE_KM3NET)
  target_compile_definitions(km3net-mupage.exe
    PRIVATE _KM3NET_ENABLED__)
endif()

install(
    TARGETS km3net-mupage.exe
    RUNTIME_DEPENDENCY_SET km3net-mupage-deps
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})


configure_file(${CMAKE_CURRENT_LIST_DIR}/dat/parameters.dat ${PROJECT_BINARY_DIR}/stage/${CMAKE_INSTALL_DATAROOTDIR}/mupage/dat/parameters.dat COPYONLY)

install(FILES ${PROJECT_BINARY_DIR}/stage/${CMAKE_INSTALL_DATAROOTDIR}/mupage/dat/parameters.dat DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/mupage/dat)

configure_file(${CMAKE_CURRENT_LIST_DIR}/benchmark/mupage.txt ${PROJECT_BINARY_DIR}/stage/${CMAKE_INSTALL_DATAROOTDIR}/mupage/benchmark/mupage.txt COPYONLY)

install(FILES ${PROJECT_BINARY_DIR}/stage/${CMAKE_INSTALL_DATAROOTDIR}/mupage/benchmark/mupage.txt DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/mupage/benchmark)

configure_file(${CMAKE_CURRENT_LIST_DIR}/test.sh ${PROJECT_BINARY_DIR}/stage/${CMAKE_INSTALL_BINDIR}/test.sh COPYONLY)

if(BUILD_TESTING)
  add_test(NAME basic-run COMMAND ${PROJECT_BINARY_DIR}/stage/${CMAKE_INSTALL_BINDIR}/test.sh)
  set_tests_properties(basic-run PROPERTIES
    ENVIRONMENT
      "PATH=$<TARGET_FILE_DIR:km3net-mupage.exe>:$ENV{PATH};MUPAGE_DAT_DIR=${PROJECT_BINARY_DIR}/stage/${CMAKE_INSTALL_DATAROOTDIR}/mupage/dat;MUPAGE_BENCHMARK_DIR=${PROJECT_BINARY_DIR}/stage/${CMAKE_INSTALL_DATAROOTDIR}/mupage/benchmark"
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
endif()

if(KM3NET_MUPAGE_STANDALONE)
  install(RUNTIME_DEPENDENCY_SET km3net-mupage-deps
   PRE_EXCLUDE_REGEXES
        [[libc\.so\..*]]
        [[libgcc_s\.so\..*]]
        [[libm\.so\..*]]
        [[libstdc\+\+\.so\..*]])
endif()

list(POP_BACK CMAKE_MESSAGE_CONTENT)

target_include_directories(km3net-mupage.exe PRIVATE externals/CLI)
