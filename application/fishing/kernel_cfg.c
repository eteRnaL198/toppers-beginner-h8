/* Configured with [-s tmpfile1 -c -obj -cpu h8 -system akih8_3069f ] */

#include "kernel_cfg.h"
#include "kernel_id.h"

#if TKERNEL_PRVER >= 0x1040
#define CFG_INTHDR_ENTRY(inthdr) INTHDR_ENTRY(inthdr)
#define CFG_EXCHDR_ENTRY(exchdr) EXCHDR_ENTRY(exchdr)
#define CFG_INT_ENTRY(inthdr) INT_ENTRY(inthdr)
#define CFG_EXC_ENTRY(exchdr) EXC_ENTRY(exchdr)
#else
#error "This configuration file has no compatibility with TOPPERS/JSP rel 1.3 or earlier."
#endif

#ifndef __EMPTY_LABEL
#define __EMPTY_LABEL(x,y) x y[0]
#endif

#if TKERNEL_PRID != 0x0001u /* TOPPERS/JSP */
#error "You can not use this configuration file without TOPPERS/JSP"
#endif

	/* User specified include files*/
#include "fishing.h"
#include "hw_timer.h"
#include "timer.h"
#include "hw_serial.h"
#include "serial.h"
#include "logtask.h"


	/* Object initializer [task] */

#define TNUM_TSKID 5

const ID _kernel_tmax_tskid = (TMIN_TSKID + TNUM_TSKID - 1);

static __STK_UNIT __stack_INIT_TASK[__TCOUNT_STK_UNIT(1024)];
static __STK_UNIT __stack_FISH_TASK[__TCOUNT_STK_UNIT(1024)];
static __STK_UNIT __stack_ANGLER_TASK[__TCOUNT_STK_UNIT(1024)];
static __STK_UNIT __stack_PLAY_DISPLAY_TASK[__TCOUNT_STK_UNIT(1024)];
static __STK_UNIT __stack_LOGTASK[__TCOUNT_STK_UNIT(LOGTASK_STACK_SIZE)];

const TINIB _kernel_tinib_table[TNUM_TSKID] = {
	{0x00u | 0x02u, (VP_INT)(0), (FP)(init_task), INT_PRIORITY(8), __TROUND_STK_UNIT(1024), __stack_INIT_TASK, TA_NULL, (FP)(NULL)},
	{0, (VP_INT)(0), (FP)(fish_task), INT_PRIORITY(9), __TROUND_STK_UNIT(1024), __stack_FISH_TASK, TA_NULL, (FP)(NULL)},
	{0, (VP_INT)(0), (FP)(angler_task), INT_PRIORITY(9), __TROUND_STK_UNIT(1024), __stack_ANGLER_TASK, TA_NULL, (FP)(NULL)},
	{0, (VP_INT)(0), (FP)(play_display_task), INT_PRIORITY(10), __TROUND_STK_UNIT(1024), __stack_PLAY_DISPLAY_TASK, TA_NULL, (FP)(NULL)},
	{0x00u | 0x02u, (VP_INT)(( VP_INT ) 2), (FP)(logtask), INT_PRIORITY(LOGTASK_PRIORITY), __TROUND_STK_UNIT(LOGTASK_STACK_SIZE), __stack_LOGTASK, TA_NULL, (FP)(NULL)}
};

const ID _kernel_torder_table[TNUM_TSKID] = {1,2,3,4,5};

TCB _kernel_tcb_table[TNUM_TSKID];


	/* Object initializer [semaphore] */

#define TNUM_SEMID 4

const ID _kernel_tmax_semid = (TMIN_SEMID + TNUM_SEMID - 1);

const SEMINIB _kernel_seminib_table[TNUM_SEMID] = {
	{1, 0, 1},
	{1, 1, 1},
	{1, 0, 1},
	{1, 1, 1}
};

SEMCB _kernel_semcb_table[TNUM_SEMID];


	/* Object initializer [eventflag] */

#define TNUM_FLGID 1

const ID _kernel_tmax_flgid = (TMIN_FLGID + TNUM_FLGID - 1);

