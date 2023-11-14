#include "os.h"


#define MAX_CPUS 8
#define MAX_PROC 128

#define STACK_SIZE (1024 * 4)


/* task management */
struct context {
	/* ignore x0 */
	reg_t ra;   /* 0 */ 
	reg_t sp;   /* 4 */
	reg_t gp;   /* 8  */
	reg_t tp;   /* 12  */
	reg_t t0;   /* 16  */
	reg_t t1;   /* 20  */
	reg_t t2;   /* 24  */
	reg_t s0;   /* 28  */
	reg_t s1;   /* 32  */
	reg_t a0;   /* 36  */
	reg_t a1;   /* 40  */
	reg_t a2;   /* 44  */
	reg_t a3;   /* 48  */
	reg_t a4;   /* 52  */
	reg_t a5;   /* 56  */
	reg_t a6;   /* 60  */
	reg_t a7;   /* 64  */
	reg_t s2;   /* 68  */
	reg_t s3;   /* 72  */
	reg_t s4;   /* 76  */
	reg_t s5;   /* 80  */
	reg_t s6;   /* 84  */
	reg_t s7;   /* 88  */
	reg_t s8;   /* 92  */
	reg_t s9;   /* 96  */
	reg_t s10;  /* 100 */
	reg_t s11;  /* 104 */
	reg_t t3;   /* 108 */ 
	reg_t t4;   /* 112 */
	reg_t t5;   /* 116 */
	reg_t t6;   /* 120 */

	reg_t pc;   /* 124 */  // actural epc.
};

// each cpu's context to support switch.
struct cpu {
    struct context ctx;
    uint8_t __attribute__((aligned(16))) stack[STACK_SIZE];
};


// process available status.
enum pstatus {
    DEAD,
    READY,
    RUNNING,
    BLOCKED
};

// like PCB.
struct proc {
    uint32_t pid;
    enum pstatus stat;
    struct context ctx;
    uint8_t __attribute__((aligned(16))) stack[STACK_SIZE];
};