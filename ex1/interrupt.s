/******************************************************************************
 *	
 *Interrupt routine
 *	
 *****************************************************************************/

.include "io.s"  /* inkludere noen nyttige symboler (konstanter) */

/* Eksempel på hvordan sette symboler (se også io.s) */
SR_GM =   16  /* statusregisterflag "GM" er bit 16 */
	
/*****************************************************************************/
/* text-segment */
/* all programkoden må plasseres i dette segmentet */
	
.text
	
.globl  _start

_start: /* programutføring vil starte her */
	lddpc r0, piob_ptr      	/* PIOB address */
	lddpc r1, pioc_ptr      	/* PIOC address */
	lddpc r2, intc_ptr		/* INTC address */

	lddpc r8, setOn_ptr    		/* 0xffffffff */

	st.w r1[AVR32_PIO_PER], r8      /* enable PIOC pins */
        st.w r1[AVR32_PIO_OER], r8      /* set PIOC to output */
	st.w r1[AVR32_PIO_CODR], r8    	/* turn off all LEDs */
	lddpc r12, max_ptr       	/* load max LED vector value */
        mov r4, r12
        lddpc r11, min_ptr      	/* load min LED vector value */
	mov r3, 0b00100000
	st.w r1[AVR32_PIO_SODR], r3     /* start with the leftmost LED */
        
        st.w r0[AVR32_PIO_PER], r8      /* Enable pins on PIOB by setting PER high */
        st.w r0[AVR32_PIO_PUER], r8     /* enable PUER on PIOB (enable input) */


	
        st.w r0[AVR32_PIO_IER], r8      /* enable all interrupts for PIOB */
	mov r3,0b00111111
	st.w r0[AVR32_PIO_IDR], r3 	/* disable buttons 3-8 */

	mov r3, 0b00000000		
	mtsr 4, r3			/* set EVBA to 0 */
	
	mov r3, interrupt_routine	
	st.w r2[AVR32_INTC_IPR14], r3	/* set autovector to interrupt_routine */
	
        mov r10, 0

	csrf SR_GM			/* turn on interrupts */
	rjmp loop
	
interrupt_routine:	
	cp.w r10, 1			/* introducing an alternating state, */
	breq skip			/* since buttons generate two interrupts */
	mov r10, 1			/* on every push */
	
        lddpc r5, button1_ptr   	/* button 1 vector */
        lddpc r6, button2_ptr   	/* button 2 vector */
	lddpc r11, min_ptr      	/* load min LED vector value */
	lddpc r12, max_ptr       	/* load max LED vector value */
	
        ld.w r7, r0[AVR32_PIO_PDSR]     /* load button status */
        cp.w r5, r7             	/* check if button 1 (left) */
        breq left
        cp.w r6, r7             	/* check if button 2 (right)*/
        breq right
	rjmp return
return:
	ld.w r3, r0[AVR32_PIO_ISR]		/* reading ISR to enable new interrupts */
	rete

skip:						/* we skip every second interrupt */
	mov r10, 0
	rjmp return
left:
        lsl r4, 1               		/* shift left to enable previous LED */
        cp.w r4, r12            		/* check if out of bounds */
        brle turn_on
        mov r4, r11             		/* max wrap around */
        rjmp turn_on
        
right:
        lsr r4, 1              			/* shift right to enable next LED */
        cp.w r4, r11           			/* check if out of bounds */
        brge turn_on
        mov r4, r12             		/* min wrap around */
        rjmp turn_on
turn_on:
        st.w r1[AVR32_PIO_CODR], r8        	/* set LEDs off */
        st.w r1[AVR32_PIO_SODR], r4        	/* turns on the selected LED */
        mov r9, 0xffff
        rjmp intr_sleep

intr_sleep:					/* busy waiting for debouncing */
        sub r9, 1
        cp.w r9, 0
        breq return
	rjmp intr_sleep
	
loop:	
	sleep 1
	rjmp loop




piob_ptr:
        .int AVR32_PIOB

pioc_ptr:
        .int AVR32_PIOC
	
intc_ptr:
	.int AVR32_INTC

button1_ptr:
        .int button1

button2_ptr:
        .int button2

setOn_ptr:
        .int setOn

min_ptr:
        .int min

max_ptr:
        .int max

/*****************************************************************************/
/* data-segment */
/* alle dataområder som skal kunne skrives til må plasseres her */
	
.data
	
button1 = 0x3fcfffff
button2 = 0x7fceffff
setOn = 0xffffffff
max = 0b10000000
min = 0b00000001
