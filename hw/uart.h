#ifndef UART_H
#define UART_H

void init_uart(void);

void uart_tx_interrupt();
void uart_rx_interrupt();

#endif // UART_H