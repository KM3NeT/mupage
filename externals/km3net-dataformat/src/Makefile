ifndef PACKAGE
$(error Symbol PACKAGE  not defined)
endif

ifndef INCLUDES
$(error Symbol INCLUDES not defined)
endif

ifndef LINKDEF
$(error Symbol LINKDEF  not defined)
endif

# Get ROOT pcm files
#
# param      dictionary name
# return     pcm file name (or empty if ROOT version is 5)
#
rootpcm =$(if $(subst 5,,$(ROOT_VERSION)),$(1)_rdict.pcm,)

# Convert the given set of white space separated words into a unique set of words.
#
# param      set of paths
# return     set of paths
#
unique =$(if $(1),\
        $(firstword $(1)) \
        $(call unique,$(filter-out $(firstword $(1)), $(wordlist 2, $(words $(1)), $(1)))))

DICTIONARY      := $(PACKAGE)Dictionary
LIBROOTso       := lib$(PACKAGE)ROOT.so

ROOT_VERSION     = $(shell root-config --version | sed 's/\..*//')
ROOTCINTFLAGS   += -DNAMESPACE=$(NAMESPACE) -I $(shell cd $(DIR)/..; pwd) -inlineInputHeader

CXXFLAGS        += $(shell root-config --cflags)

INCLUDES        := $(filter-out $(LINKDEF), $(INCLUDES))

NATIVE_SRCS      = $(wildcard *.cc)
NATIVE_OBJS      = $(NATIVE_SRCS:.cc=.o) $(DICTIONARY).o
NATIVE_LIBS      = $(LIBROOTso) $(call rootpcm,$(DICTIONARY))
PUBLIC_LIBS      = $(addprefix $(PUBLIC_LIB), $(NATIVE_LIBS))

DEPENDENCY_FILES = $(NATIVE_OBJS:.o=.d)


default libs: $(PUBLIC_LIBS)


# general rules

%.o: %.cc %.d
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.d: %.cc
	@printf '%s ' "$@"              >  $@
	@$(CXX) $(CXXFLAGS) -MM -MP $<  >> $@


# specific rules

$(LIBROOTso): $(NATIVE_OBJS)
	$(CXX) $(LDFLAGS) $(SOFLAGS) -o $@ $^ $(LDLIBS)

$(DICTIONARY).cc: $(INCLUDES) $(LINKDEF)
	rootcint -f $(DICTIONARY).cc $(ROOTCINTFLAGS) $(INCLUDES) $(LINKDEF)

$(DICTIONARY)_rdict.pcm: $(DICTIONARY).cc


# [de]installation rules

$(PUBLIC_LIBS): $(PUBLIC_LIB)%: %
	install -m 644 -p $^ $@

clean::
	rm -f $(DICTIONARY).* $(NATIVE_LIBS) $(NATIVE_OBJS) $(DEPENDENCY_FILES)


# inclusion of C++ compiler generated dependencies files

ifneq ("$(MAKECMDGOALS)", "clean")

.DELETE_ON_ERROR: $(DEPENDENCY_FILES)

-include $(DEPENDENCY_FILES)

endif
