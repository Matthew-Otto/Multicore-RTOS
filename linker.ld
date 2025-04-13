ENTRY(reset_handler)

MEMORY
{
  FLASH (rx) : ORIGIN = 0x10000000, LENGTH = 2M
  SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 255k
}

SECTIONS
{
  .boot2 :
  {
      __boot2_start__ = .;
      KEEP(*(.boot2))
      __boot2_end__ = .;
  } > FLASH

  .vectors : {
    KEEP(*(.vectors))
  } > SRAM AT > FLASH

  .text : {
    *(.text*)
    *(.rodata*)
  } > SRAM AT > FLASH

  .data : {
    _sdata = .;
    *(.data*)
    _edata = .;
  } > SRAM

  _sidata = LOADADDR(.data);

  .bss : {
    _sbss = .;
    *(.bss*)
    *(COMMON)
    _ebss = .;
  } > SRAM

  .stack (COPY) : {
    __stack_top = ORIGIN(SRAM) + LENGTH(SRAM);
  }

  ASSERT(__boot2_end__ - __boot2_start__ == 256,
    "ERROR: Pico second stage bootloader must be 256 bytes in size")
}
