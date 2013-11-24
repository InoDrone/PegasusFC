F_CPU 		:= 180000000L 
MCU 		:= STM32F429_439xx
UPLOADER 	:= dfu_util
FAMILY		:= cortex-m4

TOOLPATH=/opt/gcc-arm-embedded/bin
#TOOLPATH=/opt/sourcery/bin

STLINKDIR = /home/alienx/Documents/Electronique/stlink
STFLASH   = $(STLINKDIR)/st-flash

STM_DIR=$(PROJECT_ROOT)/make/stm32

INCLUDES += $(STM_DIR)/lib
INCLUDES += $(STM_DIR)/lib/inc
INCLUDES += $(STM_DIR)/lib/inc/core
INCLUDES += $(STM_DIR)/lib/inc/peripherals

CXX 	= $(TOOLPATH)/arm-none-eabi-g++
CC  	= $(TOOLPATH)/arm-none-eabi-gcc
OBJCOPY	= $(TOOLPATH)/arm-none-eabi-objcopy
AR		= $(TOOLPATH)/arm-none-eabi-ar

INCLUDES  := $(addprefix -I,$(INCLUDES))

DEFINES  = -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000
DEFINES += -DF_CPU=$(F_CPU) -DPEGASUS_STM32 -D$(MCU)

CFLAGS  = -g -O0 -Wall -Wextra -mthumb -mthumb-interwork -mcpu=$(FAMILY)
CFLAGS += -fsingle-precision-constant -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS += -nostdlib -ffunction-sections -fdata-sections --specs=rdimon.specs
CFLAGS += -Wl,--gc-sections  $(DEFINES)
CFLAGS += -L$(STM_DIR)/lib -lstm32f4 -Wl,--start-group -lgcc -lc -lm -lrdimon -Wl,--end-group

CXXFLAGS = -std=gnu++11
#-fno-rtti -nostdlib -Wall -mfloat-abi=hard -mfpu=fpv4-sp-d16 $(DEFINES)

#CFLAGS += -fno-exceptions -fsigned-char

LDFLAGS = -v -T$(STM_DIR)/STM32F429ZI_FLASH.ld
LDFLAGS += -mthumb -mcpu=$(FAMILY) -mfloat-abi=hard -mfpu=fpv4-sp-d16
LDFLAGS += -Wl,--gc-sections -Wl,-Map=$(PROJECTMAP)


PROJECTELF               =       $(BUILDROOT)/$(PROJECT_NAME)-STM32.elf
PROJECTBIN               =       $(BUILDROOT)/$(PROJECT_NAME)-STM32.bin
PROJECTHEX               =       $(BUILDROOT)/$(PROJECT_NAME)-STM32.hex
PROJECTMAP               =       $(BUILDROOT)/$(PROJECT_NAME)-STM32.map

vpath %.a $(STM_DIR)/lib

.PHONY: STM32
STM32: lib $(PROJECTELF) $(PROJECTHEX) $(PROJECTBIN)

.PHONY: upload
upload: $(PROJECTBIN)
	$(STFLASH) write $(PROJECTBIN)  0x08000000

lib:
	$(MAKE) -C $(STM_DIR)/lib TOOLPATH=$(TOOLPATH) MCU=$(MCU)

#$(PROJECTELF): $(SRCS)
#	@echo %% $(subst $(BUILDROOT)/,,$@)
#	@mkdir -p $(dir $@)
#   $(CXX) $(DEFS) $(INCLUDE) $(CFLAGS) $^ -o $@ -L$(STM_DIR)/lib -lstm32f4 -lm

$(PROJECTELF): $(OBJS)
	$(CXX) $(DEFINES) $(INCLUDES) $(LDFLAGS) $^ -o $@ -L$(STM_DIR)/lib -lstm32f4 -lm

$(PROJECTHEX): $(PROJECTELF)
	$(OBJCOPY) -O ihex -R .eeprom $< $@
	
$(PROJECTBIN): $(PROJECTELF)
	$(OBJCOPY) -v -O binary $< $@

$(BUILDROOT)/%.o: $(SRCROOT)/%.cpp
	@echo %% $(subst $(BUILDROOT)/,,$@)
	@mkdir -p $(dir $@)
	$(CXX) -c $(INCLUDES) $(CFLAGS) $(CXXFLAGS) $^ -o $@
	
$(BUILDROOT)/%.o: $(SRCROOT)/%.c
	@echo %% $(subst $(BUILDROOT)/,,$@)
	@mkdir -p $(dir $@)
	$(CXX) -c $(INCLUDES) $(CFLAGS) $(CXXFLAGS) $^ -o $@