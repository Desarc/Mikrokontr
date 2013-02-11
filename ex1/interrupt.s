.include "io.s" 
SR_GM =   16  
	
.text
.globl  _start
_start:
	/* Set registers */
	lddpc r0, piob_ptr      	/* PIOB address */
	lddpc r1, pioc_ptr      	/* PIOC address */
	lddpc r2, intc_ptr		/* INTC address */
	lddpc r8, setOn_ptr

	/* Set LEDS */
	st.w r1[AVR32_PIO_PER], r8      /* Enable PIOC pins */
        st.w r1[AVR32_PIO_OER], r8      /* Set PIOC to output */
	lddpc r11, min_ptr      	/* Load min LED vector value */
	lddpc r12, max_ptr       	/* Load max LED vector value */
	mov r4, r12			/* Set LED status */
	mov r3, 0b10000000
	st.w r1[AVR32_PIO_SODR], r3     /* Start with the leftmost LED */
        
	/* Set buttons */
        st.w r0[AVR32_PIO_PER], r8      /* Enable pins on PIOB by setting PER high */
        st.w r0[AVR32_PIO_PUER], r8     /* Enable PUER on PIOB (enable input) */
        st.w r0[AVR32_PIO_IER], r8      /* Enable all interrupts for PIOB */
        lddpc r5, button1_ptr   	/* Button 1 vector */
        lddpc r6, button2_ptr   	/* Button 2 vector */
	mov r3, 0b00111111
	st.w r0[AVR32_PIO_IDR], r3 	/* Disable buttons 3-8 */

	/* Set up interrupt */
	mov r3, 0b00000000		
	mtsr 4, r3			/* Set EVBA to 0 */
	mov r3, interrupt_routine	
	st.w r2[AVR32_INTC_IPR14], r3	/* Set autovector to interrupt_routine */
        mov r10, 0			/* Set register to check the button state to avoid jumps */
	csrf SR_GM			/* Turn on interrupts */
	rjmp loop
	
interrupt_routine:	
	/* Check the state of r10 to avoid double interrupts on button presses */
	cp.w r10, 1
	breq skip
	mov r10, 1
	 
	/* Min and max LED values to avoid out of bounds */
	lddpc r11, min_ptr 
	lddpc r12, max_ptr
	
	/* Check button status */
        ld.w r7, r0[AVR32_PIO_PDSR]     
        cp.w r5, r7             	/* Check if button left */
        breq left
        cp.w r6, r7             	/* Check if button right */
        breq right
	rjmp return

return:
	ld.w r3, r0[AVR32_PIO_ISR]	/* Reading ISR to enable new interrupts */
	rete

skip:					/* We skip every second interrupt */
	mov r10, 0
	rjmp return

left:
        lsl r4, 1               	/* Shift left to enable previous LED */
        cp.w r4, r12           		/* Check if out of bounds */
        brle turn_on
        mov r4, r11            		/* Max wrap around */
        rjmp turn_on
        
right:
        lsr r4, 1       		/* Shift right to enable next LED */
        cp.w r4, r11   			/* Check if out of bounds */
        brge turn_on
        mov r4, r12            		/* Min wrap around */
        rjmp turn_on

turn_on:
        st.w r1[AVR32_PIO_CODR], r8    	/* Set LEDs off */
        st.w r1[AVR32_PIO_SODR], r4    	/* Turns on the selected LED */
        mov r9, 0xffff
        rjmp intr_sleep

intr_sleep:				/* Busy waiting for debouncing */
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

.data
button1 = 0x3fcfffff
button2 = 0x7fceffff
setOn = 0xffffffff
max = 0b10000000
min = 0b00000001
