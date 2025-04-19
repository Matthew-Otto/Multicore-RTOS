#include <stdint.h>
#include <math.h>
#include "../inc/rp2040.h"
#include "../hw/sys.h"
#include "../os/ipc.h"
#include "../os/schedule.h"

#define PERI_CLK_RATE 133000000
#define TARGET_BAUD_RATE 115200

static FIFO8_t *tx_fifo;
static FIFO8_t *rx_fifo;

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

    // init software fifos
    tx_fifo = fifo8_init(256);
    rx_fifo = fifo8_init(256);
}

void uart_tx_interrupt() {
    // WARNING: fifo get blocks when empty (dont block in an interrupt)
    while (!(UART0_UARTFR & (0x1 << 5)) && fifo8_size(tx_fifo)) {
        UART0_UARTDR = fifo8_get(tx_fifo);
    }
}

void uart_rx_interrupt() {
    while (!(UART0_UARTFR & (0x1 << 4))) {
        fifo8_put(rx_fifo, UART0_UARTDR);
    }
}

void uart_out_byte(uint8_t data) {
    // if tx_fifo is empty and hardware fifo is not full
    if (!fifo8_size(tx_fifo) && !(UART0_UARTFR & (0x1 << 5))) {
        //put directly into hardware TX buffer
        UART0_UARTDR = data;
    } else { 
        // put into software buffer
        while (fifo8_put(tx_fifo, data));
    }
}

void uart_out_string(char *buff) {
    while (*buff) {
        uart_out_byte(*buff++);
    }
}

// updates the terminal (echos back input)
void uart_in_string(char *buff, uint32_t buff_size) {
    uint32_t length = 0;
    uint8_t inchar;
    do {
        inchar = fifo8_get(rx_fifo);
        
        if (inchar == '\r') {
            uart_out_byte('\r');
            uart_out_byte('\n');
            break;
        } else if (inchar == 0x08 || inchar == 0x7F) {
            if (length) {
                buff--;
                length--;
                uart_out_byte(0x08);
                uart_out_byte(' ');
                uart_out_byte(0x08);
            }
        } else if (length < buff_size) {
            *buff++ = inchar;
            length++;
            uart_out_byte(inchar);
        }
    } while (length < buff_size);
    *buff = '\0';
}
