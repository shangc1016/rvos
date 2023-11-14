#include "os.h"
#include "syscall.h"


// kernel part of syscall.

int sys_gethid(unsigned int *hid) {
    if(hid == NULL) {
        return -1;
    }
    *hid = r_mhartid();
    return 0;
}

void do_syscall(struct context *ctx) {
    // ctx->a7 store syscall number
    unsigned int syscall_num = ctx->a7;
    switch(syscall_num) {
        case SYS_gethid: {
            ctx->a0 = sys_gethid((unsigned int*)(ctx->a0));
            break;
        }
        default: {
            printf("unknown syscall number\n");
        }
    }
}