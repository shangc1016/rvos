#include "os.h"

#define DELAY 100

// caused exception
void trap_maker() {
    *((int*)(0x0)) = 123;

    printf("back from trap.\n");
}

static void user_func0() {

	while(1) {
		spinlock_lock();
		uart_puts("Task 0: Created!\n");
		// yield();
		for (int i = 0; i < 30; i++) {
			printf("Task 0: Running...\n");
			task_delay(DELAY);
		}
		spinlock_unlock();
	}
}

static void user_func1() {
    uart_puts("Task 1: Created!\n");
	while (1) {
		uart_puts("Task 1: Running...\n");
		task_delay(DELAY);
	}
}

static void user_func2() {
	printf("Task 2: Crated!\n");
	while(1) {
		printf("Task 2: Running ... \n");
		task_delay(DELAY);
	}
}

// add time-sharing proc here.
void user_main() {
	// create two proc.
    create_proc(user_func0);
    create_proc(user_func1);
	create_proc(user_func2);
}