#include "os.h"


int spinlock_lock() {
    w_mstatus(r_mstatus() & ~MSTATUS_MIE);
    return 0;
}


int spinlock_unlock() {
    w_mstatus(r_mstatus() | MSTATUS_MIE);
    return 0;
}