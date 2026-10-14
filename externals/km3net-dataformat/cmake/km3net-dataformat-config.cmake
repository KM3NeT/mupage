include(CMakeFindDependencyMacro)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})
find_dependency(ROOT)
find_dependency(libuuid)
include(${CMAKE_CURRENT_LIST_DIR}/km3net-dataformat.cmake)

