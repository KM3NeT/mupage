cmake_minimum_required(VERSION 3.23)

project(km3net-dataformat LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(KM3NeTCMakeSetup
  GIT_REPOSITORY git@git.km3net.de:common/KM3NeTCMakeSetup
  GIT_TAG f146da118440ec98202a81ce5ee84fe8b8f6d027)
FetchContent_MakeAvailable(KM3NeTCMakeSetup)
KM3NeTCMakeSetup()

option(KM3NET_DATAFORMAT_ENABLE_TESTING "enable testing" ${PROJECT_IS_TOP_LEVEL})
option(KM3NET_DATAFORMAT_ENABLE_DOCUMENTATION "build documentation" OFF)
option(KM3NET_DATAFORMAT_ENABLE_HOMEBREW "use homebrew provided dependencies (relevant for macOS only)" ON)

if(KM3NET_DATAFORMAT_ENABLE_TESTING)
  include(CTest)
endif()

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include(dependencies.cmake)

add_subdirectory(definitions)
add_subdirectory(src)

if(PROJECT_IS_TOP_LEVEL)
  add_subdirectory(examples/cpp)
endif()

if(BUILD_TESTING AND KM3NET_DATAFORMAT_ENABLE_TESTING)
  add_subdirectory(tests)
endif()

install(EXPORT km3net-dataformat
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/km3net-dataformat
        NAMESPACE km3net::
        )

install(FILES
          cmake/km3net-dataformat-config.cmake
          cmake/Findlibuuid.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/km3net-dataformat)

if(KM3NET_DATAFORMAT_ENABLE_DOCUMENTATION)
  include(cmake/doxygen.cmake)
endif()

if(NOT PROJECT_IS_TOP_LEVEL)
  # do not show those options in the basic options as
  # they are probably of little interest for project including us
  mark_as_advanced(KM3NET_DATAFORMAT_ENABLE_TESTING)
endif()

