function(custom_find_root)
  if(APPLE)
    # until
    # https://github.com/root-project/root/issues/14113#issuecomment-1907991061
    # is resolved, need some special treatment for Root installed with Homebrew
    find_program(BREW_EXECUTABLE brew)
    if(BREW_EXECUTABLE)
      # even on mac, we might want to bypass homebrew (to use conda or Spack
      # instead for instance), so check we really want it
      if(KM3NET_DATAFORMAT_ENABLE_HOMEBREW)
        set(CMAKE_PREFIX_PATH "/opt/homebrew/include/root;/opt/homebrew/lib/root")
        find_package(FLEX REQUIRED)
      endif()
    endif()
  endif()
  find_package(ROOT REQUIRED)
endfunction()

find_package(libuuid REQUIRED)

custom_find_root()

if(NOT TARGET ROOT::root AND TARGET ROOT::rootcling)
  get_target_property(_rootcling_location ROOT::rootcling IMPORTED_LOCATION)
  get_filename_component(_root_bindir "${_rootcling_location}" DIRECTORY)
  find_program(ROOT_EXECUTABLE NAMES root root.exe HINTS "${_root_bindir}" REQUIRED)
  add_executable(ROOT::root IMPORTED GLOBAL)
  set_target_properties(ROOT::root PROPERTIES
    IMPORTED_LOCATION "${ROOT_EXECUTABLE}")
endif()

if(NOT TARGET ROOT::root)
  message(FATAL_ERROR "ROOT installation is incomplete: missing executable target ROOT::root and/or ROOT::rootcling")
endif()
