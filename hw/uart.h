#ifndef UART_H
#define UART_H

void init_uart(void);

void uart_tx_interrupt();
void uart_rx_interrupt();

void uart_in_string(char *buff, uint32_t buff_size);
void uart_out_string(char *buff);
void uart_out_byte(uint8_t data);

#endif // UART_H