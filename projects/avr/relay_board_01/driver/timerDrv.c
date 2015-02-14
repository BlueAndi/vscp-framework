/* The MIT License (MIT)
 * 
 * Copyright (c) 2014 - 2015, Andreas Merkle
 * http://www.blue-andi.de
 * vscp@blue-andi.de
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 */

/*******************************************************************************
    DESCRIPTION
*******************************************************************************/
/**
@brief  Hardware timer driver
@file   timerDrv.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
@see timerDrv.h

@section svn Subversion
$Author: amerkle $
$Rev: 449 $
$Date: 2015-01-05 20:23:52 +0100 (Mo, 05 Jan 2015) $
*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "timerDrv.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <stdlib.h>

/*******************************************************************************
    COMPILER SWITCHES
*******************************************************************************/

/*******************************************************************************
    CONSTANTS
*******************************************************************************/

#if (16000000UL == F_CPU)

#if (1 == TIMERDRV_TICK_IN_MS)

/* CPU clock = 16 MHz
 * Prescaler = 8
 * Timer clock = CPU clock / Prescaler = 2000 kHz
 * => Timer increase every 1 / 2000 kHz = 0,5 us
 * => Overflow happens after 256, therefore every (256 * 4 us) = 128 us
 *
 * Generate a interrupt every 100 us, that means the interrupt shall be 28 us faster.
 * => 28 us / 0,5 us = 56
 * => Preload the counter register with 56.
 *
 * Now increase every 10th interrupt the tick counter to get a 1 ms tick.
 */

/* Prescaler of 64 */
#define TIMERDRV_PRESCALER          ((0 << CS02) | (1 << CS01) | (0 << CS00))

/* Preload value */
#define TIMERDRV_PRELOAD_VALUE      56

/* Timer threshold value */
#define TIMERDRV_THRESHOLD_VALUE    10

#endif  /* (10 == TIMERDRV_TICK_IN_MS) */

#if (10 == TIMERDRV_TICK_IN_MS)

/* CPU clock = 16 MHz
 * Prescaler = 64
 * Timer clock = CPU clock / Prescaler = 250 kHz
 * => Timer increase every 1 / 250 kHz = 4 us
 * => Overflow happens after 256, therefore every (256 * 4 us) = 1,024 ms
 *
 * Generate a interrupt every 1 ms, that means the interrupt shall be 0,024 ms faster.
 * => 0,024 ms / 4 us = 6
 * => Preload the counter register with 6.
 *
 * Now increase every 10th interrupt the tick counter to get a 10 ms tick.
 */

/* Prescaler of 64 */
#define TIMERDRV_PRESCALER          ((0 << CS02) | (1 << CS01) | (1 << CS00))

/* Preload value */
#define TIMERDRV_PRELOAD_VALUE      6

/* Timer threshold value */
#define TIMERDRV_THRESHOLD_VALUE    10

#endif  /* (10 == TIMERDRV_TICK_IN_MS) */

#else

#error CPU clock not supported!

#endif

/*******************************************************************************
    MACROS
*******************************************************************************/

/*******************************************************************************
    TYPES AND STRUCTURES
*******************************************************************************/

/*******************************************************************************
    PROTOTYPES
*******************************************************************************/

/*******************************************************************************
    LOCAL VARIABLES
*******************************************************************************/

/** Tick counter */
static volatile uint8_t             timerDrv_tickCounter    = 0;

/** Timer tick count callback */
static volatile timerDrv_Callback   timerDrv_cb             = NULL;

/** Timer interrupt counter; counted per interrupt */
static volatile uint8_t             timerDrv_intrCounter    = 0;

/*******************************************************************************
    GLOBAL VARIABLES
*******************************************************************************/

/******************************************************************************\
    GLOBAL FUNCTIONS
*******************************************************************************/

/**
 * This function initialize the module (static initialization). Don't call
 * other functions before this function is executed.
 */
extern void timerDrv_init(void)
{
    timerDrv_tickCounter    = 0;
    timerDrv_cb             = NULL;
    timerDrv_intrCounter    = 0;

    /* Set prescaler */

#if defined(__AVR_AT90CAN32__) || defined(__AVR_AT90CAN64__) || defined (__AVR_AT90CAN128__)
    TCCR0A = TIMERDRV_PRESCALER;
#else
    TCCR0B = TIMERDRV_PRESCALER;
#endif

    /* Clear counter */
    TCNT0 = TIMERDRV_PRELOAD_VALUE;

    /* Enable overflow interrupt */
    TIMSK0 |= (1 << TOIE0);

    return;
}

/**
 * This function set the callback function which will be called every tick
 * in the timer ISR.
 *
 * @param[in]   cb  Callback
 */
extern void timerDrv_setCallback(timerDrv_Callback cb)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        timerDrv_cb = cb;
    }

    return;
}

/**
 * This function returns the tick counter value.
 *
 * @return  Tick counter value
 */
extern uint8_t  timerDrv_getTickCount(void)
{
    return timerDrv_tickCounter;
}

/**
 * This function sleeps the given time in ticks. Important: The calling task
 * will be blocked until sleep time is over.
 *
 * @param[in]   sleepTime   Time to sleep in ticks
 */
extern void timerDrv_sleep(uint8_t sleepTime)
{
    uint8_t endTime     = 0;

    endTime = timerDrv_tickCounter + sleepTime;

    if (endTime < timerDrv_tickCounter)
    {
        /* Wait for overflow first */
        while(timerDrv_tickCounter > endTime)
        {
            /* Wait */
            ;
        }
    }

    while(timerDrv_tickCounter < endTime);

    return;
}

/*******************************************************************************
    LOCAL FUNCTIONS
*******************************************************************************/

/**
 * Interrupt service routine for overflow interrupt timer 0.
 */
ISR(TIMER0_OVF_vect)
{
    ++timerDrv_intrCounter;

    if (TIMERDRV_THRESHOLD_VALUE <= timerDrv_intrCounter)
    {
        /* Increase tick counter */
        ++timerDrv_tickCounter;

        /* Any callback to call? */
         if (NULL != timerDrv_cb)
         {
            timerDrv_cb();
         }

         timerDrv_intrCounter  = 0;
    }

    /* Preload counter */
    TCNT0 = TIMERDRV_PRELOAD_VALUE;
}
