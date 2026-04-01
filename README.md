# Multicore-RTOS


A simple, bare-metal real-time operating system (RTOS) for the Raspberry Pi Pico with multicore scheduling support.

This implementation is completely bare metal. It does not rely on Pico SDK or any other libraries beyond the `arm-none-eabi` toolchain.

I do not guarantee this implementation to be completely bug free.

## Dependencies

* arm-none-eabi-gcc (and friends)
* openocd


## Key Features

* **Constant-Time $O(1)$ Priority Scheduling:** The shared thread schedule is implemented as an array of linked lists, allowing the CPU to select the next highest-priority task in constant time.
* **True Multicore Synchronization:** Safe inter-core communication and resource sharing achieved through custom multicore mutexes, built directly on top of the RP2040's hardware spinlock registers.
* **Fast Context Switching:** A full context switch (from PendSV trigger to handler exit) takes just 3.5µs on average at 133MHz.
  -  The context switch routine includes quite a few instructions in order to handle the ARMv6-M's inability to push/pop upper registers. There might be a method to reduce this, but more research will be required.
* **Blocking Semaphores:** Counting semaphores support thread blocking and unblocking. Blocked threads are removed from the active schedule to save CPU cycles until a `signal()` is received.
* **Sleeping:** Threads can yield execution via `sleep()`. The RTOS utilizes the RP2040's hardware timer alarms to sort sleeping threads and wake them precisely when needed.
* **Periodic Background Tasks:** Support for scheduled, recurring tasks that execute reliably on CPU0.

## Performance Benchmarks

The multicore scheduling overhead is exceptionally low. Using a dual-thread approximation of the Dhrystone benchmark, this RTOS achieved a **1.85x speedup** when utilizing both cores compared to single-core execution (1.52s vs 2.81s). 

I presume the sublinear speedup is caused by bus contention when instructions are fetched from flash memory. Since both cores execute with the same time slice and the workload does not involve asynchronous interrupts, contention for the scheduler lock is unlikely. However, I would need to investigate further to know for sure.

## References & Resources

* [RP2040 Datasheet](https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf)
* [ARMv6-M Architecture Reference Manual](https://documentation-service.arm.com/static/5f8ff05ef86e16515cdbf826)
* [Good baremetal examples for RP2040](https://github.com/carlosftm/RPi-Pico-Baremetal)
* [Resetting both cores on RP2040 with OpenOCD](https://stackoverflow.com/questions/72745969/openocd-one-liner-for-proper-multicore-mcu-reset)
