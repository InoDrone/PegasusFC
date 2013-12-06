SYSTYPE := $(shell uname)
export LANG=C

SRCSUFFIXES = *.cpp *.c *.S *.s

CC = $(TOOLPATH)/arm-none-eabi-gcc
CXX = $(TOOLPATH)/arm-none-eabi-g++
AR	=	$(TOOLPATH)/arm-none-eabi-ar

MAKE_DIR := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
PROJECT_ROOT  := $(realpath $(dir $(firstword $(MAKEFILE_LIST))))
PROJECT_NAME  := $(lastword $(subst /, ,$(PROJECT_ROOT)))

STM_DIR		=$(PROJECT_ROOT)/../make/stm32
LIB_DIR		=$(PROJECT_ROOT)/../lib
LIB_FILE	=$(LIB_DIR)/libpegasus.a


BUILDROOT 		= $(PROJECT_ROOT)/build
PEGASUSROOT 	= $(PROJECT_ROOT)/core/src
PEGASUSFCROOT 	= $(PROJECT_ROOT)/fc/src
PEGASUSPERIROOT = $(PROJECT_ROOT)/peripherals
INCLUDES     	= $(PROJECT_ROOT)

ifeq ($(PEGASUS_PLATFORM), PIC32)
	PEGASUSHALROOT = $(PROJECT_ROOT)/hal/pic32/src
	PEGASUSHALINC  = $(PROJECT_ROOT)/hal/pic32/include
endif

ifeq ($(PEGASUS_PLATFORM), STM32)
	PEGASUSHALROOT = $(PROJECT_ROOT)/hal/stm32f4/src
	PEGASUSHALINC  = $(PROJECT_ROOT)/hal/stm32f4/include
endif

###################################################################
#	PEGASUS CORE SOURCES / OBJS
###################################################################
PEGASUSSRCS := $(wildcard $(addprefix $(PEGASUSROOT)/,$(SRCSUFFIXES)))
PEGASUSOBJS := $(subst $(PEGASUSROOT),$(BUILDROOT),$(PEGASUSSRCS))
PEGASUSOBJS := $(addsuffix .o,$(basename $(PEGASUSOBJS)))

###################################################################
#	FLIGHT CONTROL SOURCES / OBJS
###################################################################
PEGASUSFCSRCS    := $(wildcard $(addprefix $(PEGASUSFCROOT)/,$(SRCSUFFIXES)))
PEGASUSFCOBJS   := $(subst $(PEGASUSFCROOT),$(BUILDROOT),$(PEGASUSFCSRCS))
PEGASUSFCOBJS   := $(addsuffix .o,$(basename $(PEGASUSFCOBJS)))

###################################################################
#	PEGASUS HAL SOURCES / OBJS
###################################################################
PEGASUSHALSRCS := $(wildcard $(addprefix $(PEGASUSHALROOT)/,$(SRCSUFFIXES)))
PEGASUSHALOBJS := $(subst $(PEGASUSHALROOT),$(BUILDROOT),$(PEGASUSHALSRCS))
PEGASUSHALOBJS := $(addsuffix .o,$(basename $(PEGASUSHALOBJS)))

###################################################################
#	PEGASUS PERIPHERALS / OBJS
###################################################################
PEGASUSPERISRCS := $(wildcard $(addprefix $(PEGASUSPERIROOT)/,$(SRCSUFFIXES)))
PEGASUSPERIOBJS := $(subst $(PEGASUSPERIROOT),$(BUILDROOT),$(PEGASUSPERISRCS))
PEGASUSPERIOBJS := $(addsuffix .o,$(basename $(PEGASUSPERIOBJS)))


SRCS = ${PEGASUSSRCS} ${PEGASUSHALSRCS} ${PEGASUSFCSRCS} $(PEGASUSPERISRCS)
OBJS = ${PEGASUSOBJS} ${PEGASUSHALOBJS} ${PEGASUSFCOBJS} $(PEGASUSPERIOBJS)

INCLUDES += $(STM_DIR)/lib
INCLUDES += $(STM_DIR)/lib/inc
INCLUDES += $(STM_DIR)/lib/inc/core
INCLUDES += $(STM_DIR)/lib/inc/peripherals

INCLUDES  := $(addprefix -I,$(INCLUDES))

CFLAGS  = -g -Os -Wall -DPEGASUS_STM32 -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -DSTM32F429_439xx
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4
CFLAGS += -fsingle-precision-constant -Wdouble-promotion
CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=hard
CFLAGS += -ffreestanding -nostdlib

CXXFLAGS = -std=gnu++11

.PHONY: $(LIB_FILE)
all: $(LIB_FILE)

$(LIB_FILE) : $(OBJS)
	$(AR) -r $@ $(OBJS)


$(BUILDROOT)/%.o: $(PEGASUSROOT)/%.cpp
	@echo %% $(subst $(BUILDROOT)/,,$@)
	@mkdir -p $(dir $@)
	$(CXX) -c $(DEFINES) $(INCLUDES) $(CFLAGS) $(CXXFLAGS) $^ -o $@
	
$(BUILDROOT)/%.o: $(PEGASUSROOT)/%.c
	@echo %% $(subst $(BUILDROOT)/,,$@)
	@mkdir -p $(dir $@)
	$(CC) -c $(DEFINES) $(INCLUDES) $(CFLAGS) $^ -o $@
	
$(BUILDROOT)/%.o: $(PEGASUSHALROOT)/%.cpp
	@echo %% $(subst $(BUILDROOT)/,,$@)
	@mkdir -p $(dir $@)
	$(CXX) -c $(DEFINES) $(INCLUDES) $(CFLAGS) $(CXXFLAGS) $^ -o $@
	
$(BUILDROOT)/%.o: $(PEGASUSHALROOT)/%.c
	@echo %% $(subst $(BUILDROOT)/,,$@)
	@mkdir -p $(dir $@)
	$(CC) -c $(DEFINES) $(INCLUDES) $(CFLAGS) $^ -o $@

$(BUILDROOT)/%.o: $(PEGASUSFCROOT)/%.cpp
	@echo %% $(subst $(BUILDROOT)/,,$@)
	@mkdir -p $(dir $@)
	$(CXX) -c $(DEFINES) $(INCLUDES) $(CFLAGS) $(CXXFLAGS) $^ -o $@
	
$(BUILDROOT)/%.o: $(PEGASUSFCROOT)/%.c
	@echo %% $(subst $(BUILDROOT)/,,$@)
	@mkdir -p $(dir $@)
	$(CC) -c $(DEFINES) $(INCLUDES) $(CFLAGS) $^ -o $@

$(BUILDROOT)/%.o: $(PEGASUSPERIROOT)/%.cpp
	@echo %% $(subst $(BUILDROOT)/,,$@)
	@mkdir -p $(dir $@)
	$(CXX) -c $(DEFINES) $(INCLUDES) $(CFLAGS) $(CXXFLAGS) $^ -o $@
	
$(BUILDROOT)/%.o: $(PEGASUSPERIROOT)/%.c
	@echo %% $(subst $(BUILDROOT)/,,$@)
	@mkdir -p $(dir $@)
	$(CC) -c $(DEFINES) $(INCLUDES) $(CFLAGS) $^ -o $@

clean:
	rm -rf $(BUILDROOT)/* $(LIB_FILE)
