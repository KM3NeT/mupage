CSV_FILES     :=  $(wildcard *.csv)
CPP_FILES     :=  $(CSV_FILES:.csv=.hh)
PYTHON_FILES  :=  $(CSV_FILES:.csv=.py)
JULIA_FILES   :=  $(CSV_FILES:.csv=.jl)
ZSH_FILES     :=  $(CSV_FILES:.csv=.sh)
CSH_FILES     :=  $(CSV_FILES:.csv=.csh)

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

export URL           := https://git.km3net.de/common/km3net-dataformat
export DESCRIPTION   := KM3NeT Data Definitions $(GIT_VERSION)


default libs: $(CPP_FILES) $(PYTHON_FILES) $(JULIA_FILES) $(ZSH_FILES) $(CSH_FILES)


$(CPP_FILES): %.hh: %.csv
	@echo Generating $@ from $<
	@echo "#ifndef __$(basename $<)__"                                >  $@
	@echo "#define __$(basename $<)__"                                >> $@
	@echo                                                             >> $@
	@echo "/**"                                                       >> $@
	@echo " * \file"                                                  >> $@
	@echo " * $(DESCRIPTION)"                                         >> $@
	@echo " * $(URL)"                                                 >> $@
	@echo " */"                                                       >> $@
	@echo                                                             >> $@
	@echo "/**"                                                       >> $@
	@echo " * $(basename $<)"                                         >> $@
	@echo " */"                                                       >> $@
	@grep -v '^#' $< | grep , \
	    | awk -F, '{ sub(/^ */,"",$$4); printf "static const %s %-32s = %-10s %s\n",\
	               $$1, $$2, $$3";", "//!< "$$4}'                     >> $@
	@echo "#endif"                                                    >> $@


$(PYTHON_FILES): %.py: %.csv
	@echo Generating $@ from $<
	@echo "# -*- coding: utf-8 -*-"                                   >  $@
	@echo '"""'                                                       >> $@
	@echo $(DESCRIPTION)                                              >> $@
	@echo $(URL)                                                      >> $@
	@echo '"""'                                                       >> $@
	@echo                                                             >> $@
	@echo "# $(basename $<)"                                          >> $@
	@echo "data = dict("                                              >> $@
	@grep -v '^#' $< | grep , \
	    | awk -F, '{printf "    %s=%s,  # %s\n", $$2, $$3, $$4}'      >> $@
	@echo ")"                                                         >> $@


$(JULIA_FILES): %.jl: %.csv
	@echo Generating $@ from $<
	@echo '"""'                                                       >  $@
	@echo '# $(DESCRIPTION)'                                          >> $@
	@echo $(URL)                                                      >> $@
	@echo '"""'                                                       >> $@
	@echo                                                             >> $@
	@echo module $(basename $(shell echo $< | tr '[a-z]' '[A-Z]' ))   >> $@
	@grep -v '^#' $< | grep , \
	    | awk -F, '{printf "  const %s = %s  # %s\n", $$2, $$3, $$4}' >> $@
	@echo "end"                                                       >> $@


$(ZSH_FILES): %.sh: %.csv
	@echo Generating $@ from $<
	@echo '#!/usr/bin/env zsh'                                        >  $@
	@echo '# $(DESCRIPTION)'                                          >> $@
	@echo '# $(URL)'                                                  >> $@
	@echo                                                             >> $@
	@grep -v '^#' $< | grep , \
	    | awk -F, '{gsub(/^[ \t]+/,"", $$3); \
	                gsub(/[ \t]+$$/,"",$$3); \
                        printf "export %s=%s  # %s\n", $$2, $$3, $$4}'>> $@


$(CSH_FILES): %.csh: %.csv
	@echo Generating $@ from $<
	@echo '#!/usr/bin/env csh'                                        >  $@
	@echo '# $(DESCRIPTION)'                                          >> $@
	@echo '# $(URL)'                                                  >> $@
	@echo                                                             >> $@
	@grep -v '^#' $< | grep , \
	    | awk -F, '{gsub(/^[ \t]+/,"", $$3); \
	                gsub(/[ \t]+$$/,"",$$3); \
                        printf "setenv %s %s  # %s\n", $$2, $$3, $$4}'>> $@

clean::
	rm -f $(CPP_FILES) $(PYTHON_FILES) $(JULIA_FILES) $(ZSH_FILES)
