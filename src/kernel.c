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

    // enable proc switch.
    sched_init();

    // create two proc.
    create_proc(user_func0);
    create_proc(user_func1);

    // enter scheduler, should never return.
    scheduler();

    panic("should never reach here.\n");
}
