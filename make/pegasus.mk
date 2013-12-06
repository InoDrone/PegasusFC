SYSTYPE := $(shell uname)
export LANG=C

SRCSUFFIXES = *.cpp *.c *.S *.s

MAKE_DIR := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
PROJECT_ROOT  := $(realpath $(dir $(firstword $(MAKEFILE_LIST))))
PROJECT_NAME  := $(lastword $(subst /, ,$(PROJECT_ROOT)))

# SOURCES
SRCROOT 		= $(PROJECT_ROOT)/src
PEGASUS_DIR     = $(PROJECT_ROOT)/pegasus
LIB_DIR     	= $(PROJECT_ROOT)/lib
PEGASUS_LIB     = $(LIB_DIR)/libpegasus.a

BUILDROOT = /tmp/$(PROJECT_NAME)

include $(PROJECT_ROOT)/config.mk
ifeq ($(PEGASUS_PLATFORM),)
$(error ERROR: could not load $(PROJECT_ROOT)/config.mk)
endif


###################################################################
#	FLIGHT CONTROL SOURCES / OBJS
###################################################################
FCSRCS    := $(wildcard $(addprefix $(SRCROOT)/,$(SRCSUFFIXES)))
FCSOBJS   := $(subst $(SRCROOT),$(BUILDROOT),$(FCSRCS))
FCSOBJS   := $(addsuffix .o,$(basename $(FCSOBJS)))
FCINC     := ${SRCROOT}

###################################################################
#	SOURCES / INCLUDE
###################################################################
SRCS = ${FCSRCS}  $(SRCROOT)/platform/src/Pegasus$(PEGASUS_PLATFORM)$(PEGASUS_VERSION).cpp
OBJS = ${FCSOBJS} $(BUILDROOT)/platform/src/Pegasus$(PEGASUS_PLATFORM)$(PEGASUS_VERSION).o
INCLUDES = ${FCINC}

ifeq ($(PEGASUS_PLATFORM),PIC32)
include $(MAKE_DIR)/platform_pic32.mk
endif

ifeq ($(PEGASUS_PLATFORM),STM32)
include $(MAKE_DIR)/platform_stm32.mk
endif

check:
	@echo $(SRCS)
	@echo $(OBJS)

all: clean info $(PEGASUS_PLATFORM)

info:
	@echo "****************************************************************************"
	@echo "* Make Directory			: $(MAKE_DIR)"
	@echo "* Project root			: $(PROJECT_ROOT)"
	@echo "* Sources directory		: $(SRCROOT)"
	@echo "* Build directory		: $(BUILDROOT)"
	@echo "* Platform				: $(PEGASUS_PLATFORM)"
	@echo "* Uploader				: $(UPLOADER)"
	@echo "****************************************************************************"
