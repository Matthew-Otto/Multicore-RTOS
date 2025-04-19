#include <stdint.h>
#include <string.h>
#include "../hw/uart.h"

#define TIME        1564253156U
#define RESET_TIME  1314397196U
#define STATS       1781603564U
#define HELP        946971642U

#define HELP_MESSAGE "\
### Commands:\r\n\
help:           print this message\r\n\
time:           print the current system time\r\n\
reset_time:     reset the system time\r\n\
stats:          print various OS diagnostics\r\n\
\r\n"

// FNV-1A hash
static inline uint32_t hash(const char *data) {
    uint32_t hash = 0x811c9dc5; // FNV offset basis
    while (*data) {
        hash ^= (uint8_t)*data++;
        hash *= 0x01000193; // FNV prime
    }
    return hash;
}

void interpreter(void) {
    const uint32_t buffsize = 256;
    char input_buffer[buffsize];
    uart_out_string("\x1B[2J\x1B[H");
    uart_out_string("Enter a command:\r\n");
    while (1) {
        uart_out_string("> ");
        uart_in_string(input_buffer, buffsize);

        if (*input_buffer == 0) continue;

        char *cmd = strtok(input_buffer, " ");
        char *args = input_buffer + strlen(cmd) + 1;

        switch (hash(cmd)) {
            case TIME:
                uart_out_string("idk the time yet\r\n");
                break;
            case RESET_TIME:
                uart_out_string("cant reset something idk\r\n");
                break;
            case STATS:
                uart_out_string("no stats yet\r\n");
                break;
            case HELP:
                uart_out_string(HELP_MESSAGE);
                break;
            default:
                uart_out_string("Error: unknown command.\r\n");
                break;
        }
    }
}
