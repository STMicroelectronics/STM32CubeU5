/*
*****************************************************************************
** @attention
**
** Copyright (c) 2021 STMicroelectronics.
** All rights reserved.
**
** This software is licensed under terms that can be found in the LICENSE file
** in the root directory of this software component.
** If no LICENSE file comes with this software, it is provided AS-IS.
**
*****************************************************************************
*/
#include "region_defs.h"
/* Entry Point */
ENTRY(Reset_Handler)

/* Highest address of the user mode stack */
_estack = ORIGIN(RAM) + LENGTH(RAM);/* end of "RAM" Ram type memory */

_Min_Heap_Size = 0x200 ;/* required amount of heap  */
_Min_Stack_Size = 0x400 ;/* required amount of stack */

/* Memories definition */
MEMORY
{
  RAM (xrw): ORIGIN = S_DATA_START, LENGTH = S_DATA_SIZE  
  ROM (rx): ORIGIN = S_CODE_START, LENGTH = S_CODE_SIZE   
  ROM_NSC (rx): ORIGIN = CMSE_VENEER_REGION_START , LENGTH = CMSE_VENEER_REGION_SIZE   
}

/* Sections */
SECTIONS
{
  /* The startup code into "ROM" Rom type memory */
  .vectors :
  {
    . = ALIGN(8);
    KEEP(*(.vectors)) /* Startup code */
    . = ALIGN(8);
  } >ROM

  /* The program code and other data into "ROM" Rom type memory */
  .text :
  {
    . = ALIGN(8);
    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(8);
    _etext = .;        /* define a global symbols at end of code */
  } >ROM

  /* Constant data into "ROM" Rom type memory */
  .rodata :
  {
    . = ALIGN(8);
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    . = ALIGN(8);
  } >ROM

  .ARM.extab   : { 
    . = ALIGN(8);
    *(.ARM.extab* .gnu.linkonce.armextab.*)
    . = ALIGN(8);
  } >ROM
  
  .ARM : {
    . = ALIGN(8);
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
    . = ALIGN(8);
  } >ROM

  .preinit_array     :
  {
    . = ALIGN(8);
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
    . = ALIGN(8);
  } >ROM
  
  .init_array :
  {
    . = ALIGN(8);
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
    . = ALIGN(8);
  } >ROM
  
  .fini_array :
  {
    . = ALIGN(8);
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
    . = ALIGN(8);
  } >ROM

  .copy.table :
  {
        . = ALIGN(4);
        __copy_table_start__ = .;
        LONG (__etext)
        LONG (_sdata)
        LONG ((_edata - _sdata)/4)
        __copy_table_end__ = .;
   } > ROM

   .zero.table :
   {
        . = ALIGN(4);
        __zero_table_start__ = .;
        LONG (__bss_start__)
        LONG ((__bss_end__ - __bss_start__)/4)
        __zero_table_end__ = .;
    } > ROM

  /* Initialized data sections into "RAM" Ram type memory */
  __etext = .;

  .data : AT (__etext)
  {
    . = ALIGN(8);
    _sdata = .;        /* create a global symbol at data start */
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */

    . = ALIGN(8);
    _edata = .;        /* define a global symbol at data end */
    
  } >RAM 

  .gnu.sgstubs :
  {
    . = ALIGN(8);
    *(.gnu.sgstubs*)   /* Secure Gateway stubs */
    . = ALIGN(8);
  } >ROM_NSC

  /* Uninitialized data section into "RAM" Ram type memory */
  . = ALIGN(8);
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss section */
    _sbss = .;         /* define a global symbol at bss start */
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss*)
    *(COMMON)

    . = ALIGN(8);
    _ebss = .;         /* define a global symbol at bss end */
    __bss_end__ = _ebss;
  } >RAM

  .msp_stack :
  {
        . = ALIGN(8);
        . += _Min_Stack_Size;
   } > RAM
  Image$$ARM_LIB_STACK$$ZI$$Limit = ADDR(.msp_stack) + SIZEOF(.msp_stack);
  Image$$ARM_LIB_STACK$$ZI$$Base = ADDR(.msp_stack) ;
  /* User_heap_stack section, used to check that there is enough "RAM" Ram  type memory left */
  ._user_heap :
  {
    . = ALIGN(8);
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
    . = . + _Min_Heap_Size;
    . = ALIGN(8);
  } >RAM

  /* Remove information from the compiler libraries */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  .ARM.attributes 0 : { *(.ARM.attributes) }
  
  PROVIDE(__stack = Image$$ARM_LIB_STACK$$ZI$$Limit);
  PROVIDE(__StackTop = Image$$ARM_LIB_STACK$$ZI$$Limit);
  PROVIDE(__StackLimit = Image$$ARM_LIB_STACK$$ZI$$Base);
}
