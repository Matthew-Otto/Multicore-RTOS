#include <stdio.h>
#include <stdint.h>
#include "schedule.h"
#include "../inc/rp2040.h"
#include "../hw/hwctrl.h"
#include "../hw/timer.h"
#include "../os/ipc.h"
#include "../os/semaphore.h"



extern void (*vector_table[])(void);

#define NUM_CORES 2
TCB_t RunPtBlackHole = {.state=DEAD};
TCB_t *RunPt[NUM_CORES] = {NULL};
TCB_t *NextRunPt[NUM_CORES] = {NULL};
TCB_t *IdleThread[NUM_CORES] = {NULL};
uint64_t IdleTime[NUM_CORES] = {0};
uint64_t IdleTimeStart[NUM_CORES] = {0};

// Memory shared by both cores. must be accesses with mutex
#define PRIORITY_LVL_CNT 5
#define IDLE_PRIORITY PRIORITY_LVL_CNT // alias
static uint32_t LifetimeThreadCount = 0;
static uint16_t ActivePriorityCount[PRIORITY_LVL_CNT+1] = {0}; // count the number of active threads in each priority level
static uint16_t RunningPriorityCount[PRIORITY_LVL_CNT] = {0}; // count the number of running threads in each priority level
static TCB_t *ThreadSchedule[PRIORITY_LVL_CNT+1]; // tracks pointers to link-list of each priority schedule
static TCB_t *SleepScheduleRoot = NULL;

// forward declarations
static void core1_entry(void);
static void idle_thread(void);
static void enter_idle(uint8_t cpu_id);
static void exit_idle(uint8_t cpu_id);

void init_scheduler(uint32_t timeslice, bool multicore) {
    uint8_t cpu = proc_id();

    // wake CPU1
    if (cpu == 0 && multicore) {
        uint32_t initial_pc1 = (uint32_t)core1_entry | 0x1; // set thumb bit
        const uint32_t wake_seq[6] = {0, 0, 1, (uint32_t)vector_table, CPU1_STACK_TOP, initial_pc1};
        uint8_t idx = 0;
    
        do {
            uint32_t cmd = wake_seq[idx];
            if (!cmd) { // always drain the READ FIFO (from core 1) before sending a 0
                multicore_fifo_drain(); // discard data from read FIFO until empty
                __asm ("sev"); // execute a SEV as core 1 may be waiting for FIFO space
            }
            multicore_fifo_push_blocking(cmd); // write 32 bit value to write FIFO
            uint32_t response = multicore_fifo_pop_blocking(); // read 32 bit value from read FIFO once available
            idx = cmd == response ? idx + 1 : 0; // move to next state on correct response (echo-d value) otherwise start over
        } while (idx < 6);

        // send timeslice to cpu1
        multicore_fifo_push_blocking(timeslice);
    }

    // lock scheduler before initializing systick to stagger core context switches
    lock(SCHEDULER);

    // initialize systick timer
    #define CLK_RATE 133000000 // 133Mhz
    SYST_RVR = (CLK_RATE / 1000) * timeslice; // set schedule timeslice (in ms)
    SYST_CVR = 0;   // clear current countdown value
    SYST_CSR = 0x7; // use processor clock, enable systick interrupts, enable systick timer

    // initialize idle task
    TCB_t *idle_tcb = (TCB_t *)malloc(sizeof(TCB_t));
    if (idle_tcb == NULL) return;
    uint8_t *stack = (uint8_t *)malloc(128);
    if (stack == NULL) return;
    idle_tcb->id = 0;
    idle_tcb->priority = PRIORITY_LVL_CNT; // special last prioirity
    idle_tcb->state = IDLE;
    idle_tcb->stack = stack;
    // initialize idle thread stack
    *(uint32_t *)&stack[128-4] = 0x01000000;            // PSR
    *(uint32_t *)&stack[128-8] = (uint32_t)idle_thread; // PC (function pointer)
    *(uint32_t *)stack = 0xDEADBEEF;                    // magic value for stack overflow detection
    idle_tcb->sp = &stack[128-64];                      // set thread SP
    IdleThread[cpu] = idle_tcb;
    ActivePriorityCount[IDLE_PRIORITY] = 1;

    // launch threads on both cpus
    // select first thread
    uint8_t pri = 0;
    while (ActivePriorityCount[pri] == 0) { 
      pri++;
    }
    if (pri == IDLE_PRIORITY) {
        RunPt[cpu] = IdleThread[cpu];
    } else {
        RunPt[cpu] = ThreadSchedule[pri];
        ThreadSchedule[pri] = ThreadSchedule[pri]->next_tcb; // point root to next element to be scheduled
        RunPt[cpu]->state = RUNNING;
        ActivePriorityCount[pri]--;
        RunningPriorityCount[pri]++;
    }
    unlock(SCHEDULER);

    // launch first task
    __asm ("LDR     R0, [%0]":: "r" (RunPt[cpu])); // Load @ RunPt to get SP
    __asm ("MOV     SP, R0");      // load SP
    __asm ("ADD     SP, SP, #56"); // Advance stack pointer to PC
    __asm ("LDR     R1, [SP]");    // load initial function pointer into LR from stack
    __asm ("ADD     SP, SP, #4");  // poor man's pop
    __asm ("MOV     LR, R1");
    __asm ("CPSIE   I");           // enable interrupts
    __asm ("BX      LR");          // branch to first thread
}

