/******************************************************************************
 *	
 * Øving 1 UCSysDes
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

        lddpc r0, piob_ptr      /*Laster minneadressen til pioa inn i r0*/
        lddpc r8, setOn_ptr     /* 0xffffffff */
        
        mov r1, AVR32_PIO_PER   /* PIO enable bits */
        st.w r0[r1], r8         /*Enable pins on PIOA by setting PER high */

        mov r1, AVR32_PIO_PUER  /* PIO pull-up enable bits */
        st.w r0[r1], r8         /* set enable */

        lddpc r2, pioc_ptr      /* PIOC address */
        mov r1, AVR32_PIO_PER   /* PIO enable bits */
        
        st.w r2[r1], r8         /* set enable */
        
        mov r1, AVR32_PIO_OER   /* PIOC set input/output */

        st.w r2[r1], r8         /* set output */
        
        mov r1, AVR32_PIO_CODR  /* LED enable bits */

        st.w r2[r1], r8         /* set enable */

        mov r1, AVR32_PIO_PDSR  /* button status bits */
        mov r5, AVR32_PIO_SODR  /* LEDS on bits */
        mov r6, AVR32_PIO_CODR  /* LEDS off bits */
        lddpc r3, button1_ptr   /* button 1 vector */
        lddpc r7, button2_ptr   /* button 2 vector */
        
        st.w r2[r6], r8         /* set LEDs off */
        lddpc r9, max_ptr       /* load max LED vector value */
        mov r11, r9
        lddpc r12, min_ptr      /* load min LED vector value */
        st.w r2[r5], r9         /* start with the leftmost LED */
loop:   
        ld.w r4, r0[r1]         /* load button status */
        cp.w r3, r4             /* check if button 1 (left) */
        breq left
        cp.w r7, r4             /* check if button 2 (right)*/
        breq right
        rjmp loop
left:
        st.w r2[r6], r8         /* set LEDs off */
        lsl r9, 1               /* shift left to enable previous LED */
        cp.w r9, r11            /* check if out of bounds */
        brle turnOn
        mov r9, r12             /* max wrap around */
        rjmp turnOn
        
right:
        st.w r2[r6], r8         /* set LEDs off */
        lsr r9, 1               /* shift right to enable next LED */
        cp.w r9, r12            /* check if out of bounds */
        brge turnOn
        mov r9, r11             /* min wrap around */
        rjmp turnOn
turnOn:
        st.w r2[r5], r9         /* turns on the selected LED */
        rjmp loop

        


pioc_ptr:
        .int AVR32_PIOC

piob_ptr:
        .int AVR32_PIOB

pioa_ptr:
        .int AVR32_PIOA

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
max = 0x00000080
min = 0x00000001
