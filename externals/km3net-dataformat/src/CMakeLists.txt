add_library(km3net-dataformat)
add_library(km3net::dataformat ALIAS km3net-dataformat)

set_target_properties(km3net-dataformat PROPERTIES
  EXPORT_NAME dataformat)

target_sources(km3net-dataformat PRIVATE
  Evt.cc
  Head.cc
  JDAQ.cc
  JDAQAbstractPreamble.cc
  JDAQEvent.cc
  JDAQHit.cc
  JDAQKeyHit.cc
  JDAQModuleIdentifier.cc
  JDAQPMTIdentifier.cc
  JDAQPreamble.cc
  JDAQSummaryFrame.cc
  JDAQTimeslice.cc
  JDAQTriggeredHit.cc
  MultiHead.cc
  io.cc
  )

# the project is a bit inconsistent in the sense that the includes are
# used as e.g. #include "km3net-dataformat/online/JDAQ.hh but the
# km3net-dataformat part of the path is not part of the project
# source directory... So we recreate the "expected" structure in the
# build directory for the includes to work during the build.
# And as installation time we install under include/km3net-dataformat.
# (see below the install(TARGETS ...) command.
#
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/km3net-dataformat)
foreach(dir offline online tools)
  file(CREATE_LINK ${PROJECT_SOURCE_DIR}/${dir} ${PROJECT_BINARY_DIR}/km3net-dataformat/${dir} SYMBOLIC)
endforeach()

set(headers
  online/JDAQ.hh
  online/JDAQAbstractPreamble.hh
  online/JDAQChronometer.hh
  online/JDAQClock.hh
  online/JDAQDataTypes.hh
  online/JDAQEvent.hh
  online/JDAQEventHeader.hh
  online/JDAQException.hh
  online/JDAQFrame.hh
  online/JDAQFrameStatus.hh
  online/JDAQHeader.hh
  online/JDAQHit.hh
  online/JDAQKeyHit.hh
  online/JDAQModuleIdentifier.hh
  online/JDAQPMTIdentifier.hh
  online/JDAQPreamble.hh
  online/JDAQPrint.hh
  online/JDAQRoot.hh
  online/JDAQSummaryFrame.hh
  online/JDAQSummaryslice.hh
  online/JDAQSummarysliceHeader.hh
  online/JDAQSuperFrame.hh
  online/JDAQSuperFrameHeader.hh
  online/JDAQTimeslice.hh
  online/JDAQTimesliceHeader.hh
  online/JDAQTriggerCounter.hh
  online/JDAQTriggerMask.hh
  online/JDAQTriggeredHit.hh
  online/JDAQUTCExtended.hh
  offline/AAObject.hh
  offline/Evt.hh
  offline/Exception.hh
  offline/Head.hh
  offline/Hit.hh
  offline/MultiHead.hh
  offline/Trk.hh
  offline/Vec.hh
  offline/io_ascii.hh
  offline/io_online.hh
  tools/multiplicity.hh
  tools/reconstruction.hh
  tools/time_converter.hh
)

list(TRANSFORM headers PREPEND ${PROJECT_BINARY_DIR}/km3net-dataformat/)

target_sources(km3net-dataformat PUBLIC
        FILE_SET HEADERS
        BASE_DIRS
          ${PROJECT_BINARY_DIR}
        FILES
          ${headers}
        )


target_link_libraries(km3net-dataformat
  PUBLIC ROOT::Core uuid::uuid km3net-dataformat-definitions-cpp)

# the library name used in the original Makefile version of the build
set(libname KM3NeTROOT)

set(dictionaryFile "G__${libname}.cc")

# using the root_generate_dictonary macro provided by Root I was not able to
# properly express (at least not properly enough to survive a make -j 16 for
# instance) the dependency on generated definitions/*.hh files, so let's do it
# "by hand" using `add_custom_command`

add_custom_command(OUTPUT ${dictionaryFile} G__${libname}_rdict.pcm
  COMMAND $<TARGET_FILE:ROOT::rootcling>
    -f ${dictionaryFile}
    -inlineInputHeader
    -I${PROJECT_BINARY_DIR}
    $<$<NOT:$<PLATFORM_ID:Darwin>>:-I$<TARGET_PROPERTY:uuid::uuid,INTERFACE_INCLUDE_DIRECTORIES>>
    ${headers}
    ${PROJECT_SOURCE_DIR}/src/LinkDef.hh
  COMMAND
  ${CMAKE_COMMAND} -E copy_if_different G__${libname}_rdict.pcm $<TARGET_FILE_DIR:km3net-dataformat>/G__${libname}_rdict.pcm
  DEPENDS
    km3net::definitions-cpp
)

target_sources(km3net-dataformat PRIVATE ${dictionaryFile})

# some properties to "match" the output names used in the original
# Makefile version of the build
set_target_properties(km3net-dataformat PROPERTIES
  OUTPUT_NAME ${libname})

install(TARGETS km3net-dataformat
        EXPORT km3net-dataformat
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        FILE_SET HEADERS)

install(FILES $<TARGET_FILE_DIR:km3net-dataformat>/G__${libname}_rdict.pcm TYPE LIB)

