#ifndef PERIODIC_H
#define PERIODIC_H

#include <stdint.h>

typedef struct BGT BGT_t;
struct BGT {
  void (*func)(void);
  BGT_t *next;
  BGT_t *prev;
  uint32_t period;
  uint32_t sched_time;
};

void schedule_background(void);

uint32_t add_periodic_task(void(*task)(void), uint32_t period);


#endif // PERIODIC_H