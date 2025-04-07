// TODO: remove

#include "sys.h"

/* reset the subsystems used in this program */
void resetSubsys(void) {
    // Reset IO Bank
    PUT32( ( RESETS_RESET | CLR ), ( 1 << 5 ) );
    while( GET32( RESETS_RESET_DONE ) & ( 1 << 5 ) == 0 );
    // Reset PADS BANK
    PUT32( ( RESETS_RESET | CLR ), ( 1 << 8 ) );
    while( GET32( RESETS_RESET_DONE ) & (1 << 8) == 0 );
    // Reset UART0
    PUT32( ( RESETS_RESET | CLR ), ( 1 << 22 ) );
    while( GET32( RESETS_RESET_DONE ) & ( 1 << 22 ) == 0 );
}


#define PUT32(address, value) (*((volatile unsigned int *)address)) = value
#define SIO_BASE 0xD0000000UL
#define SIO_GPIO_OUT_XOR (SIO_BASE + 0x1c)