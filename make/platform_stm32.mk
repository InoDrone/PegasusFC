F_CPU 		:= 168000000L 
MCU 		:= STM32F40_41xxx
UPLOADER 	:= dfu_util
FAMILY		:= cortex-m4

TOOLPATH=/opt/gcc-arm-embedded/bin
#TOOLPATH=/opt/sourcery/bin

STLINKDIR = /home/alienx/Documents/Electronique/stlink
STFLASH   = $(STLINKDIR)/st-flash

STM_DIR=$(PROJECT_ROOT)/make/stm32

INCLUDES += $(PEGASUS_DIR)
INCLUDES += $(STM_DIR)/lib
INCLUDES += $(STM_DIR)/lib/inc
INCLUDES += $(STM_DIR)/lib/inc/core
INCLUDES += $(STM_DIR)/lib/inc/peripherals
INCLUDES += $(LIB_DIR)/STM32_USB_VCP/inc
INCLUDES += $(LIB_DIR)/STM32_USB_Device_Library/Class/cdc/inc
INCLUDES += $(LIB_DIR)/STM32_USB_Device_Library/Core/inc
INCLUDES += $(LIB_DIR)/STM32_USB_OTG_Driver/inc

LIB_PREFIX = $(STM_DIR)/arm-none-eabi/lib/thumb/cortex-m4/float-abi-hard/fpuv4-sp-d16/

CXX 	= $(TOOLPATH)/arm-none-eabi-g++
CC  	= $(TOOLPATH)/arm-none-eabi-gcc
OBJCOPY	= $(TOOLPATH)/arm-none-eabi-objcopy
AR		= $(TOOLPATH)/arm-none-eabi-ar
OD      = $(TOOLPATH)/arm-none-eabi-objdump

INCLUDES  := $(addprefix -I,$(INCLUDES))

DEFINES  = -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000
DEFINES += -DF_CPU=$(F_CPU) -DPEGASUS_STM32 -D$(MCU)

#CFLAGS  = -g -Os -Wall -Wextra -mthumb -mthumb-interwork -mcpu=$(FAMILY)
CFLAGS  = -g -O2 -Wall -Wextra -Wabi -mthumb -mcpu=$(FAMILY)
#CFLAGS += -fsingle-precision-constant -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS +=  -fsingle-precision-constant -ffunction-sections -fdata-sections -mfloat-abi=hard -mfpu=fpv4-sp-d16 
#CFLAGS += -L$(STM_DIR)/lib -lstm32f4
#-nostdlib
CXXFLAGS = -std=gnu++11
#-Wl,--start-group -lgcc -lc -lm -Wl,--end-group
#-fno-rtti -nostdlib -Wall -mfloat-abi=hard -mfpu=fpv4-sp-d16 $(DEFINES)

#CFLAGS += -fno-exceptions -fsigned-char

#LDFLAGS =  -T$(STM_DIR)/STM32F429ZI_FLASH.ld
#-nostartfiles
LDFLAGS =  -T$(STM_DIR)/STM32F4XX.ld
LDFLAGS += -mthumb -mcpu=$(FAMILY) -mfloat-abi=hard -mfpu=fpv4-sp-d16
LDFLAGS += -Wl,--gc-sections -Wl,-Map,$(PROJECTMAP) -Wl,--cref -L${LIB_PREFIX}



PROJECTELF               =       $(BUILDROOT)/$(PROJECT_NAME)-STM32.elf
PROJECTBIN               =       $(BUILDROOT)/$(PROJECT_NAME)-STM32.bin
PROJECTHEX               =       $(BUILDROOT)/$(PROJECT_NAME)-STM32.hex
PROJECTMAP               =       $(BUILDROOT)/$(PROJECT_NAME)-STM32.map
PROJECTLST             =       $(BUILDROOT)/$(PROJECT_NAME)-STM32.lst

vpath %.a $(STM_DIR)/lib
vpath %.a $(LIB_DIR)

.PHONY: STM32
STM32: libSTM32 libPegasus $(PROJECTELF) $(PROJECTHEX) $(PROJECTBIN)

.PHONY: upload
upload: $(PROJECTBIN)
	$(STFLASH) write $(PROJECTBIN)  0x08000000


libPegasus:
	$(MAKE) -C $(PEGASUS_DIR) TOOLPATH=$(TOOLPATH) PEGASUS_PLATFORM=STM32
	
libSTM32:
	$(MAKE) -C $(STM_DIR)/lib TOOLPATH=$(TOOLPATH) MCU=$(MCU)

#$(PROJECTELF): $(SRCS)
#	@echo %% $(subst $(BUILDROOT)/,,$@)
#	@mkdir -p $(dir $@)
#   $(CXX) $(DEFS) $(INCLUDE) $(CFLAGS) $^ -o $@ -L$(STM_DIR)/lib -lstm32f4 -lm

$(PROJECTELF): $(OBJS)
	$(eval PEGASUSLIB_OBJ := $(shell find $(PEGASUS_DIR)/build/lib/ -type f -name '*.o'))
	$(eval PEGASUS_OBJ := $(wildcard $(PEGASUS_DIR)/build/*.o))
	$(CXX) $(DEFINES) $(INCLUDES) $(LDFLAGS) $(PEGASUSLIB_OBJ) $(PEGASUS_OBJ) $^ -o $@ -lgcc -lm -lc

$(PROJECTHEX): $(PROJECTELF)
	$(OBJCOPY) -O ihex -R .eeprom $< $@
	$(OD) -S $(PROJECTELF) > $(PROJECTLST)
	
$(PROJECTBIN): $(PROJECTELF)
	$(OBJCOPY) -v -O binary $< $@

$(BUILDROOT)/%.o: $(SRCROOT)/%.cpp
	@echo %% $(subst $(BUILDROOT)/,,$@)
	@mkdir -p $(dir $@)
	$(CXX) -c $(DEFINES) $(INCLUDES) $(CFLAGS) $(CXXFLAGS) $^ -o $@
	
$(BUILDROOT)/%.o: $(SRCROOT)/%.c
	@echo %% $(subst $(BUILDROOT)/,,$@)
	@mkdir -p $(dir $@)
	$(CXX) -c $(DEFINES) $(INCLUDES) $(CFLAGS) $^ -o $@
	

clean:
	$(MAKE) -C $(PEGASUS_DIR) clean
	rm -rf $(BUILDROOT)/*