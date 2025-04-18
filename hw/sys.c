#include <stdint.h>
#include "sys.h"
#include "../inc/rp2040.h"

// use XOSC+PLL as system clock source
void init_sysclock(void) {
    // init XOSC
    XOSC_STARTUP = 47; // set startup delay (47 ~= 1ms)
    XOSC_CTRL = 0xFAB000; // enable XOSC 
    while (!(XOSC_STATUS & (1<<31))); // wait for XOSC to warm up (~1ms)
    
    // configure PLL
    init_subsystem(PLL_SYS);
    PLL_SYS_FBDIV_INT = 133; // FBDIV = 133
    PLL_SYS_PWR_CLR = 0x21; // turn on main power and VCO
    while (!(PLL_SYS_CS & (1<<31))); // wait for PLL lock
    PLL_SYS_PRIM = 0x62000; // set up post dividers (POSTDIV1 = 6, POSTDIV2 = 2)
    PLL_SYS_PWR_CLR = 0x8; // turn on post dividers

    // switch reference clock source to XOSC
    CLK_REF_CTRL = 2;
    while (!CLK_REF_SELECTED); // wait for glitchless mux to switch clock source

    // switch system clock source to PLL
    CLK_SYS_CTRL = 1; // Set sysclock source to AUX (AUX set to PLL at reset)
    while (!CLK_SYS_SELECTED); // wait for glitchless mux to switch clock source
    
    // enable peripheral clock and switch source to PLL
    CLK_PERI_CTRL = (1 << 11) | (1 << 5);
    while (!CLK_PERI_SELECTED); // wait for glitchless mux to switch clock source
    
    // disable ROSC
    ROSC_CTRL = (0xd1e << 12);
}

void init_watchdog_tick(void) {
    WATCHDOG_TICK_SET = 12; // 12MHz XOSC / 12 = 1Mhz (1us) tick
    WATCHDOG_TICK_SET = (0x1 << 9); // enable watchdog
    while (!(WATCHDOG_TICK & (0x1<<10))); // wait for watchdog tick to start
}

void init_subsystem(subsystem_e sys) {
    RESET_CLR = (0x1 << sys); // pull subsystem out of reset
    while (!(RESET_DONE & (0x1 << sys))); // wait for subsystem to complete reset
}
