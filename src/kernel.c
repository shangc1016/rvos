#include "os.h"

/*
 * Following functions SHOULD be called ONLY ONE time here,
 * so just declared here ONCE and NOT included in file os.h.
 */
extern void uart_init(void);
extern void page_init(void);

extern int proc_sz;

void start_kernel() {
    
    uart_init();
    uart_puts("Hello, RVOS\n");

    // enable physical memory management.
    page_init();

    // set mtvec, enable trap.
    trap_init();

    // init plic, enable device interrupt.
    plic_init();

    // enable hw timer.
    timer_init();

    // enable proc switch.
    sched_init();

    // add user task, let scheduler loop them.
    user_main();

    // enter scheduler, should never return.
    scheduler();

    panic("should never reach here.\n");
}
