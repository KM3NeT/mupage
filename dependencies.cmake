
if(KM3NET_MUPAGE_ENABLE_ANTARES)
  find_package(ROOT REQUIRED)
  include(FetchContent)
  FetchContent_Declare(antares-dataformat
     GIT_REPOSITORY git@git.km3net.de:common/antares-dataformat
     GIT_TAG 5dc7aa760516399cb474160f7d75c1235734d263
     FIND_PACKAGE_ARGS NO_CMAKE_INSTALL_PREFIX
)
  FetchContent_MakeAvailable(antares-dataformat)
endif()

if(KM3NET_MUPAGE_ENABLE_KM3NET)
  find_package(ROOT REQUIRED)
  include(FetchContent)
  FetchContent_Declare(km3net-dataformat
    GIT_REPOSITORY git@git.km3net.de:common/km3net-dataformat
    GIT_TAG 8d2fdcbf7c93c5fb7075c92cdd45bf0482e6d3e1 # v3.6.1
    FIND_PACKAGE_ARGS NO_CMAKE_INSTALL_PREFIX
  )
  FetchContent_MakeAvailable(km3net-dataformat)
endif()

