SYSTYPE := $(shell uname)
export LANG=C

SRCSUFFIXES = *.cpp *.c *.S *.s

MAKE_DIR := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
PROJECT_ROOT  := $(realpath $(dir $(firstword $(MAKEFILE_LIST))))
PROJECT_NAME  := $(lastword $(subst /, ,$(PROJECT_ROOT)))
SRCROOT = $(PROJECT_ROOT)/src
BUILDROOT = /tmp/$(PROJECT_NAME)

include $(PROJECT_ROOT)/config.mk
ifeq ($(PEGASUS_PLATFORM),)
$(error ERROR: could not load $(PROJECT_ROOT)/config.mk)
endif

ifneq ($(findstring pic32, $(MAKECMDGOALS)),)
	PEGASUS_PLATFORM = PIC32
endif

ifneq ($(findstring stm32, $(MAKECMDGOALS)),)
	PEGASUS_PLATFORM = STM32
endif


PROJECTSRCS := $(wildcard $(addprefix $(SRCROOT)/,$(SRCSUFFIXES)))
PROJECTOBJS := $(subst $(SRCROOT),$(BUILDROOT),$(PROJECTSRCS))
PROJECTOBJS := $(addsuffix .o,$(basename $(PROJECTOBJS)))

####
#	GET LIBRARIS IN SOURCE FILE
####
#SEXPR = 's/^[[:space:]]*\#include[[:space:]][<\"]([^>\"./]+).*$$/ID_\1/p'
#LIBTOKENS   :=    $(sort $(shell cat $(PROJECTSRCS) | sed -nEe $(SEXPR)))

LIBTOKENS	 = Pegasus Pegasus$(PEGASUS_PLATFORM)
LIBTOKENS	+= LIBRARIES

PROJECTLIBS 			:= $(wildcard $(addprefix $(SRCROOT)/libraries/,$(LIBTOKENS)))
PROJECTLIBNAMES 		:= $(notdir $(PROJECTLIBS))
PROJECTLIBSRCDIRS  		:= $(PROJECTLIBS)
PROJECTLIBSRCS      	:= $(wildcard $(foreach suffix,$(SRCSUFFIXES),$(addsuffix /$(suffix),$(PROJECTLIBSRCDIRS))))
PROJECTLIBOBJS      	:= $(addsuffix .o,$(basename $(subst $(SRCROOT),$(BUILDROOT),$(PROJECTLIBSRCS))))
PROJECTLIBINCLUDES  	:= $(addprefix -I,$(PROJECTLIBS))
PROJECTLIBSRCSRELATIVE  := $(subst $(SRCROOT)/,,$(PROJECTLIBSRCS))

ifeq ($(PEGASUS_PLATFORM), PIC32)
include $(MAKE_DIR)/platform_pic32.mk
endif

#ifeq ($(PEGASUS_PLATFORM), SMT32)
include $(MAKE_DIR)/platform_stm32.mk
#endif

all: info $(PEGASUS_PLATFORM)

info:
	@echo "****************************************************************************"
	@echo "* Make Directory			: $(MAKE_DIR)"
	@echo "* Project root			: $(PROJECT_ROOT)"
	@echo "* Sources directory		: $(SRCROOT)"
	@echo "* Build directory		: $(BUILDROOT)"
	@echo "* Platform				: $(PEGASUS_PLATFORM)"
	@echo "* Uploader				: $(UPLOADER)"
	@echo "****************************************************************************"
	
clean:
	rm -rf $(BUILDROOT)/*