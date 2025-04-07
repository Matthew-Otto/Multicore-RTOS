#include "../inc/rp2040.h"


// use XOSC as system clock source
void init_sysclock(void) {
        //// Enable the XOSC
        //PUT32( XOSC_CTRL, 0xAA0 );                        // Frequency range: 1_15MHZ
        //PUT32( XOSC_STARTUP, 0xc4 );                      // Startup delay ( default value )
        //PUT32( ( XOSC_CTRL | SET ), 0xFAB000 ) ;          // Enable ( magic word )
        //while ( !( GET32( XOSC_STATUS ) & 0x80000000 ) ); // Oscillator is running and stable

        //// Set the XOSC as source clock for REF, SYS and Periferals
        //PUT32( CLK_REF_CTRL, 2 );                          // CLK REF source = xosc_clksrc
        //PUT32( CLK_SYS_CTRL, 0 );                          // CLK SYS source = clk_ref
        //PUT32( CLK_REF_DIV, ( 1 << 8 ) );                  // CLK REF Divisor = 1
        //PUT32( CLK_PERI_CTRL, ( 1 << 11 ) | ( 4 << 5 ) );  // CLK PERI Enable & AUX SRC = xosc_clksrc
}