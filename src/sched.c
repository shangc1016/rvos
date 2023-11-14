#include "os.h"

#define MAX_CPUS 8
#define MAX_PROC 128

#define STACK_SIZE (1024 * 4)

static void w_mscratch(reg_t x) {
    asm volatile("csrw mscratch, %0" : : "r" (x));
}

static reg_t r_mhartid() {
    reg_t x;
    asm volatile("csrr %0, mhartid" : "=r" (x));
    return x;
}


// all cpus exists in system.
struct cpu cpus[MAX_CPUS];

// all procs exists in system.
struct proc procs[MAX_PROC];
// indicate system's proc nums
uint32_t proc_sz;

// kernel scheduler, should never return
void scheduler() {
    while(1) {
        for(uint32_t i = 0; i < proc_sz; i++) {
            if(procs[i].stat == READY) {
                printf("scheduler choose: pid = %d\n", i);
                switch_to(&procs[i].ctx);
            }
        }
    }
}

// init scheduler
void sched_init() {
    // init mscratch registry to 0, means first ctx
    // thus no need to save ctx to the place where mscratch point to in `switch_to`
    w_mscratch(0);

    // init each cpu's switch context
    reg_t hartid = r_mhartid();
    cpus[hartid].ctx.ra = (reg_t)scheduler;
    cpus[hartid].ctx.sp = (reg_t)&cpus[hartid].stack[STACK_SIZE];
    
    // init proc_sz
    proc_sz = 0;
}

// mock time consuming
void task_delay(volatile uint32_t count) {
    count *= 500000;
    while(count--);
}

int create_proc(void* func) {
    if(proc_sz >= MAX_PROC) {
        printf("proc nums overflow\n");
        return -1;
    }

    procs[proc_sz].ctx.ra = (reg_t)func;
    procs[proc_sz].ctx.sp = (reg_t)&procs[proc_sz].stack[STACK_SIZE];
    procs[proc_sz].stat = READY;
    proc_sz++;
    return 0;
}

// yield the use of current cpu.
void yield(uint32_t pid) {
    // just to envoke another proc.
    if(pid == 0) {
        procs[1].stat = READY;
        procs[0].stat = BLOCKED;
    } else if(pid == 1) {
        procs[0].stat = READY;
        procs[1].stat = BLOCKED;
    }
    printf("proc %d yield cpu.\n", pid);
    reg_t hartid = r_mhartid();
    switch_to(&cpus[hartid].ctx);
}