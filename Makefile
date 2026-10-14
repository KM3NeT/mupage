###########################################################################
# MUPAGE Makefile
# G.Carminati - carminati@bo.infn.it
# C.Distefano - distefano_c@lns.infn.it
###########################################################################
SHELL = /bin/bash

include $(PWD)/Make.config

#General config
EXTERNALS_DIR=$(PWD)/externals

C++    = g++
CXXFLAGS = -std=c++11 -g -O2 -Wall -Wno-deprecated -Wall -pedantic
RM = rm

# Detect the host OS; macOS does not support fully static binaries
UNAME_S := $(shell uname -s)

# Needed libraries and include file folders
package = mupage

# include ROOT libraries
ROOTLIBS  = `root-config --libs`
ROOTINCL  = `root-config --cflags`

# folders list
SRC     = src
INCLUDE = inc
BUILD   = lib


INCFILES  = -I$(INCLUDE)


# external libraries

#-------------------------------------------------------------------
# ROOT
#-------------------------------------------------------------------

ifeq "$(GOPT_ENABLE_KM3NET)" "YES"
    INCFILES  += $(ROOTINCL)
    LINKFILES += $(ROOTLIBS)
else
ifneq "$(UNAME_S)" "Darwin"
    CXXFLAGS += -static
endif
endif

#uuid
ifeq ($(UNAME_S),Darwin)
  UUID_LDLIBS :=
else
  UUID_LDLIBS := -luuid
endif


#-------------------------------------------------------------------
# km3net-dataformat
#-------------------------------------------------------------------

ifeq "$(GOPT_ENABLE_KM3NET)" "YES"
    INCFILES := $(INCFILES) -I$(GOPT_WITH_KM3NET_INC)
    LINKFILES  := $(LINKFILES) -L$(GOPT_WITH_KM3NET_LIB) -lKM3NeTROOT $(UUID_LDLIBS)
    CXXFLAGS := $(CXXFLAGS) -D_KM3NET_ENABLED__ 
    EXTERNALS := $(EXTERNALS) externals/km3net-dataformat
endif

#-------------------------------------------------------------------
# antares-io
#-------------------------------------------------------------------

ifeq "$(GOPT_ENABLE_ANTARES)" "YES"
    INCFILES := $(INCFILES) -I$(GOPT_WITH_ANTARES_INC)
    LINKFILES  := $(LINKFILES) $(GOPT_WITH_ANTARES_LIB)/io_gcc.o
    CXXFLAGS := $(CXXFLAGS) -D_ANTARES_ENABLED__ 
    EXTERNALS := $(EXTERNALS) externals/io
endif

#CLI11
INCFILES := $(INCFILES) -Iexternals/CLI


#--------------------------- File Dependencies ----------------------------
SOURCES:= $(wildcard $(SRC)/*.cc)
OBJECTS:= $(notdir $(SOURCES:.cc=.o))



#GIT VERSIONING
GIT_VERSION := $(shell git --no-pager describe --tags --always)
BUILD_DATE  := $(shell date "+%y%m%d")
# If working tree is dirty, append dirty flag
ifneq ($(strip $(shell git status --porcelain 2>/dev/null)),)
    GIT_VESION := $(GIT_VERSION)-D
endif


CXXFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"\
	 -DBUILD_DATE=\"$(BUILD_DATE)\"

# Build step for C++ source
%.o: $(SRC)/%.cc config
	$(C++) -c $(CXXFLAGS) $(INCFILES) -o $(BUILD)/$@ $<

all: ${package}.exe

$(EXTERNALS):
	$(MAKE) -C $@ EXTERNALS_DIR="$(EXTERNALS_DIR)" 

${package}.exe: config $(OBJECTS)
	$(C++) $(CXXFLAGS) -o ${package}.exe $(INCFILES) \
	$(addprefix $(BUILD)/,$(OBJECTS)) $(LINKFILES)

clean:
	$(RM) -rf $(BUILD) ${package}.exe

config: $(EXTERNALS)
	mkdir -p $(BUILD)

test:
	PATH="$(PWD):$$PATH" ./test.sh

clang:
	clang-format --version
	diff -u <(cat src/* inc/*) <(clang-format --style=Google src/* inc/*)
	clang-format -style=Google --Werror --dry-run src/* inc/*hh

.PHONY: all $(EXTERNALS) clang
