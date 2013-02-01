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

        lddpc r0, piob_ptr      	/*Laster minneadressen til piob inn i r0*/
        lddpc r8, setOn_ptr    		/* 0xffffffff */
        
        st.w r0[AVR32_PIO_PER], r8      /* Enable pins on PIOB by setting PER high */
        st.w r0[AVR32_PIO_PUER], r8     /* enable PUER on PIOB */
        st.w r0[AVR32_PIO_IER], r8      /* enable all interrupts for PIOB */
	mov r1, 0x3f
        st.w r0[AVR32_PIO_IDR], r1 	/* disable buttons 3-8 */

	mov r4, 0		
	mtsr 4, r4			/* set EVBA to 0 */
	
	lddpc r2, intc_ptr
	mov r4, interruptRoutine	
	
	st.w r2[AVR32_INTC_IPR14], r4	/* set autovector to interruptRoutine */
	
	csrf SR_GM			/* turn on interrupts */

	lddpc r2, pioc_ptr      	/* PIOC address */
	st.w r2[AVR32_PIO_PER], r8      /* enable PIOC pins */
        st.w r2[AVR32_PIO_OER], r8      /* set PIOC to output */
        st.w r2[AVR32_PIO_CODR], r8    	/* turn off all LEDs */

	lddpc r9, max_ptr       	/* load max LED vector value */
        mov r11, r9
        lddpc r12, min_ptr      	/* load min LED vector value */
        st.w r2[AVR32_PIO_SODR], r9     /* start with the leftmost LED */
	rjmp sleep

interruptRoutine:
	/*st.w --sp, r10			/* save stack */	
		
        lddpc r3, button1_ptr   	/* button 1 vector */
        lddpc r7, button2_ptr   	/* button 2 vector */
	
        ld.w r4, r0[AVR32_PIO_PDSR]     /* load button status */
        cp.w r3, r4             	/* check if button 1 (left) */
        breq left
        cp.w r7, r4             	/* check if button 2 (right)*/
        breq right

left:
        lsl r9, 1               /* shift left to enable previous LED */
        cp.w r9, r11            /* check if out of bounds */
        brle turnOn
        mov r9, r12             /* max wrap around */
        rjmp turnOn
        
right:
        lsr r9, 1               /* shift right to enable next LED */
        cp.w r9, r12            /* check if out of bounds */
        brge turnOn
        mov r9, r11             /* min wrap around */
        rjmp turnOn
turnOn:
        st.w r2[AVR32_PIO_CODR], r8         /* set LEDs off */
        st.w r2[AVR32_PIO_SODR], r9         /* turns on the selected LED */
        mov r10, 0xffff
        rjmp intr_sleep_start

intr_sleep_start:		/* busy waiting to avoid bounce */
        sub r10, 1
        cp.w r10, 0
        breq return
	rjmp intr_sleep_start

return:
	ld.w r2, r0[AVR32_PIO_ISR]		/* reading ISR to enable new interrupts */
	/*ld.w r10, sp++		/* load stack */
	rete

sleep:
	sleep 1


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
	
eksempelvariabel:       /* eksempel på variabel med startverdien 5 */
        .int 5
button1 = 0x3fcfffff
button2 = 0x7fceffff
setOn = 0xffffffff
vector = 0x00fff0ff
max = 0x00000080
min = 0x00000001
