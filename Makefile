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

DEBUG_INTERFACE = interface/ftdi/ft232h-module-swd.cfg
DEBUG_TARGET = target/rp2040.cfg

#Flag points to the INC folder containing header files
INC = -Iinc

NAME    = c-blink
CPU     = cortex-m0plus
ARMGNU  = arm-none-eabi
AFLAGS  = --warn --fatal-warnings -mcpu=$(CPU) -g
LDFLAGS = -nostdlib
CFLAGS  = -mcpu=$(CPU) -nostartfiles -ffreestanding -g -O0 -fpic -mthumb -c
PICOTOOL = /usr/local/bin

# SRCS: all source files from src directory
C_SRCS = $(wildcard *.c) $(wildcard inc/*.c) $(wildcard os/*.c)
S_SRCS = $(wildcard *.s) $(wildcard inc/*.s) $(wildcard os/*.s)
# OBJS: list of object files
C_OBJS = $(addprefix $(BUILD),$(notdir $(C_SRCS:.c=.o)))
S_OBJS = $(addprefix $(BUILD),$(notdir $(S_SRCS:.s=.o)))
OBJS = $(C_OBJS) $(S_OBJS)
ASMS = $(addprefix $(BUILD),$(notdir $(C_SRCS:.c=.s)))




all: $(BUILD)test.elf

uf2: $(BUILD)test.bin
	picotool uf2 convert $(BUILD)test.bin $(BUILD)test.uf2 -o 0x10000000 --family rp2040

$(BUILD)test.bin: $(BUILD)test.elf
	$(OBJCOPY) -O binary $(BUILD)test.elf $(BUILD)test.bin

$(BUILD)test.elf: $(BUILD)boot2_patch.o $(OBJS)
	$(MKDIR)
	$(LD) $(LDFLAGS) -T memmap.ld -o $@ $^
	$(OBJDUMP) -D $(BUILD)test.elf > $(BUILD)test.list

# turn .c source files into object files
vpath %.c $(CWD) inc os
$(BUILD)%.o: %.c
	$(MKDIR)
	$(CC) -o $@ $^ $(INC) $(CFLAGS)


# turn .s assembly files into object files
vpath %.s $(CWD) inc os
$(BUILD)%.o: %.s
	$(MKDIR)              
	$(AS) -o $@ $^ $(INC) $(AFLAGS)


# build bootloader code
$(BUILD)boot2.bin : $(BOOTLOADER)boot2.s $(BOOTLOADER)memmap_boot2.ld
	$(MKDIR)
	$(AS) $(AFLAGS) $(BOOTLOADER)boot2.s -o $(BUILD)boot2.o
	$(LD) $(LDFLAGS) -T $(BOOTLOADER)memmap_boot2.ld $(BUILD)boot2.o -o $(BUILD)boot2.elf
	$(OBJCOPY) -O binary $(BUILD)boot2.elf $(BUILD)boot2.bin

$(BUILD)boot2_patch.o : $(BUILD)boot2.bin
	$(BOOTLOADER)pad_checksum -p 256 -s 0xFFFFFFFF $(BUILD)boot2.bin $(BUILD)boot2_patch.s
	$(AS) $(AFLAGS) $(BUILD)boot2_patch.s -o $(BUILD)boot2_patch.o


flash: $(BUILD)test.elf
	openocd -f $(DEBUG_INTERFACE) -f $(DEBUG_TARGET) -c "adapter speed 20000" -c "init" -c "reset halt" -c "program $(BUILD)test.elf 0x0 reset exit"

clean:
	rm -rf build
	rm -rf bin
