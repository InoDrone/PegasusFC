F_CPU 		:= 80000000L 
MCU 		:= 32MX795F512L
UPLOADER 	:= $(shell which avrdude)

ifeq ($(PIC32_COMPILER), PIC32)
TOOLPATH := /home/alienx/Dropbox/Drone/MPIDE/mpide-0023-linux-20120903/hardware/pic32/compiler/pic32-tools/bin/

CC 		= $(addprefix $(TOOLPATH),pic32-gcc)
CXX 	= $(addprefix $(TOOLPATH),pic32-g++)
LD 	= 	  $(addprefix $(TOOLPATH),pic32-g++)
AR 		= $(addprefix $(TOOLPATH),pic32-ar)
OBJDUMP = $(addprefix $(TOOLPATH),pic32-objdump)
OBJCOPY = $(addprefix $(TOOLPATH),pic32-objcopy)
BINTOHEX = $(addprefix $(TOOLPATH), pic32-bin2hex)
else
TOOLPATH := /opt/microchip/xc32/v1.21/bin/

CC 		= $(addprefix $(TOOLPATH),xc32-gcc)
CXX 	= $(addprefix $(TOOLPATH),xc32-g++)
LD 	= 	  $(addprefix $(TOOLPATH),xc32-g++)
AR 		= $(addprefix $(TOOLPATH),xc32-ar)
OBJDUMP = $(addprefix $(TOOLPATH),xc32-objdump)
OBJCOPY = $(addprefix $(TOOLPATH),xc32-objcopy)
BINTOHEX = $(addprefix $(TOOLPATH), xc32-bin2hex)
endif


CPUFLAGS = -mprocessor=$(MCU) -DMCU_$(MCU)
DEFINES = -DF_CPU=$(F_CPU) -DPEGASUS_PIC32

CXXOPTS = $(CPUFLAGS) -w -Wall -fno-exceptions -ffunction-sections -fdata-sections -mno-smart-io -fno-short-double -O2 $(DEFINES)
LDOPTS = -mprocessor=$(MCU) -lm -Wl,--gc-sections -Os -T$(PROJECT_ROOT)/make/pic32/chipKIT-MAX32-application-32MX795F512L.ld


LIBS = -lm -lc

PROJECT_INCLUDE = $(PROJECTLIBINCLUDES)

PROJECTELF               =       $(BUILDROOT)/$(PROJECT_NAME).elf
PROJECTEEP				 =       $(BUILDROOT)/$(PROJECT_NAME).eep
PROJECTHEX               =       $(BUILDROOT)/$(PROJECT_NAME).hex
PROJECTLIB				 =       $(BUILDROOT)/$(PROJECT_NAME).a 
ALLOBJS                  =       $(PROJECTOBJS) $(PROJECTLIBOBJS)
ALLDEPS                  =       $(ALLOBJS:%.o=%.d)

PIC32: $(PROJECTELF) $(PROJECTEEP) $(PROJECTHEX)


.PHONY: upload
upload: reset raw_upload
	

raw_upload: $(PROJECTHEX)
	$(UPLOADER) -q −V -c stk500v2 -b 115200 -p $(MCU) -P $(PORT) -C $(MAKE_DIR)/pic32/avrdude.conf -U flash:w:$(PROJECTHEX):i
	
reset:
		@if [ -z "$(PORT)" ]; then \
			echo "No Arduino-compatible TTY device found -- exiting"; exit 2; \
			fi
		for STTYF in 'stty --file' ; \
		  do $$STTYF /dev/tty >/dev/null 2>/dev/null && break ; \
		done ;\
		$$STTYF $(PORT)  hupcl ;\
		(sleep 0.1 || sleep 1)     ;\
		$$STTYF $(PORT) -hupcl

-include $(ALLDEPS)

$(PROJECTELF): $(PROJECTOBJS) $(PROJECTLIBOBJS)
	$(CXX) $(LDOPTS) -o $@ $^ $(LIBS)

#$(PROJECTLIB): $(PROJECTOBJS) $(PROJECTLIBOBJS)
#	$(AR) rcs $@ $^

$(PROJECTEEP): $(PROJECTELF)
	$(OBJCOPY) -O  ihex  -j  .eeprom  --set-section-flags=.eeprom=alloc,load  --no-change-warnings  --change-section-lma  .eeprom=0 $< $@

#$(PROJECTHEX): $(PROJECTELF)
#	$(OBJCOPY) -O ihex -R .eeprom $< $@

$(PROJECTHEX): $(PROJECTELF)
	$(BINTOHEX) -a $<


$(BUILDROOT)/%.o: $(SRCROOT)/%.cpp
	@echo %% $(subst $(BUILDROOT)/,,$@)
	@mkdir -p $(dir $@)
	$(CXX) -c $(CXXOPTS) -o $@ $< $(PROJECT_INCLUDE)
	
$(BUILDROOT)/%.o: $(SRCROOT)/%.c
	@echo %% $(subst $(BUILDROOT)/,,$@)
	@mkdir -p $(dir $@)
	$(CC) -c $(CXXOPTS) -o $@ $< $(PROJECT_INCLUDE)
	
$(BUILDROOT)/libraries/%.o: $(SRCROOT)/libraries/%.cpp
	@echo %% $(subst $(BUILDROOT)/,,$@)
	@mkdir -p $(dir $@)
	$(CXX) -c $(CXXOPTS) -o $@ $< $(PROJECT_INCLUDE)