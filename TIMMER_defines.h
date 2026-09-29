#ifndef __TIMER_H
#define __TIMER_H

#include <lpc214x.h>
#include "types.h"


#define TIMER0_CHNO 4
#define TIMER0_LED  7


/*------------------------------------------------
              TIMER0 DELAY FUNCTIONS
------------------------------------------------*/

void Init_timer0(void)
{
    T0TCR = 1<<1;
    T0MCR = 1<<2;
}


void tdelay_us(u32 us)
{
    T0MR0 = us;
    T0PR  = 14;
    T0TC  = 0;
    T0TCR = 1<<0;

    while(T0MR0 != T0TC);
}


void tdelay_ms(u32 ms)
{
    T0MR0 = ms;
    T0PR  = 14999;
    T0TC  = 0;
    T0TCR = 1<<0;

    while(T0MR0 != T0TC);
}


void tdelay_s(u32 s)
{
    T0MR0 = s;
    T0PR  = 14999999;
    T0TC  = 0;
    T0TCR = 1<<0;

    while(T0MR0 != T0TC);
}


/*------------------------------------------------
           TIMER MATCH TOGGLE FUNCTION
------------------------------------------------*/

void timer0_enable(void)
{
    T0TCR = 1<<1;

    PINSEL0 |= 2<<(3*2);

    T0MCR = 1<<1;

    T0EMR = 1<<0 | 3<<4;
}


/*------------------------------------------------
             TIMER0 INTERRUPT
------------------------------------------------*/

void timer0_isr(void)__irq
{
    IOPIN0 ^= 1<<TIMER0_LED;

    T0IR = 1<<0;

    VICVectAddr = 0;
}


void enable_timer_interrupt(void)
{
    T0TCR = 1<<1;

    T0MCR = 1<<0 | 1<<1;

    T0PR  = 15000000-1;

    T0MR0 = 2;

    VICIntEnable = 1<<TIMER0_CHNO;

    VICVectCntl0 = 1<<5 | TIMER0_CHNO;

    VICVectAddr0 = (u32)timer0_isr;

    IODIR0 |= 1<<TIMER0_LED;

    T0TCR = 1<<0;
}


/*------------------------------------------------
                 COUNTER MODE
------------------------------------------------*/

void Init_counter0(void)
{
    T0TCR = 1<<1;

    T0CTCR = 2<<0;

    T0CTCR |= 1<<2;

    PINSEL0 |= 2<<(4*2);

    T0TCR = 1<<0;
}


u32 Read_counter0(void)
{
    return T0TC;
}

#endif