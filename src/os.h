#ifndef __OS_H__
#define __OS_H__

#include "types.h"
#include "platform.h"
#include "riscv.h"
#include "proc.h"

#include <stddef.h>
#include <stdarg.h>

/* uart */
extern int uart_putc(char ch);
extern void uart_puts(char *s);

/* printf */
extern int  printf(const char* s, ...);
extern void panic(char *s);

/* memory management */
extern void *page_alloc(int npages);
extern void page_free(void *p);

/* defined in entry.S */
extern void switch_to(struct context *next);

/* sched.c */
void scheduler();
void sched_init();
void task_delay(volatile uint32_t count);
int create_proc(void* func);
void yield(uint32_t pid);


/* user.c */
void user_func0();
void user_func1();

/* trap.c */ 
void trap_init();


#endif /* __OS_H__ */
