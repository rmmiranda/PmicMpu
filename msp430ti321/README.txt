                       MSP430 C/C++ CODE GENERATION TOOLS
                              3.2.1 Release Notes
			          May 2009

================================================================================
Contents
================================================================================
1) Boot routine specialization
2) Build option reorganization
3) IAR compatibility enhancements

--------------------------------------------------------------------------------
1) Boot routine specialization
--------------------------------------------------------------------------------

The linker now performs boot routine specialization.  This optimization 
selects a specialized boot routine based on what is required.  This reduces
the number of RTS procedures that need to be included in many programs and
consequently reduces code size.  This results in a large reduction in code
size in small programs, but the significance diminishes on larger programs.

--------------------------------------------------------------------------------
2) Build option reorganization
--------------------------------------------------------------------------------

The build options have been reorganized for clarity both from the command line
and within Code Composer.  The most common options will now appear in a
separate basic category and all categories will now list the most common options
first.

--------------------------------------------------------------------------------
3) IAR compatibility enhancements
--------------------------------------------------------------------------------

Two minor changes have been made to enhance IAR compatibility.

A) The #pragma vector= now places the corresponding interrupt routine into the
   .text:_isr section for devices with extended memory.  This makes sure that 
   the routine will be located in low memory (which is necessary for correct
   operation) when the standard linker command files are used.

B) The code generated when __even_in_range is used has been improved for better
   performance.
