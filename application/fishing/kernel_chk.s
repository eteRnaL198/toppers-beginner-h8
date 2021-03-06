;	GCC For the Hitachi H8/300
;	By Hitachi America Ltd and Cygnus Support
; -O2

	.h8300h
	.file	"kernel_chk.c"
	.section .text
	.align 1
	.global _checker_function
_checker_function:
	push.l	er6
	mov.l	er7,er6
; #APP
	sTMAX_TPRI,(#16),(0)@
	sTMIN_TPRI,(#1),(0)@
	sTMAX_MPRI,(#16),(0)@
	sTMIN_MPRI,(#1),(0)@
	sTMAX_RELTIM,(#2147483647),(0)@
	squeue::next,(#4),(#0)@
	squeue::prev,(#4),(#4)@
	dtask_3,ANGLER_TASK@
	dtask_2,FISH_TASK@
	dtask_1,INIT_TASK@
	dtask_5,LOGTASK@
	dtask_4,PLAY_DISPLAY_TASK@
	s_kernel_tmax_tskid,(#4),(0)@
	s_kernel_tinib_table,(#32),(0)@
	stask_initialization_block::tskatr,(#4),(#0)@
	stask_initialization_block::exinf,(#4),(#4)@
	stask_initialization_block::task,(#4),(#8)@
	stask_initialization_block::ipriority,(#4),(#12)@
	stask_initialization_block::stksz,(#4),(#16)@
	stask_initialization_block::stk,(#4),(#20)@
	stask_initialization_block::texatr,(#4),(#24)@
	stask_initialization_block::texrtn,(#4),(#28)@
	dsemaphore_1,SERIAL_RCV_SEM1@
	dsemaphore_3,SERIAL_RCV_SEM2@
	dsemaphore_2,SERIAL_SND_SEM1@
	dsemaphore_4,SERIAL_SND_SEM2@
	s_kernel_tmax_semid,(#4),(0)@
	s_kernel_seminib_table,(#12),(0)@
	ssemaphore_initialization_block::sematr,(#4),(#0)@
	ssemaphore_initialization_block::isemcnt,(#4),(#4)@
	ssemaphore_initialization_block::maxsem,(#4),(#8)@
	deventflag_1,DONE_FLG@
	s_kernel_tmax_flgid,(#4),(0)@
	s_kernel_flginib_table,(#8),(0)@
	seventflag_initialization_block::flgatr,(#4),(#0)@
	seventflag_initialization_block::iflgptn,(#4),(#4)@
	ddataqueue_1,LAND_DTQ@
	s_kernel_tmax_dtqid,(#4),(0)@
	s_kernel_dtqinib_table,(#12),(0)@
	sdataqueue_initialization_block::dtqatr,(#4),(#0)@
	sdataqueue_initialization_block::dtqcnt,(#4),(#4)@
	sdataqueue_initialization_block::dtq,(#4),(#8)@
	s_kernel_tmax_mbxid,(#4),(0)@
	s_kernel_mbxinib_table,(#8),(0)@
	smailbox_initialization_block::mbxatr,(#4),(#0)@
	smailbox_initialization_block::maxmpri,(#4),(#4)@
	s_kernel_tmax_mpfid,(#4),(0)@
	s_kernel_mpfinib_table,(#16),(0)@
	sfixed_memorypool_initialization_block::mpfatr,(#4),(#0)@
	sfixed_memorypool_initialization_block::blksz,(#4),(#4)@
	sfixed_memorypool_initialization_block::mpf,(#4),(#8)@
	sfixed_memorypool_initialization_block::limit,(#4),(#12)@
	dcyclic_3,HUNGER_HANDLER@
	dcyclic_2,SCREEN_HANDLER@
	dcyclic_1,SWITCH_HANDLER@
	s_kernel_tmax_cycid,(#4),(0)@
	s_kernel_cycinib_table,(#20),(0)@
	scyclic_handler_initialization_block::cycatr,(#4),(#0)@
	scyclic_handler_initialization_block::exinf,(#4),(#4)@
	scyclic_handler_initialization_block::cychdr,(#4),(#8)@
	scyclic_handler_initialization_block::cyctim,(#4),(#12)@
	scyclic_handler_initialization_block::cycphs,(#4),(#16)@
	dinterrupt_0,24@
	dinterrupt_1,53@
	dinterrupt_2,54@
	dinterrupt_3,57@
	dinterrupt_4,58@
	s_kernel_tnum_inhno,(#4),(0)@
	s_kernel_inhinib_table,(#12),(0)@
	sinterrupt_handler_initialization_block::inhno,(#4),(#0)@
	sinterrupt_handler_initialization_block::inhatr,(#4),(#4)@
	sinterrupt_handler_initialization_block::inthdr,(#4),(#8)@
	s_kernel_tnum_excno,(#4),(0)@
	s_kernel_excinib_table,(#12),(0)@
	scpu_exception_handler_initialization_block::excno,(#4),(#0)@
	scpu_exception_handler_initialization_block::excatr,(#4),(#4)@
	scpu_exception_handler_initialization_block::exchdr,(#4),(#8)@
; #NO_APP
	pop.l	er6
	rts
	.end
	.ident
"GCC: (GNU) 3.0.2"
