# General

export URL           := https://git.km3net.de/common/data-format
export NAMESPACE     := KM3NET
export DIR           := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
export PUBLIC_LIB    := $(DIR)/lib/
export DEF_DIR       := definitions
export TEST_DIR      := tests



# GIT 

GIT_EXITCODE  := $(shell git status > /dev/null 2>&1; echo $$?)

ifeq ($(GIT_EXITCODE),0)
  GIT_VERSION   := $(shell git --no-pager describe --tags --always)
  # If working tree is dirty, append dirty flag
  ifneq ($(strip $(shell git status --porcelain 2>/dev/null)),)
    GIT_VERSION := $(GIT_VERSION)-D
  endif
else
  GIT_VERSION   := unknown version
endif

export DESCRIPTION   := KM3NeT Data Definitions $(GIT_VERSION)

# ROOT library

export PACKAGE       := KM3NeT
export LINKDEF       := $(DIR)/src/LinkDef.hh
export INCLUDES      := $(foreach dir,$(DIR)/online $(DIR)/offline $(DIR)/tools, $(wildcard $(dir)/*.hh))

# Compiler

export CXX           ?= g++
export CXXFLAGS      += -Wall -ansi -fPIC -std=c++17 -O3 -I $(shell cd $(DIR)/..; pwd)
export SOFLAGS       += -O -shared
export LDLIBS        += $(shell root-config --libs)


default libs clean:: | $(PUBLIC_LIB)
	@echo KM3NeT Data format $(GIT_VERSION) / namespace $(NAMESPACE)
	$(MAKE) $(MFLAGS) -C $(DEF_DIR)   $@
	$(MAKE) $(MFLAGS) -C $(DIR)/src/  $@


$(PUBLIC_LIB)::
	mkdir -p $@


HEAD_BEHAVIOR_TEST := $(TEST_DIR)/head_behavior_test
HEAD_BEHAVIOR_TEST_CXXFLAGS := $(CXXFLAGS) $(shell root-config --cflags)
# The legacy shared library links to libuuid, so the standalone test needs it too.
HEAD_BEHAVIOR_TEST_LDFLAGS := $(shell root-config --libs) -L$(PUBLIC_LIB) -l$(PACKAGE)ROOT -luuid

$(HEAD_BEHAVIOR_TEST): $(TEST_DIR)/test_head.cc default libs
	$(CXX) $(HEAD_BEHAVIOR_TEST_CXXFLAGS) -o $@ $< $(HEAD_BEHAVIOR_TEST_LDFLAGS)

test:: $(HEAD_BEHAVIOR_TEST)
	LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):$(PWD)/lib $(HEAD_BEHAVIOR_TEST)


clean::
	rm -rf $(PUBLIC_LIB)
	rm -f $(HEAD_BEHAVIOR_TEST)
