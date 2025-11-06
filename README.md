# Multi Threaded Scheduling
a real-time juggling act for threads with each pretending to be a vital organ in the tiny body of an embedded system. memory, cpu, device, app, and ui threads all taking turns under a strict RTOS scheduler so nobody takes up the processor for too long. this can be configured with correct led header and initialization files to see how each thread configured to an led takes up...since it’s multi-threaded, you can observe context switching, thread priorities, and signaling in real time as the scheduler balances load and timing between concurrent processes. so cool


using: arm-cortex m3 (lpc1768 board), keil µvision (ide), cmsis-rtos.
