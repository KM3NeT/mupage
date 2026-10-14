include(CheckCXXSymbolExists)
include(CheckLibraryExists)
include(FindPackageHandleStandardArgs)

if(TARGET uuid::uuid)
  return()
endif()

find_path(UUID_INCLUDE_DIR uuid/uuid.h)

set(_have_uuid_header FALSE)
set(_have_uuid_library FALSE)

if(UUID_INCLUDE_DIR)
  set(CMAKE_REQUIRED_INCLUDES ${UUID_INCLUDE_DIR})
  check_cxx_symbol_exists("uuid_generate_random" "uuid/uuid.h" _have_uuid_header)
endif()

check_library_exists("uuid" "uuid_generate_random" "" _have_uuid_library)

message(VERBOSE "_have_uuid_header=${_have_uuid_header}")
message(VERBOSE "_have_uuid_library=${_have_uuid_library}")

if(DEFINED _have_uuid_header AND NOT DEFINED UUID_LIBRARY AND NOT APPLE)
  if(_have_libuuid)
    set(UUID_LIBRARY "uuid")
    set(UUID_LIBRARIES ${UUID_LIBRARY})
  else()
    message(STATUS "Using PkgConfig to find uuid")
    # using our last option to find the package : PkgConfig...
    find_package(PkgConfig REQUIRED)
    pkg_search_module(uuid REQUIRED IMPORTED_TARGET GLOBAL uuid)
    set(UUID_LIBRARY ${pkgcfg_lib_uuid_uuid})
    set(UUID_LIBRARIES ${UUID_LIBRARY})
  endif()
endif()

if(NOT TARGET uuid::uuid)
  message(STATUS "Generating our own uuid::uuid")
  add_library(uuid::uuid INTERFACE IMPORTED)
  if(NOT APPLE)
    message(VERBOSE "UUID_INCLUDE_DIR=${UUID_INCLUDE_DIR} UUID_LIBRARY=${UUID_LIBRARY}")
    set_property(TARGET uuid::uuid PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${UUID_INCLUDE_DIR})
    if(NOT _uuid_header_only AND UUID_LIBRARY)
      set_property(TARGET uuid::uuid PROPERTY INTERFACE_LINK_LIBRARIES "${UUID_LIBRARIES}")
    endif()
  endif()
else()
  message(STATUS "uuid::uuid target found")
endif()

unset(CMAKE_REQUIRED_INCLUDES)
unset(_have_uuid_header)
unset(_have_libuuid)

find_package_handle_standard_args(libuuid DEFAULT_MSG UUID_INCLUDE_DIR)
mark_as_advanced(UUID_INCLUDE_DIR UUID_LIBRARY)
