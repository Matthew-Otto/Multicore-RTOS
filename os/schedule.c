#include <stdio.h>
#include <stdint.h>
#include "schedule.h"
#include "../inc/rp2040.h"
#include "../hw/hwctrl.h"
#include "semaphore.h"


extern void (*vector_table[])(void);

#define NUM_CORES 2
volatile TCB_t *RunPt[NUM_CORES] = {NULL};
volatile TCB_t *NextRunPt[NUM_CORES] = {NULL};
volatile TCB_t *IdleThread[NUM_CORES] = {NULL};

// Memory shared by both cores. must be accesses with mutex
#define PRIORITY_LVL_CNT 5
#define IDLE_PRIORITY PRIORITY_LVL_CNT // alias
static uint32_t LifetimeThreadCount = 0;
static uint16_t ActivePriorityCount[PRIORITY_LVL_CNT+1] = {0}; // count the number of active threads in each priority level
static TCB_t *ThreadSchedule[PRIORITY_LVL_CNT+1]; // tracks pointers to link-list of each priority schedule

// forward declarations
static void idle_thread(void);
static void core1_entry(void);

void init_scheduler(uint32_t timeslice, bool multicore) {
    uint8_t cpu = proc_id();

    // initialize systick timer
    #define CLK_RATE 133000000 // 133Mhz
    SYST_RVR = (CLK_RATE / 1000) * timeslice; // set schedule timeslice (in ms)
    SYST_CSR = 0x7; // use processor clock, enable systick interrupts, enable systick timer


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


    // initialize idle task
    TCB_t *idle_tcb = (TCB_t *)malloc(sizeof(TCB_t));
    if (idle_tcb == NULL) return;
    uint32_t *stack = (uint32_t *)malloc(32*sizeof(uint32_t));
    if (stack == NULL) return;
    idle_tcb->id = 0;
    idle_tcb->priority = PRIORITY_LVL_CNT; // special last prioirity
    idle_tcb->state = IDLE;
    idle_tcb->stack = stack;
    // initialize idle thread stack
    idle_tcb->stack[32-1] = 0x01000000;            // PSR
    idle_tcb->stack[32-2] = (uint32_t)idle_thread; // PC (function pointer)
    idle_tcb->stack[0] = 0xDEADBEEF;               // magic value for stack overflow detection
    idle_tcb->sp = &idle_tcb->stack[32-16];        // set thread SP
    IdleThread[cpu] = idle_tcb;
    ActivePriorityCount[IDLE_PRIORITY] = 1;


    // launch threads on both cpus
    // select first thread
    sched_lock();
    uint8_t pri = 0;
    while (ActivePriorityCount[pri] == 0) { 
      pri++;
    }
    if (pri == IDLE_PRIORITY) {
        RunPt[cpu] = IdleThread[cpu];
    } else {
        RunPt[cpu] = ThreadSchedule[pri];
        ThreadSchedule[pri] = RunPt[cpu]->next_tcb; // point root to next element to be scheduled
        RunPt[cpu]->state = RUNNING;
        ActivePriorityCount[pri]--;
    }
    sched_release();

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
void schedule(void) {
    sched_lock();
    uint32_t primask = start_critical();

    uint8_t cpu = proc_id();


    // ...
    // TODO sleeping
    // TODO blocking
    // TODO kill

    // update current thread / put it back into schedule
    if (RunPt[cpu]->state == RUNNING) {
        RunPt[cpu]->state == ACTIVE;
        ActivePriorityCount[RunPt[cpu]->priority]++;
    }

    // Schedule next thread
    // find highest priority level with at least one active thread
    uint8_t pri = 0;
    while (ActivePriorityCount[pri] == 0) { 
        pri++;
    }
    if (pri == IDLE_PRIORITY) {
        NextRunPt[cpu] = IdleThread[cpu];
    } else {
        NextRunPt[cpu] = ThreadSchedule[pri];
        ThreadSchedule[pri] = NextRunPt[cpu]->next_tcb; // point root to next element to be scheduled
        NextRunPt[cpu]->state = RUNNING;
        ActivePriorityCount[pri]--;
    }

    // trigger pendsv interrupt to context switch
    ICSR = 1 << PENDSVSET_OFFSET;

    sched_release();
    end_critical(primask);
}

// add new thread to schedule
uint32_t add_thread(void(*task)(void), uint32_t stack_size, uint32_t priority) {
    sched_lock();
    uint32_t primask = start_critical();

    if (priority >= PRIORITY_LVL_CNT) return 1;

    TCB_t *newtcb = (TCB_t *)malloc(sizeof(TCB_t));
    if (newtcb == NULL) {
        sched_release();
        end_critical(primask);
        return 1;
    }
    uint32_t *stack = (uint32_t *)malloc(stack_size*sizeof(uint32_t));
    if (stack == NULL) {
      free(newtcb);
      end_critical(primask);
      return 1;
    }

    LifetimeThreadCount++;
    newtcb->id = LifetimeThreadCount;
    newtcb->priority = priority;
    newtcb->state = ACTIVE;
    newtcb->stack = stack;

    // initialize thread stack
    newtcb->stack[stack_size-1] = 0x01000000;     // PSR
    newtcb->stack[stack_size-2] = (uint32_t)task; // PC (function pointer)
    newtcb->stack[0] = 0xDEADBEEF;                // magic value for stack overflow detection
    newtcb->sp = &newtcb->stack[stack_size-16];    // set thread SP

    // add thread to schedule
    if (ThreadSchedule[priority] == NULL) {
        newtcb->next_tcb = newtcb;
        newtcb->prev_tcb = newtcb;
        ThreadSchedule[priority] = newtcb;
    } else {  // insert into end of list  
        newtcb->prev_tcb = ThreadSchedule[priority]->prev_tcb;
        ThreadSchedule[priority]->prev_tcb = newtcb;
        newtcb->prev_tcb->next_tcb = newtcb;
        newtcb->next_tcb = ThreadSchedule[priority];
        
        // immediately schedule new thread if it is the second thread to be added to this priority
        if (ActivePriorityCount[priority] == 1) {
            ThreadSchedule[priority] = newtcb;
        }
    }
    ActivePriorityCount[priority]++;

    sched_release();
    end_critical(primask);
    return 0;
}

// schedules next thread
void suspend(void) {

}

// sleeps thread for <sleep_time> ms
void sleep(uint32_t sleep_time) {

}

// removes thread from schedule and frees memory
void kill(void) {

}

// stub to grab timeslice from cpu0 and initialize scheduler
static void core1_entry(void) {
    // get timeslice from cpu0
    uint32_t timeslice = multicore_fifo_pop_blocking();
    init_scheduler(timeslice, false);
}

#include "../hw/gpio.h"
static void idle_thread(void){
    init_gpio(25, GPIO_OUTPUT);
    init_gpio(2, GPIO_OUTPUT);
    while(1) {
        gpio_toggle(25);
        gpio_toggle(2);
        wait_for_interrupt();
    }
}