// schedules next task
void schedule() {
    uint32_t primask = start_critical();
    lock(SCHEDULER);

    // update idle time
    uint8_t cpu = proc_id();
    if (RunPt[cpu]->state == IDLE) {
        exit_idle(cpu);
    }

    // update current thread / put it back into schedule
    if (RunPt[cpu]->state == RUNNING) {
        RunPt[cpu]->state = ACTIVE;
        ActivePriorityCount[RunPt[cpu]->priority]++;
        RunningPriorityCount[RunPt[cpu]->priority]--;
    }

    // Schedule next thread
    // find highest priority level with at least one active (but not running) thread
    uint8_t pri = 0;
    while (ActivePriorityCount[pri] == 0) { 
        pri++;
    }
    if (pri == IDLE_PRIORITY) {
        NextRunPt[cpu] = IdleThread[cpu];
        enter_idle(cpu);
    } else {
        if (ThreadSchedule[pri]->state == RUNNING) {
            NextRunPt[cpu] = ThreadSchedule[pri]->next_tcb;
        } else {
            NextRunPt[cpu] = ThreadSchedule[pri];
            ThreadSchedule[pri] = ThreadSchedule[pri]->next_tcb; // point root to next element to be scheduled
        }
        NextRunPt[cpu]->state = RUNNING;
        ActivePriorityCount[pri]--;
        RunningPriorityCount[pri]++;
    }

    // trigger pendsv interrupt to context switch
    ICSR = 1 << PENDSVSET_OFFSET;

    unlock(SCHEDULER);
    end_critical(primask);
}

// add new thread to schedule
uint32_t add_thread(void(*task)(void), uint32_t stack_size, uint32_t priority) {
    uint32_t primask = start_critical();
    lock(SCHEDULER);

    if (priority >= PRIORITY_LVL_CNT) return 1;

    TCB_t *newtcb = (TCB_t *)malloc(sizeof(TCB_t));
    if (newtcb == NULL) {
        unlock(SCHEDULER);
        end_critical(primask);
        return 1;
    }
    uint8_t *stack = (uint8_t *)malloc(stack_size);
    if (stack == NULL) {
      free(newtcb);
      unlock(SCHEDULER);
      end_critical(primask);
      return 1;
    }

    LifetimeThreadCount++;
    newtcb->id = LifetimeThreadCount;
    newtcb->priority = priority;
    newtcb->stack = stack;

    // initialize thread stack
    *(uint32_t *)&stack[stack_size-4] = 0x01000000;     // PSR
    *(uint32_t *)&stack[stack_size-8] = (uint32_t)task; // PC (function pointer)
    *(uint32_t *)stack = 0xDEADBEEF;                    // magic value for stack overflow detection
    newtcb->sp = &stack[stack_size-64];                 // set thread SP

    // add thread to schedule
    enqueue_thread(newtcb);

    unlock(SCHEDULER);
    end_critical(primask);
    return 0;
}

