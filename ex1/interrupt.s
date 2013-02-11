/*****************************************************************************************
* TDT4258: Exercise 1 by group 9, Stian Habbestad, Øyvin Richardsen and Sandor Zeestraten
*****************************************************************************************/
.include "io.s" 
SR_GM =   16  
	
.text
.globl  _start
_start:
	/* Set registers */
	lddpc r0, piob_ptr      	/* PIOB address */
	lddpc r1, pioc_ptr      	/* PIOC address */
	lddpc r2, intc_ptr		/* INTC address */
	lddpc r8, setOn_ptr		/* 0xffffffff */
        lddpc r5, button1_ptr   	/* Button 1 vector */
        lddpc r6, button2_ptr   	/* Button 2 vector */

	/* Set LEDS */
	st.w r1[AVR32_PIO_PER], r8      /* Enable PIOC pins */
        st.w r1[AVR32_PIO_OER], r8      /* Set PIOC to output */
	mov r11, 0b00000001      	/* Load min LED vector value */
	mov r12, 0b10000000       	/* Load max LED vector value */
	mov r4, r12			/* Set LED status */
	st.w r1[AVR32_PIO_SODR], r12     /* Start with the leftmost LED */
        
	/* Set buttons */
        st.w r0[AVR32_PIO_PER], r8      /* Enable pins on PIOB by setting PER high */
        st.w r0[AVR32_PIO_PUER], r8     /* Enable PUER on PIOB (enable input) */
        st.w r0[AVR32_PIO_IER], r8      /* Enable interrupts for all buttons */
	mov r3, 0b00111111		/* Disable interrupt for buttons 3-8 */
	st.w r0[AVR32_PIO_IDR], r3 	

	/* Set up interrupt */
	mov r3, 0b00000000		
	mtsr 4, r3			/* Set EVBA offset to 0 */
	mov r3, interrupt_routine	/* Set autovector to interrupt_routine */
	st.w r2[AVR32_INTC_IPR14], r3	
	csrf SR_GM			/* Turn on interrupts */
	rjmp loop
	
interrupt_routine:	
	/* Load which button was pressed */ 
        ld.w r7, r0[AVR32_PIO_PDSR]  	/* Read Pin Data Status Register */   

	/* Avoid double interrups on single button press */
	cp.w r10, r7 			/* Check if button was pressed on last interrupt */
	breq return			/* Skip to avoid double interrupt */
	mov r10, r7 			/* Note which button was pressed for next time */			
	/* Check which button was pressed */
        cp.w r5, r7             	/* Check if button left */
        breq left
        cp.w r6, r7             	/* Check if button right */
        breq right

return:
	/* Read ISR and return to normal state after interrupt */
	ld.w r3, r0[AVR32_PIO_ISR]	/* Read Interrupt Status Register to allow new interrupts */
	rete

left:
	/* Move LED to the left */
        lsl r4, 1               	/* Shift left to enable previous LED */
        cp.w r4, r12           		/* Check if out of bounds */
        brle turn_on
        mov r4, r11            		/* Max wrap around */
        rjmp turn_on
        
right:
	/* Move LED to the right */
        lsr r4, 1       		/* Shift right to enable next LED */
        cp.w r4, r11   			/* Check if out of bounds */
        brge turn_on
        mov r4, r12            		/* Min wrap around */
        rjmp turn_on

turn_on:
	/* Turn on the selected LED */
        st.w r1[AVR32_PIO_CODR], r8    	/* Turn off all LED's */
        st.w r1[AVR32_PIO_SODR], r4    	/* Turn on the LED specified in r4 */
        mov r9, 0xffff			/* Countdown value used for debouncing */
        rjmp intr_sleep

intr_sleep:				
	/* Debouncing by counting down from 0xffff to 0 */
        sub r9, 1
        cp.w r9, 0
        breq return
	rjmp intr_sleep
	
loop:	
	/* Sleep loop */
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

.data
button1 = 0x3fcfffff
button2 = 0x7fceffff
setOn = 0xffffffff