const FLGINIB _kernel_flginib_table[TNUM_FLGID] = {
	{0x00u | 0x00u | 0x04u, 0}
};

FLGCB _kernel_flgcb_table[TNUM_FLGID];


	/* Object initializer [dataqueue] */

#define TNUM_DTQID 1

const ID _kernel_tmax_dtqid = (TMIN_DTQID + TNUM_DTQID - 1);

#if (0) > 0
  static VP_INT __dataqueue_LAND_DTQ[0];
#else
  #define __dataqueue_LAND_DTQ NULL
#endif

const DTQINIB _kernel_dtqinib_table[TNUM_DTQID] = {
	{0, 0, __dataqueue_LAND_DTQ}
};

DTQCB _kernel_dtqcb_table[TNUM_DTQID];


	/* Object initializer [mailbox] */

#define TNUM_MBXID 0

const ID _kernel_tmax_mbxid = (TMIN_MBXID + TNUM_MBXID - 1);

__EMPTY_LABEL(const MBXINIB, _kernel_mbxinib_table);
__EMPTY_LABEL(MBXCB, _kernel_mbxcb_table);


	/* Object initializer [mempfix] */

#define TNUM_MPFID 0

const ID _kernel_tmax_mpfid = (TMIN_MPFID + TNUM_MPFID - 1);

__EMPTY_LABEL(const MPFINIB, _kernel_mpfinib_table);
__EMPTY_LABEL(MPFCB, _kernel_mpfcb_table);


	/* Object initializer [cyclic] */

#define TNUM_CYCID 3

const ID _kernel_tmax_cycid = (TMIN_CYCID + TNUM_CYCID - 1);

const CYCINIB _kernel_cycinib_table[TNUM_CYCID] = {
	{0x00u | 0x02u,0,(FP)(switch_handler),100,0},
	{0x00u | 0x02u,0,(FP)(screen_handler),500,0},
	{0x00u | 0x02u,0,(FP)(hunger_handler),4000,0}
};

CYCCB _kernel_cyccb_table[TNUM_CYCID];


	/* Object initializer [interrupt] */

#define TNUM_INHNO 5

const UINT _kernel_tnum_inhno = TNUM_INHNO;

CFG_INTHDR_ENTRY(timer_handler);
CFG_INTHDR_ENTRY(sio_in_handler);
CFG_INTHDR_ENTRY(sio_out_handler);
CFG_INTHDR_ENTRY(sio_in2_handler);
CFG_INTHDR_ENTRY(sio_out2_handler);

const INHINIB _kernel_inhinib_table[TNUM_INHNO] = {
	{24,0,(FP)CFG_INT_ENTRY(timer_handler)},
	{53,0,(FP)CFG_INT_ENTRY(sio_in_handler)},
	{54,0,(FP)CFG_INT_ENTRY(sio_out_handler)},
	{57,0,(FP)CFG_INT_ENTRY(sio_in2_handler)},
	{58,0,(FP)CFG_INT_ENTRY(sio_out2_handler)}
};


	/* Object initializer [exception] */

#define TNUM_EXCNO 0

const UINT _kernel_tnum_excno = TNUM_EXCNO;

__EMPTY_LABEL(const EXCINIB, _kernel_excinib_table);
	/* Initialization handler */

void
_kernel_call_inirtn(void)
{
	timer_initialize( (VP_INT)(0) );
	serial_initialize( (VP_INT)(0) );
}

void
_kernel_call_terrtn(void)
{
	timer_terminate( (VP_INT)(0) );
}

	/* Object initialization routine */

void
_kernel_object_initialize(void)
{
	_kernel_task_initialize();
	_kernel_semaphore_initialize();
	_kernel_eventflag_initialize();
	_kernel_dataqueue_initialize();
	_kernel_cyclic_initialize();
	_kernel_interrupt_initialize();
}

TMEVTN   _kernel_tmevt_heap[TNUM_TSKID + TNUM_CYCID];

	/* Variables for kernel checker */
static const UW _checker_magic_number = 0x01234567;

