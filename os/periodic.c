#include <stdio.h>
#include "periodic.h"
#include "../hw/hwctrl.h"
#include "../hw/timer.h"
#include "../os/semaphore.h"

static BGT_t *PeriodicScheduleRoot;

void schedule_background(void) {
    uint32_t primask = start_critical();
    lock(BGSCHEDULER);

    BGT_t *bgthread = PeriodicScheduleRoot;

    // execute background task
    (*bgthread->func)();

    // calculate when task should be scheduled next
    bgthread->sched_time += (bgthread->period * 1000);

    // resort queue if necessary
    if (bgthread->sched_time > bgthread->next->sched_time) {
        // remove bgthread from queue
        bgthread->prev->next = bgthread->next;
        bgthread->next->prev = bgthread->prev;
        PeriodicScheduleRoot = bgthread->next;

        // reinsert it in the proper location
        BGT_t *node = PeriodicScheduleRoot->next;
        while (node != PeriodicScheduleRoot && bgthread->sched_time > node->sched_time) {
            node = node->next;
        }
        bgthread->next = node;
        bgthread->prev = node->prev;
        node->prev->next = bgthread;
        node->prev = bgthread;
    }
    
    // rearm timer
    arm_timer(PERIODIC, PeriodicScheduleRoot->sched_time);

    unlock(BGSCHEDULER);
    end_critical(primask);
}

uint32_t add_periodic_task(void(*task)(void), uint32_t period) {
    uint32_t primask = start_critical();
    lock(BGSCHEDULER);

    // create new background thread
    BGT_t *bgthread = (BGT_t *)malloc(sizeof(BGT_t));
    if (bgthread == NULL) return 1;

    bgthread->func = task;
    bgthread->period = period;
    bgthread->sched_time = get_raw_time() + (period * 1000);

    // insert thread into background queue and arm timer if necessary
    if (PeriodicScheduleRoot == NULL) {
        bgthread->next = bgthread;
        bgthread->prev = bgthread;
        PeriodicScheduleRoot = bgthread;
        arm_timer(PERIODIC, bgthread->sched_time);
    } else {
        // insert into sorted queue
        BGT_t *node;
        if (bgthread->sched_time < PeriodicScheduleRoot->sched_time){
            node = PeriodicScheduleRoot;
            arm_timer(PERIODIC, bgthread->sched_time);
        } else {
            node = PeriodicScheduleRoot->next;
            while (node != PeriodicScheduleRoot && bgthread->sched_time > node->sched_time) {
                node = node->next;
            }
        }
        bgthread->next = node;
        bgthread->prev = node->prev;
        node->prev->next = bgthread;
        node->prev = bgthread;
    }

    unlock(BGSCHEDULER);
    end_critical(primask);
}