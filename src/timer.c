#include "os.h"

/* interval ~= 1s */
#define TIMER_INTERVAL CLINT_TIMEBASE_FREQ

#define MAX_SWTIMER 100

// software timer array.
static struct swtimer timer_list[MAX_SWTIMER];

static uint32_t _tick = 0;


void swtimer_checker();

/* load timer interval(in ticks) for next timer interrupt.*/
void timer_load(int interval)
{
	/* each CPU has a separate source of timer interrupts. */
	int id = r_mhartid();
	
	*(uint64_t*)CLINT_MTIMECMP(id) = *(uint64_t*)CLINT_MTIME + interval;
}

void timer_init()
{
	/*
	 * On reset, mtime is cleared to zero, but the mtimecmp registers 
	 * are not reset. So we have to init the mtimecmp manually.
	 */
	timer_load(TIMER_INTERVAL);

	/* enable machine-mode timer interrupts. */
	w_mie(r_mie() | MIE_MTIE);

	/* enable machine-mode global interrupts. */
	w_mstatus(r_mstatus() | MSTATUS_MIE);
	
	// enable swtimer.
	swtimer_init();
}

void timer_handler() 
{
	_tick++;
	printf("------ tick: %d\n", _tick);

	timer_load(TIMER_INTERVAL);

	// traverse all swtimer, if time arrival, call it's func.
	swtimer_checker();
	
	if(_tick % 5 == 0) {
		scheduler();
	}
}



// software timer about function below.

void swtimer_checker() {
	struct swtimer *t = &timer_list[0];
	for(int i = 0; i < MAX_SWTIMER; i++) {
		if(t->func != NULL && _tick >= t->timeout_ticks) {
			t->func(t->args);

			if(t->periodic) {
				t->timeout_ticks += t->interval;
			} else {
				swtimer_delete(t);
			}
		}
		t++;
	}
}


struct swtimer* swtimer_create(void (*func)(void *args), void *args, uint32_t timeout, int periodic) {
	struct swtimer *t = &timer_list[0];
	int i = 0;
	for(; i < MAX_SWTIMER; i++) {
		if(t->func == NULL) {
			break;
		}
		t++;
	}
	if(i >= MAX_SWTIMER) {
		panic("swtimer nbums overflow\n");
		return NULL;
	}

	t->func = func;
	t->args = args;
	// thus when _tick reach t->timeout, timer func calleed.
	t->timeout_ticks = _tick + timeout;
	t->interval = timeout;
	t->periodic = periodic;
	return t;
}


void swtimer_delete(struct swtimer *timer) {
	timer->func = NULL;
	timer->args = NULL;
	timer->timeout_ticks = 0;
}

// init swtimer array.
void swtimer_init() {
	struct swtimer *t = &timer_list[0];
	for(int i = 0; i < MAX_SWTIMER; i++) {
		t->func = NULL;
		t->args = NULL;
		t->timeout_ticks = 0;
		t->periodic = 0;
		t++;
	}
}