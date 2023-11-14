#include "os.h"

#define DELAY 100


void trap_maker() {
    *((int*)(0x0)) = 123;

    printf("back from trap.\n");
}

void user_func0() {

    uart_puts("Task 0: Created!\n");
	uint32_t x = 0;
	while (1) {
		x++;
		if (x % 10 == 0) {
			yield();
		}
		printf("Task 0: Running..., x = %d\n", x);
		task_delay(DELAY);
	}
}


void user_func1() {
    uart_puts("Task 1: Created!\n");
	while (1) {
		uart_puts("Task 1: Running...\n");
		task_delay(DELAY);
	}
}