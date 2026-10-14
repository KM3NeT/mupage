add_executable(csv2code)
target_sources(csv2code PRIVATE csv2code.cc csv2cpp.cc csv2python.cc csv2julia.cc csv2shell.cc csv2csh.cc)
target_compile_features(csv2code PRIVATE cxx_std_11)