// schedule next thread
// simple alias for schedule()
void suspend(void) __attribute__((alias("schedule")));

// remove thread from schedule
void sched_block(Sema4_t *sem) {
    uint32_t primask = start_critical();
    lock(SCHEDULER);

    uint8_t cpu = proc_id();
    TCB_t *thread = RunPt[cpu];
    
    // remove RunPt from thread pool
    dequeue_thread(thread);
    // set thread state to blocked
    thread->state = BLOCKED;

    // add thread to semaphore
    if (sem->bthreads_root == NULL) {
        sem->bthreads_root = thread;
    } else {
        // insert tcb into blocked list based on priority
        TCB_t *blocked_node = sem->bthreads_root;
        if (thread->priority < blocked_node->priority) {
            // insert into front of list
            thread->next_tcb = blocked_node;
            sem->bthreads_root = thread;
        } else {
            while (1) {
                if (blocked_node->next_tcb == NULL) {
                    blocked_node->next_tcb = thread;
                    break;
                } else if (thread->priority < blocked_node->next_tcb->priority) {
                    thread->next_tcb = blocked_node->next_tcb;
                    blocked_node->next_tcb = thread;
                    break;
                }
                blocked_node = blocked_node->next_tcb;
            }
        }
    }

    unlock(SCHEDULER);
    end_critical(primask);

    schedule();
}

// insert thread into schedule
bool sched_unblock(Sema4_t *sem) {
    uint32_t primask = start_critical();
    lock(SCHEDULER);

    TCB_t *thread = sem->bthreads_root;

    // update semaphore blocked list
    sem->bthreads_root = sem->bthreads_root->next_tcb;

    // insert unblocked thread into beginning of active list
    enqueue_thread(thread);

    unlock(SCHEDULER);
    end_critical(primask);

    // determine if this unblocked thread was higher priority 
    // than the thread that signaled it
    uint8_t cpu = proc_id();
    return (thread->priority >= RunPt[cpu]->priority);
}

// sleeps thread for <sleep_time> ms
void sleep(uint32_t sleep_time) {
    if (sleep_time == 0) return;
    uint32_t primask = start_critical();
    lock(SCHEDULER);

    uint8_t cpu = proc_id();
    TCB_t *thread = RunPt[cpu];
    
    // calculate when this thread should be re-queued
    thread->resume_tick = get_raw_time() + (sleep_time * 1000);

    // remove RunPt from thread pool
    dequeue_thread(thread);
    // update thread state
    thread->state = SLEEPING;

    // insert thread into sleep queue and arm timer if necessary
    if (SleepScheduleRoot == NULL) {
        SleepScheduleRoot = thread;
        arm_timer(SLEEP, thread->resume_tick);
    } else {
        // if current thread will resume before the current head of the list
        if (thread->resume_tick < SleepScheduleRoot->resume_tick){
            // insert before SleepScheduleRoot
            thread->next_tcb = SleepScheduleRoot;
            SleepScheduleRoot = thread;
            arm_timer(SLEEP, thread->resume_tick);
        } else {
            // find where the new sleeping thread belongs
            TCB_t *node = SleepScheduleRoot;
            while (1) {
            if (node->next_tcb == NULL) {
                node->next_tcb = thread;
                break;
            } else if (thread->resume_tick < node->next_tcb->resume_tick) {
                thread->next_tcb = node->next_tcb;
                node->next_tcb = thread;
                break;
            }
            node = node->next_tcb;
            }
        }
    }

    unlock(SCHEDULER);
    end_critical(primask);

    schedule();
}

// unsleeps the first thread in the sleep queue
void unsleep(void) {
    if (SleepScheduleRoot == NULL) return; // this shouldn't happen, but just in case
    uint32_t primask = start_critical();
    lock(SCHEDULER);

    TCB_t *resumed_thread = SleepScheduleRoot;
    SleepScheduleRoot = SleepScheduleRoot->next_tcb;

    // arm timer for next thread
    if (SleepScheduleRoot != NULL) {
        // hack to avoid missing threads that will resume soon
        if (SleepScheduleRoot->resume_tick < (get_raw_time() + 10)) {
            arm_timer(SLEEP, get_raw_time() + 10);
        } else {
            arm_timer(SLEEP, SleepScheduleRoot->resume_tick);
        }
    }

    // insert unslept thread into the front of the queue
    enqueue_thread(resumed_thread);

    unlock(SCHEDULER);
    end_critical(primask);
}

