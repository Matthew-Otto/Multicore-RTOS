BOOTLOADER = ./bootloader/
BUILD = ./build/
BIN = ./bin/

#UTILITY VARIABLES
AS = arm-none-eabi-as  #assembler
CC = arm-none-eabi-gcc #compiler
LD = arm-none-eabi-ld #linker
OBJCOPY = arm-none-eabi-objcopy #final executable builder
OBJDUMP = arm-none-eabi-objdump
RM      = rm -rf
MKDIR   = @mkdir -p $(@D) #creates folders if not present

DEBUG_INTERFACE = interface/cmsis-dap.cfg
#DEBUG_INTERFACE = interface/ftdi/ft232h-module-swd.cfg
DEBUG_TARGET = target/rp2040.cfg

#Flag points to the INC folder containing header files
INC = -Iinc

ARMGNU  = arm-none-eabi
AFLAGS  = --warn --fatal-warnings -mthumb -mcpu=cortex-m0plus -g
LDFLAGS = -Llib/ -lc -lgcc --gc-sections -Map=$(BUILD)main.map --print-memory-usage
CFLAGS  = -mthumb -mcpu=cortex-m0plus -nostartfiles -ffreestanding -g -c -ggdb -std=c99 -O0 #-Og
PICOTOOL = /usr/local/bin


# SRCS: all source files from src directory
VPATH = . lib inc os hw benchmarks
H_SRCS = inc/rp2040.h lib/utils.h hw/hwctrl.h
C_SRCS = $(wildcard *.c) $(wildcard inc/*.c) $(wildcard os/*.c) $(wildcard hw/*.c) $(wildcard benchmarks/*.c)
S_SRCS = $(wildcard *.s) $(wildcard inc/*.s) $(wildcard os/*.s) $(wildcard hw/*.s)
# OBJS: list of object files
C_OBJS = $(addprefix $(BUILD),$(notdir $(C_SRCS:.c=.o)))
S_OBJS = $(addprefix $(BUILD),$(notdir $(S_SRCS:.s=.o)))
OBJS = $(C_OBJS) $(S_OBJS)
ASMS = $(addprefix $(BUILD),$(notdir $(C_SRCS:.c=.s)))



all: $(BUILD)main.elf

uf2: $(BUILD)main.elf
	picotool uf2 convert $(BUILD)main.elf $(BUILD)main.uf2 --family rp2040

$(BUILD)main.bin: $(BUILD)main.elf
	$(OBJCOPY) -O binary $(BUILD)main.elf $(BUILD)main.bin

$(BUILD)main.elf: $(BUILD)boot2_patch.o $(OBJS)
	$(MKDIR)
	$(LD) -T linker.ld -o $@ $^ $(LDFLAGS)
	$(OBJDUMP) -D $(BUILD)main.elf > $(BUILD)main.list

# turn .c source files into object files
$(BUILD)%.o: %.c | $(H_SRCS)
	$(MKDIR)
	$(CC) -o $@ $^ $(INC) $(CFLAGS)


# turn .s assembly files into object files
vpath %.s $(CWD) inc os hw
$(BUILD)%.o: %.s
	$(MKDIR)              
	$(AS) -o $@ $^ $(INC) $(AFLAGS)

# build bootloader code
$(BUILD)boot2_patch.o : $(BUILD)boot2.bin
	$(BOOTLOADER)pad_checksum -p 256 -s 0xFFFFFFFF $(BUILD)boot2.bin $(BUILD)boot2_patch.s
	$(AS) $(AFLAGS) $(BUILD)boot2_patch.s -o $(BUILD)boot2_patch.o

$(BUILD)boot2.bin : $(BOOTLOADER)boot2.c
	$(MKDIR)
	$(CC) -nostdlib -mcpu=cortex-m0plus -mthumb -g -O1 $(BOOTLOADER)boot2.c -o $(BUILD)boot2.o
	$(ARMGNU)-objcopy -O binary $(BUILD)boot2.o $(BUILD)boot2.bin


flash: $(BUILD)main.elf
	openocd -f $(DEBUG_INTERFACE) -f $(DEBUG_TARGET) -c "adapter speed 5000" -c "program $(BUILD)main.elf reset; reset halt; rp2040.core1 arp_reset assert 0; rp2040.core0 arp_reset assert 0; exit"

debug: $(BUILD)main.elf
	openocd -f $(DEBUG_INTERFACE) -f $(DEBUG_TARGET) -c "adapter speed 5000"

reset:
	openocd -f $(DEBUG_INTERFACE) -f $(DEBUG_TARGET) -c "adapter speed 5000" -c "init; reset halt; rp2040.core1 arp_reset assert 0; rp2040.core0 arp_reset assert 0; exit"

clean:
	rm -rf build
	rm -rf bin
