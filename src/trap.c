#include "os.h"

// defined in switch.S
extern void trap_vector(void);

// 
// total scenery of trap:
// 1. trap occur.
// 2. hardware handle the upper half of trap, including store pc to `mepc`, store `mie`, etc.
// 3. hardware replace pc with the address stored in register `mtvec`. (trap_init function init it.)
// 4. execute along trap_vector, save context of previous code.
// 5. call trap_handler, it's C function, we focus here.
// 6. return to trap_vector, noice the return value is the epc to set, thus the address to return finally.
// 7. restore to ordinary code.

void trap_init() {
    // set trap-vector base address from machine-mode
    w_mtvec((reg_t)trap_vector);
}

void external_exception_handler() {
    
    int irq = plic_claim();

    if(irq == UART0_IRQ) {
        uart_isr();
    } else if(irq) {
        printf("unexpected interrupt irq = %d\n", irq);
    }

    if(irq) {
        plic_complete(irq);
    }
}


// accord to switch.S relavent code, the args is mepc and mcause.
reg_t trap_handler(reg_t mepc, reg_t mcause) {
    // here we 'switch-case' mepc and mcause.
    reg_t return_pc = mepc;
    reg_t cause_code = mcause & 0xfff;

    // according to Table 3.6: Machine cause register (mcause) values after trap.
    // 

    if(mcause & 0x80000000) {
        // interrupt.
        // pc register store the address of next instruction.
        // so after trap_handler, will execute from next instruction.
        switch(cause_code) {
            case 3:
                printf("software interrupt\n");
                break;
            case 7:
                printf("machine timer interrupt\n");
                break;
            case 11:
                printf("--- machine external interrupt\n");
                external_exception_handler();
                break;
            default:
                printf("unknown interrupt\n");
                break;
        }
    } else {
        // exception.
        // pc register store the address of the instruction caused exception.
        // so after trap_handler, the instruction will be executed for a second time.
        printf("sync exception, code = %d\n", cause_code);
        // panic("oops!");

        // if we not move pc to next instruction, there will be a dead loop.
        return_pc += 4;
    }

    return return_pc;
}