// removes thread from schedule and frees memory
void kill(void) {
    uint32_t primask = start_critical();

    uint8_t cpu = proc_id();
    TCB_t *thread = RunPt[cpu];

    // lock scheduler
    lock(SCHEDULER);
    // remove RunPt from thread pool
    dequeue_thread(thread);
    thread->state = DEAD;
    // release scheduler
    unlock(SCHEDULER);
    
    // free memory
    free(thread->stack);
    free(thread);
    // set RunPt to point to a stub so context switch doesnt write to recently freed memory
    RunPt[cpu] = &RunPtBlackHole;
    
    schedule();
    end_critical(primask);
}

void enqueue_thread(TCB_t *thread) {
    uint8_t priority = thread->priority;
    // insert thread into beginning of active list
    if (ThreadSchedule[priority] == NULL) {
        ThreadSchedule[priority] = thread;
        thread->next_tcb = thread;
        thread->prev_tcb = thread;
    } else {
        thread->next_tcb = ThreadSchedule[priority];
        thread->prev_tcb = ThreadSchedule[priority]->prev_tcb;
        thread->prev_tcb->next_tcb = thread;
        ThreadSchedule[priority]->prev_tcb = thread;
        ThreadSchedule[priority] = thread;
    }
    // update thread state
    thread->state = ACTIVE;
    // increment active count for this priority level
    ActivePriorityCount[priority]++;
}

void dequeue_thread(TCB_t *thread) {
    uint8_t priority = thread->priority;
    if (thread->state == RUNNING) {
        RunningPriorityCount[priority]--;
    }
    
    if (ActivePriorityCount[priority] == 0 && RunningPriorityCount[priority] == 0) {
        ThreadSchedule[priority] = NULL;
    } else {
        thread->prev_tcb->next_tcb = thread->next_tcb;
        thread->next_tcb->prev_tcb = thread->prev_tcb;
        ThreadSchedule[priority] = thread->next_tcb;
    }
    thread->next_tcb = NULL;
}

// stub to grab timeslice from cpu0 and initialize scheduler
static void core1_entry(void) {
    // get timeslice from cpu0
    uint32_t timeslice = multicore_fifo_pop_blocking();
    // init cpu1
    init_scheduler(timeslice, false);
}

#include "../hw/gpio.h"
static void idle_thread(void){
    uint32_t pin = 2 + proc_id();
    init_gpio(pin, GPIO_OUTPUT);
    while(1) {
        for (int i = 1;; i = (i%10)+1) {
            gpio_toggle(pin);
            for (int j = 0; j < i; ++j){   
                wait_for_interrupt();
            }
        }
    }
}

static void enter_idle(uint8_t cpu_id) {
    IdleTimeStart[cpu_id] = get_raw_time();
}

static void exit_idle(uint8_t cpu_id) {
    IdleTime[cpu_id] += get_raw_time() - IdleTimeStart[cpu_id];
}

uint32_t get_idle_percentage(uint8_t cpu_id) {
    uint64_t total_runtime = get_raw_time();
    return (IdleTime[cpu_id] * 1000 + total_runtime/2) / total_runtime;
}

void scheduler_stats(scheduler_stats_t *stats) {
    uint32_t primask = start_critical();
    lock(SCHEDULER);
    stats->running_thread_cnt = 0;
    stats->active_thread_cnt = 0;
    for (int i = 0; i < PRIORITY_LVL_CNT; ++i){
        stats->running_thread_cnt += RunningPriorityCount[i];
        stats->active_thread_cnt += ActivePriorityCount[i];
    }
    stats->lifetime_thread_cnt = LifetimeThreadCount;
    unlock(SCHEDULER);
    end_critical(primask);
}
