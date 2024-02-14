TARGET       = PCAN_USB
all:
OBJECT_FILES += $(addsuffix .o, $(basename $(TARGET).c))
OBJECT_FILES    := $(addprefix $(patsubst %/,%,$(OBJDIR))/, $(notdir $(OBJECT_FILES)))
clean: 
	rm -f $(OBJECT_FILES:%.o=%.d)
	rm -f $(TARGET).elf $(TARGET).hex $(TARGET).lss 
all: elf hex lss
elf: $(TARGET).elf
hex: $(TARGET).hex 
lss: $(TARGET).lss
$(OBJDIR)/%.o: %.c 
	avr-gcc -c -pipe -mmcu=atmega16u2 -Wall -fno-jump-tables -Os  -MMD -MF $(@:%.o=%.d) $< -o $@
%.elf: $(OBJECT_FILES)
	avr-gcc $^ -o $@ -lm -Wl,--cref -Wl,--gc-sections -Wl,--relax -mmcu=atmega16u2
%.hex: %.elf
	avr-objcopy -O ihex -R .eeprom -R .fuse -R .lock -R .signature $< $@
%.lss: %.elf
	avr-objdump -h -d -S -z $< > $@
