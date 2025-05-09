#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "../hw/uart.h"
#include "../hw/timer.h"
#include "../os/schedule.h"

#define TIME        1564253156U
#define STATS       1781603564U
#define SLEEP       2313861896U
#define HELP        946971642U

#define HELP_MESSAGE "\
### Commands:\r\n\
help:           print this message\r\n\
time [reset]:   print (or reset) the current system time\r\n\
stats:          print various OS diagnostics\r\n\
sleep <ms>:     sleeps interpreter thread for specified ms\r\n\
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
        memset(input_buffer, 0, sizeof(input_buffer));
        uart_out_string("> ");
        uart_in_string(input_buffer, buffsize);

        if (*input_buffer == 0) continue;

        char *cmd = strtok(input_buffer, " ");
        char *args = input_buffer + strlen(cmd) + 1;

        switch (hash(cmd)) {
            case TIME: {
                char s[20];
                int32_t argcount = sscanf(args, "%s", &s);
                if (argcount <= 0){
                    uint32_t time = get_mstime();
                    snprintf(input_buffer, buffsize, "System uptime: %d ms\r\n", time);
                    uart_out_string(input_buffer);
                    break;
                } else if (argcount == 1) {
                    if (hash(s) == 1695364032U) {
                        clear_mstime();
                        uart_out_string("System time reset.\r\n");
                        break;
                    }
                    uart_out_string("Error: invalid argument\r\n");
                    break;
                }
                uart_out_string("Error: invalid number of arguments\r\n");
                    break;
            }

            case STATS: {
                // cpu usage
                uint32_t cpu0_idle = get_idle_percentage(0);
                uint32_t cpu1_idle = get_idle_percentage(1);
                snprintf(input_buffer, buffsize, "CPU0 idle percentage: %d.%d\r\nCPU1 idle percentage: %d.%d\r\n",
                        cpu0_idle/10, cpu0_idle%10, cpu1_idle/10, cpu1_idle%10);
                uart_out_string(input_buffer);
                // scheduler stats
                scheduler_stats_t sstats;
                scheduler_stats(&sstats);
                snprintf(input_buffer, buffsize, "\r\nRunning Threads: %d\r\nActive Threads: %d\r\nLifetime Threads: %d\r\n", 
                    sstats.running_thread_cnt, sstats.active_thread_cnt, sstats.lifetime_thread_cnt);
                uart_out_string(input_buffer);
                // heap stats
                heap_stats_t hstats;
                heap_stats(&hstats);
                snprintf(input_buffer, buffsize, "\r\nHeap size: %d\r\nHeap used: %d\r\nHeap free: %d\r\n", 
                    hstats.size, hstats.used, hstats.free);
                uart_out_string(input_buffer);
                break;
            }

            case SLEEP: {
                uint32_t sleep_time;
                if(sscanf(args, "%d", &sleep_time) != 1){
                    uart_out_string("Error: invalid number of arguments\r\n");
                    break;
                }
                snprintf(input_buffer, buffsize, "Sleeping for: %d ms.\r\n", sleep_time);
                uart_out_string(input_buffer);
                sleep(sleep_time);
                break;
            }

            case HELP:
                uart_out_string(HELP_MESSAGE);
                break;

            default:
                uart_out_string("Error: unknown command.\r\n");
                break;
        }
    }
}
