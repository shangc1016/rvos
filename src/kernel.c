#include "types.h"

extern void uart_init(void);
extern void uart_puts(char *s);



void start_kernel() {
    uart_init();
    uart_puts("Hello, RVOS\n");

    while(1) {

    };
}
