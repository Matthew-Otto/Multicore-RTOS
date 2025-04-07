
#include <stdint.h>
#include "os/sys.h"
#include "hardware/gpio.h"
#include "inc/rp2040.h"

//void delay(int count) {
//    while (count--);
//}

int main( void )
{
    // Setup clocks (XOSC as source clk)
    init_clocks();
    // Reset Subsystems (IO / PADS and UART0)
    resetSubsys();

    gpio_init(25);


#define COUNT (12000000 / 16)                                       // with XOSC (12MHZ), 12,000,000 ticks = 1 second.
    PUT32( CORTEX_SYST_RVR, COUNT );                                // start counting down from COUNT(12,000,000/16 ticks)
    PUT32( CORTEX_SYST_CSR, ( 1 << 2 ) | ( 1 << 1 ) | ( 1 << 0 ) ); // source clock external / enable tick int / enable
    while ( 1 )
    {
        // loop here and wait for the tick interrupt
    }

    return ( 0 );
}