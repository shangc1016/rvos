#include "os.h"

void user_func0() {
    printf("user proc 0 created\n");
    while(1) {
        printf("user proc0 running ...\n");
        task_delay(1000);
        yield(0);
    }
}


void user_func1() {
    printf("user proc 1 created\n");
    while(1) {
        printf("user proc1 running ...\n");
        task_delay(1000);
        yield(1);
    }
}