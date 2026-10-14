add_executable(readEvtFile readEvtFile.cc)
target_link_libraries(readEvtFile PUBLIC km3net::dataformat ROOT::RIO)
