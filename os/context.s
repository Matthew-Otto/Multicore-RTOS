    .cpu cortex-m0plus
    .thumb
    .section .text,"ax"

    .extern  RunPt            // currently running thread
    .extern  NextRunPt        // next thread selected by scheduler

    .global pendSV_handler
    .type pendSV_handler, %function

pendSV_handler:
    CPSID I
    PUSH {R4-R7}
    LDR R0, =RunPt
    LDR R1, [R0]
    MRS R3, PSP
    STR R3, [R1]        // store SP

    LDR R2, =NextRunPt
    LDR R1, [R2]        // load next TCB pointer
    STR R1, [R0]        // update RunPt
    LDR R3, [R1]        // load SP
    MSR PSP, R3
    POP {R4-R7}
    CPSIE I
    BX LR
    