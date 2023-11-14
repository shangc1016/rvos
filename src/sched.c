#include "os.h"


// all cpus exists in system.
struct cpu cpus[MAX_CPUS];

// all procs exists in system.
struct proc procs[MAX_PROC];
// indicate system's proc nums
uint32_t proc_sz;

uint32_t current = -1;

// kernel scheduler, should never return
void scheduler() {
    
    if(proc_sz <= 0) {
        panic("no running proc\n");
        return;
    }

    current = (current + 1) % proc_sz;
    struct context *next = &procs[current].ctx;
    switch_to(next);
    
    // while(1) {
    //     for(uint32_t i = 0; i < proc_sz; i++) {
    //         if(procs[i].stat == READY) {
    //             printf("scheduler choose: pid = %d\n", i);
    //             procs[i].stat = RUNNING;
    //             switch_to(&procs[i].ctx);
    //         }
    //     }
    // }
}

// init scheduler
void sched_init() {
    // init mscratch registry to 0, means first ctx
    // thus no need to save ctx to the place where mscratch point to in `switch_to`
    w_mscratch(0);

    // init each cpu's switch context
    reg_t hartid = r_mhartid();
    cpus[hartid].ctx.pc = (reg_t)scheduler;
    cpus[hartid].ctx.sp = (reg_t)&cpus[hartid].stack[STACK_SIZE];
    
    // init proc_sz
    proc_sz = 0;

    // enable machine-mode software interrupts.
    w_mie(r_mie() | MIE_MSIE);
}

// mock time consuming
void task_delay(volatile uint32_t count) {
    count *= 500000;
    while(count--);
}

int create_proc(void (*func)(void)) {
    if(proc_sz >= MAX_PROC) {
        printf("proc nums overflow\n");
        return -1;
    }
    // here we init proc's pc to it's entry, thus return to epc.
    procs[proc_sz].ctx.pc = (reg_t)func;
    procs[proc_sz].ctx.sp = (reg_t)&procs[proc_sz].stack[STACK_SIZE];
    procs[proc_sz].stat = READY;
    proc_sz++;
    return 0;
}

// yield the use of current cpu.
// machine-mode software interrupt.
void yield() {

    int hartid = r_mhartid();
    // write 1 to msip register to send a software interrupt.
    *(uint32_t*)CLINT_MSIP(hartid) = 1;
}