SYSTYPE := $(shell uname)
export LANG=C

SRCSUFFIXES = *.cpp *.c *.S *.s

MAKE_DIR := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
PROJECT_ROOT  := $(realpath $(dir $(firstword $(MAKEFILE_LIST))))
PROJECT_NAME  := $(lastword $(subst /, ,$(PROJECT_ROOT)))

# SOURCES
SRCROOT = $(PROJECT_ROOT)/src
PEGASUSROOT = $(PROJECT_ROOT)/src/pegasus/core/src

BUILDROOT = /tmp/$(PROJECT_NAME)

include $(PROJECT_ROOT)/config.mk
ifeq ($(PEGASUS_PLATFORM),)
$(error ERROR: could not load $(PROJECT_ROOT)/config.mk)
endif

ifeq ($(PEGASUS_PLATFORM), PIC32)
	PEGASUSHALROOT = $(PROJECT_ROOT)/src/pegasus/hal/pic32/src
	PEGASUSHALINC  = $(PROJECT_ROOT)/src/pegasus/hal/pic32/include
endif

ifeq ($(PEGASUS_PLATFORM), STM32)
	PEGASUSHALROOT = $(PROJECT_ROOT)/src/pegasus/hal/stm32f4/src
	PEGASUSHALINC  = $(PROJECT_ROOT)/src/pegasus/hal/stm32f4/include
endif


###################################################################
#	FLIGHT CONTROL SOURCES / OBJS
###################################################################
FCSRCS    := $(wildcard $(addprefix $(SRCROOT)/,$(SRCSUFFIXES)))
FCSOBJS   := $(subst $(SRCROOT),$(BUILDROOT),$(FCSRCS))
FCSOBJS   := $(addsuffix .o,$(basename $(FCSOBJS)))
FCINC     := ${SRCROOT}

###################################################################
#	PEGASUS CORE SOURCES / OBJS
###################################################################
PEGASUSSRCS := $(wildcard $(addprefix $(PEGASUSROOT)/,$(SRCSUFFIXES)))
PEGASUSOBJS := $(subst $(SRCROOT),$(BUILDROOT),$(PEGASUSSRCS))
PEGASUSOBJS := $(addsuffix .o,$(basename $(PEGASUSOBJS)))

###################################################################
#	PEGASUS HAL SOURCES / OBJS
###################################################################
PEGASUSHALSRCS := $(wildcard $(addprefix $(PEGASUSHALROOT)/,$(SRCSUFFIXES)))
PEGASUSHALOBJS := $(subst $(SRCROOT),$(BUILDROOT),$(PEGASUSHALSRCS))
PEGASUSHALOBJS := $(addsuffix .o,$(basename $(PEGASUSHALOBJS)))

###################################################################
#	SOURCES / INCLUDE
###################################################################
SRCS = ${PEGASUSSRCS} ${PEGASUSHALSRCS} ${FCSRCS}
OBJS = ${PEGASUSOBJS} ${PEGASUSHALOBJS} ${FCSOBJS}
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