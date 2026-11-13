/* @TODO(M1-02/5) Line 1: ENTRY(Reset_Handler). Then fill the two MEMORY lines (FLASH rx at 0x08000000, 32K; RAM rwx at 0x20000000, 10K). [src: M1-02 step 5] */

ENTRY(Reset_Handler)
MEMORY
{
  FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 32K
  RAM  (rwx) : ORIGIN = 0x20000000, LENGTH = 10K
}

/* @TODO(M1-02/6) _estack = the first address AFTER RAM = ORIGIN(RAM) + LENGTH(RAM). [src: M1-02 step 6] */
_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
  .isr_vector :
  {
    KEEP(*(.isr_vector))
  } > FLASH

/* @TODO(M1-02/7) .text section: code and read-only data, live in FLASH. Put *(.text*) and *(.rodata*) inside, then ALIGN(4). [src: M1-02 step 7] */
  .text :
  {
    *(.text*)
    *(.rodata*)
    . = ALIGN(4);
  } > FLASH

/* @TODO(M1-02/8) .data section: lives in RAM but is STORED in FLASH (> RAM AT> FLASH). Mark _sdata at the start and _edata at the end; _sidata = LOADADDR(.data). [src: M1-02 step 8] */
  _sidata = LOADADDR(.data);
  .data : 
  {
    . = ALIGN(4);
    _sdata = .;
    *(.data*)
    . = ALIGN(4);
    _edata = .;
  } > RAM AT> FLASH


/* @TODO(M1-02/9) .bss section: RAM only. Mark _sbss at the start and _ebss at the end; include *(.bss*) and *(COMMON). [src: M1-02 step 9] */
  .bss :
  {
    . = ALIGN(4);
    _sbss = .;
    *(.bss*)
    . = ALIGN(4);
    _ebss = .;
  } > RAM 

  /DISCARD/ : { *(.ARM.exidx*) *(.comment) }
}
