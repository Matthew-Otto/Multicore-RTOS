#include <stdint.h>
#include <math.h>
#include "../inc/rp2040.h"
#include "sys.h"

#define PERI_CLK_RATE 133000000
#define TARGET_BAUD_RATE 115200

void init_uart(void) {
    init_subsystem(UART0);
    init_subsystem(IO_BANK0);

    // hookup GPIO pins to UART
    IO_GPIO0_CTRL = 0x2;
    IO_GPIO1_CTRL = 0x2;

    // see 4.2.7.1 for non-FP calculation
    double baud_rate_div = PERI_CLK_RATE/(16 * TARGET_BAUD_RATE);
    double int_part;
    double frac_part = modf(baud_rate_div, &int_part);
    
    // set UART baud rate
    UART0_UARTIBRD = (uint32_t)int_part;
    UART0_UARTFBRD = (uint32_t)((frac_part * 64) + 0.5);

    // enable UART
    UART0_UARTLCR_H = (0x3 << 5) | (0x1 << 4); // 8 bit word, enable fifo
    UART0_UARTCR = (0x1 << 9) | (0x1 << 8) | 0x1; // enable uart, tx, rx

    // enable UART Timeout, TX, RX interrupts
    UART0_UARTIMSC = (0x1 << 6) | (0x1 << 5) | (0x1 << 4);
    // enable UART interrupt in NVIC
    NVIC_ICPR = 0x1 << UART0_IRQ;
    NVIC_ISER = 0x1 << UART0_IRQ;
}

void uart_tx_interrupt(){
    UART0_UARTICR;
}

void uart_rx_interrupt(){
    while (!(UART0_UARTFR & (1 << 4))) {
        // loopback:
        UART0_UARTDR = UART0_UARTDR;
    }
}
