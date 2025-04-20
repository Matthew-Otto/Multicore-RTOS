ENTRY(reset_handler)

MEMORY
{
  FLASH (rx) : ORIGIN = 0x10000000, LENGTH = 2M
  SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 256k
}

SECTIONS
{
    .boot2 :
    {
        _sboot2 = .;
        KEEP(*(.boot2))
        _eboot2 = .;
    } > FLASH

    .text : {
        KEEP(*(.vectors))
        *(.text*)
        *(.rodata*)
    } > FLASH

    .data : {
        _sdata = .;
        *(.data*)
        _edata = .;
    } > SRAM AT > FLASH

    _sidata = LOADADDR(.data);

    .bss : {
        _sbss = .;
        *(.bss*)
        *(COMMON)
        _ebss = .;
    } > SRAM

    .stack (COPY) : {
        __stack_top = ORIGIN(SRAM) + LENGTH(SRAM) - 4;
    }

    ASSERT(_eboot2 - _sboot2 == 256,
        "ERROR: Pico second stage bootloader must be 256 bytes in size")
}
