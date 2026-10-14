package = io
SRC = src
LIB = lib
INC = -Iinc
F77OPT = -g -fno-automatic -c -g -fno-second-underscore -fPIC -o
C++OPT = -c -g -fPIC -o

#test if f77 compiler is available
ifeq (, $(shell which f77 2> /dev/null))
$(info F77 compiler is not available, using gfortran)
FC = gfortran
endif

OBJS = ${LIB}/io_f77.o \
       ${LIB}/filopenin.o \
       ${LIB}/filopenou.o

all : info createdir fortran77 cplusplus

fortran77 : ${LIB}/lib${package}_f77.a

cplusplus : ${LIB}/io_gcc.o

${LIB}/lib${package}_f77.a : ${OBJS}
	ar -rcv $@ ${OBJS}   

${LIB}/io_f77.o : ${SRC}/io_f77.f
	${FC} ${F77OPT} $@ ${SRC}/io_f77.f ${INC} 

${LIB}/filopenin.o : ${SRC}/filopenin.F 
	${FC} -DLinux ${F77OPT} $@ ${SRC}/filopenin.F

${LIB}/filopenou.o : ${SRC}/filopenou.f 
	${FC} ${F77OPT} $@ ${SRC}/filopenou.f

${LIB}/io_gcc.o : ${SRC}/io_gcc.cc
	${CXX} ${C++OPT} $@ ${SRC}/io_gcc.cc ${INC} 

createdir :
	@test -d "${LIB}" || (echo "Creating ${LIB}"; mkdir -p ${LIB})	
install :
	@test -f "${LIB}/lib${package}_f77.a" && \
	(echo "Now moving object files to $(ANTRS_LIB)"; \
	 mv ${LIB}/lib${package}_f77.a $(ANTRS_LIB)/.)
	@test -f "${LIB}/io_gcc.o" && \
	(echo "Now moving object files to $(ANTRS_LIB)"; \
	 mv ${LIB}/io_gcc.o $(ANTRS_LIB)/.)
clean :
	rm -f ${LIB}/*	

info : 
	@echo "Targets : fortran77 cplusplus install clean"   

