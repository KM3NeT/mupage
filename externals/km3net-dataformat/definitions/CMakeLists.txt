# find the git version

find_package(Git)
set(GIT_VERSION "unknown version")
if(Git_FOUND)
  execute_process(
    COMMAND ${GIT_EXECUTABLE} --no-pager describe --tags --always
    RESULT_VARIABLE result
    OUTPUT_VARIABLE GIT_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    )
  if(result)
      set(GIT_VERSION ${GIT_VERSION})
  endif()
endif()

set(description "KM3NeT Data Definitions ${GIT_VERSION}")
set(url "https://git.km3net.de/common/km3net-dataformat")

# we are generating files in the binary dir, not in the source dir
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/km3net-dataformat/definitions)

add_subdirectory(csv2code)

set(languages "cpp;python;julia;shell")
set(extensions "hh;py;jl;sh")

# generate include, python, julia and shell files from csv files
foreach(lang ext IN ZIP_LISTS languages extensions)
  set(generated "")
  foreach(file
    applications
    daqdatatypes
    fitparameters
    module_status
    pmt_status
    reconstruction
    root
    trigger
    trkmembers
    w2list_genhen
    w2list_gseagen
    w2list_km3buu
    w2list_km3neut
    weightlist
    )
      set(generated_file
        ${PROJECT_BINARY_DIR}/km3net-dataformat/definitions/${file}.${ext})
      add_custom_command(OUTPUT ${generated_file}
                         COMMAND csv2code
                           ${ext}
                           ${PROJECT_SOURCE_DIR}/definitions/${file}.csv
                           ${generated_file}
                           ${file}
                           ${description}
                           ${url}
                           DEPENDS ${PROJECT_SOURCE_DIR}/definitions/${file}.csv
                           )
      list(APPEND generated ${generated_file})
  endforeach()

  add_library(km3net-dataformat-definitions-${lang} INTERFACE)
  add_library(km3net::definitions-${lang} ALIAS km3net-dataformat-definitions-${lang})

  set_target_properties(km3net-dataformat-definitions-${lang} PROPERTIES
          EXPORT_NAME definitions-${lang})

  target_sources(km3net-dataformat-definitions-${lang}
          PUBLIC
          FILE_SET HEADERS
          BASE_DIRS
          ${PROJECT_BINARY_DIR}
          FILES ${generated})

  install(TARGETS km3net-dataformat-definitions-${lang}
          EXPORT km3net-dataformat
          LIBRARY
            DESTINATION ${CMAKE_INSTALL_LIBDIR}
          FILE_SET HEADERS
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endforeach()
