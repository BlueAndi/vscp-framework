/* The MIT License (MIT)
 *
 * Copyright (c) 2014 - 2016, Andreas Merkle
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
@brief  This module initializes the hardware.
@file   hw.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
@see hw.h

*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "hw.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/*******************************************************************************
    COMPILER SWITCHES
*******************************************************************************/

/*******************************************************************************
    CONSTANTS
*******************************************************************************/

/*******************************************************************************
    MACROS
*******************************************************************************/

/** Define the pin as input */
#define HW_DDR_INPUT(__pin)             (0 << (__pin))

/** Define the pin as output */
#define HW_DDR_OUTPUT(__pin)            (1 << (__pin))

/** Enable internal pull up resistor. (only valid if pin is defined as input) */
#define HW_PORT_INPUT_PULLUP(__pin)     (1 << (__pin))

/** Disable internal pull up resistor. (only valid if pin is defined as input) */
#define HW_PORT_INPUT_NO_PULLUP(__pin)  (0 << (__pin))

/** Set logic high. (only valid if pin is defined as output) */
#define HW_PORT_OUTPUT_H(__pin)         (1 << (__pin))

/** Set logic low. (only valid if pin is defined as output) */
#define HW_PORT_OUTPUT_L(__pin)         (0 << (__pin))

/*******************************************************************************
    TYPES AND STRUCTURES
*******************************************************************************/

/*******************************************************************************
    PROTOTYPES
*******************************************************************************/

/*******************************************************************************
    LOCAL VARIABLES
*******************************************************************************/

/*******************************************************************************
    GLOBAL VARIABLES
*******************************************************************************/

/*******************************************************************************
    GLOBAL FUNCTIONS
*******************************************************************************/

/**
 * This function initialize the module (static initialization). Don't call
 * other functions before this function is executed.
 */
extern  void    hw_init(void)
{
    /* Disable all interrupts */
    cli();

    /* Common information:
        The DDxn bit in the DDRx Register selects the direction of this pin. If DDxn is written
        logic one, Pxn is configured as an output pin. If DDxn is written logic zero, Pxn is configured
        as an input pin.

        If PORTxn is written logic one when the pin is configured as an input pin, the pull-up
        resistor is activated. To switch the pull-up resistor off, PORTxn has to be written logic
        zero or the pin has to be configured as an output pin. The port pins are tri-stated when a
        reset condition becomes active, even if no clocks are running.

        If PORTxn is written logic one when the pin is configured as an output pin, the port pin is
        driven high (one). If PORTxn is written logic zero when the pin is configured as an output
        pin, the port pin is driven low (zero).
    */

    /* Port A */
    DDRA    =   HW_DDR_INPUT(DDA7) |    /* Input - not connected */
                HW_DDR_INPUT(DDA6) |    /* Input - not connected */
                HW_DDR_INPUT(DDA5) |    /* Input - not connected */
                HW_DDR_INPUT(DDA4) |    /* Input - not connected */
                HW_DDR_INPUT(DDA3) |    /* Input - not connected */
                HW_DDR_INPUT(DDA2) |    /* Input - not connected */
                HW_DDR_INPUT(DDA1) |    /* Input - not connected */
                HW_DDR_INPUT(DDA0);     /* Button for VSCP segment initialization (low active) */

    PORTA   =   HW_PORT_INPUT_PULLUP(PA7) | /* Input - not connected */
                HW_PORT_INPUT_PULLUP(PA6) | /* Input - not connected */
                HW_PORT_INPUT_PULLUP(PA5) | /* Input - not connected */
                HW_PORT_INPUT_PULLUP(PA4) | /* Input - not connected */
                HW_PORT_INPUT_PULLUP(PA3) | /* Input - not connected */
                HW_PORT_INPUT_PULLUP(PA2) | /* Input - not connected */
                HW_PORT_INPUT_PULLUP(PA1) | /* Input - not connected */
                HW_PORT_INPUT_PULLUP(PA0);  /* Button for VSCP segment initialization (low active) */

    /* Port B */
    DDRB    =   HW_DDR_OUTPUT(DDB7) |   /* Output - not connected */
                HW_DDR_OUTPUT(DDB6) |   /* Output - not connected */
                HW_DDR_OUTPUT(DDB5) |   /* Output - not connected */
                HW_DDR_OUTPUT(DDB4) |   /* Output - not connected */
                HW_DDR_OUTPUT(DDB3) |   /* Output - not connected */
                HW_DDR_OUTPUT(DDB2) |   /* Output - not connected */
                HW_DDR_OUTPUT(DDB1) |   /* Output - not connected */
                HW_DDR_OUTPUT(DDB0);    /* Status LED (low active) */

    PORTB   =   HW_PORT_OUTPUT_L(PB7) | /* Output - not connected */
                HW_PORT_OUTPUT_L(PB6) | /* Output - not connected */
                HW_PORT_OUTPUT_L(PB5) | /* Output - not connected */
                HW_PORT_OUTPUT_L(PB4) | /* Output - not connected */
                HW_PORT_OUTPUT_L(PB3) | /* Output - not connected */
                HW_PORT_OUTPUT_L(PB2) | /* Output - not connected */
                HW_PORT_OUTPUT_L(PB1) | /* Output - not connected */
                HW_PORT_OUTPUT_H(PB0);  /* Status LED (low active) */

    /* Port C */
    DDRC    =   HW_DDR_OUTPUT(DDC7) |   /* Status LED (low active) */
                HW_DDR_OUTPUT(DDC6) |   /* Output - not connected */
                HW_DDR_OUTPUT(DDC5) |   /* Output - not connected */
                HW_DDR_OUTPUT(DDC4) |   /* Output - not connected */
                HW_DDR_OUTPUT(DDC3) |   /* Output - not connected */
                HW_DDR_OUTPUT(DDC2) |   /* Output - not connected */
                HW_DDR_OUTPUT(DDC1) |   /* Output - not connected */
                HW_DDR_OUTPUT(DDC0);    /* One wire */

    PORTC   =   HW_PORT_OUTPUT_H(PC7) | /* Status LED (low active) */
                HW_PORT_OUTPUT_L(PC6) | /* Output - not connected */
                HW_PORT_OUTPUT_L(PC5) | /* Output - not connected */
                HW_PORT_OUTPUT_L(PC4) | /* Output - not connected */
                HW_PORT_OUTPUT_L(PC3) | /* Output - not connected */
                HW_PORT_OUTPUT_L(PC2) | /* Output - not connected */
                HW_PORT_OUTPUT_L(PC1) | /* Output - not connected */
                HW_PORT_OUTPUT_L(PC0);  /* One wire */

    /* Port D */
    DDRD    =   HW_DDR_OUTPUT(DDD7) |   /* Output - not connected */
                HW_DDR_OUTPUT(DDD6) |   /* Output - not connected */
                HW_DDR_OUTPUT(DDD5) |   /* Output - not connected */
                HW_DDR_OUTPUT(DDD4) |   /* Output - not connected */
                HW_DDR_OUTPUT(DDD3) |   /* Serial tx */
                HW_DDR_INPUT(DDD2)  |   /* Serial rx */
                HW_DDR_OUTPUT(DDD1) |   /* Output - not connected */
                HW_DDR_OUTPUT(DDD0);    /* Output - not connected */

    PORTD   =   HW_PORT_OUTPUT_L(PD7)        |  /* Output - not connected */
                HW_PORT_OUTPUT_L(PD6)        |  /* Output - not connected */
                HW_PORT_OUTPUT_L(PD5)        |  /* Output - not connected */
                HW_PORT_OUTPUT_L(PD4)        |  /* Output - not connected */
                HW_PORT_OUTPUT_L(PD3)        |  /* Serial tx */
                HW_PORT_INPUT_NO_PULLUP(PD2) |  /* Serial rx */
                HW_PORT_OUTPUT_L(PD1)        |  /* Output - not connected */
                HW_PORT_OUTPUT_L(PD0);          /* Output - not connected */

    /* Port E */
    DDRE    =   HW_DDR_OUTPUT(DDE7) |   /* Output - not connected */
                HW_DDR_OUTPUT(DDE6) |   /* Output - not connected */
                HW_DDR_OUTPUT(DDE5) |   /* Output - not connected */
                HW_DDR_OUTPUT(DDE4) |   /* Output - not connected */
                HW_DDR_OUTPUT(DDE3) |   /* Output - not connected */
                HW_DDR_OUTPUT(DDE2) |   /* Output - not connected */
                HW_DDR_OUTPUT(DDE1) |   /* Output - not connected */
                HW_DDR_OUTPUT(DDE0);    /* Output - not connected */

    PORTE   =   HW_PORT_OUTPUT_L(PE7) | /* Output - not connected */
                HW_PORT_OUTPUT_L(PE6) | /* Output - not connected */
                HW_PORT_OUTPUT_L(PE5) | /* Output - not connected */
                HW_PORT_OUTPUT_L(PE4) | /* Output - not connected */
                HW_PORT_OUTPUT_L(PE3) | /* Output - not connected */
                HW_PORT_OUTPUT_L(PE2) | /* Output - not connected */
                HW_PORT_OUTPUT_L(PE1) | /* Output - not connected */
                HW_PORT_OUTPUT_L(PE0);  /* Output - not connected */

    /* Port F */
    DDRF    =   HW_DDR_OUTPUT(DDF7) |   /* Output - not connected */
                HW_DDR_OUTPUT(DDF6) |   /* Output - not connected */
                HW_DDR_OUTPUT(DDF5) |   /* Output - not connected */
                HW_DDR_OUTPUT(DDF4) |   /* Output - not connected */
                HW_DDR_OUTPUT(DDF3) |   /* Output - not connected */
                HW_DDR_OUTPUT(DDF2) |   /* Output - not connected */
                HW_DDR_OUTPUT(DDF1) |   /* Output - not connected */
                HW_DDR_OUTPUT(DDF0);    /* Output - not connected */

    PORTF   =   HW_PORT_OUTPUT_L(PF7) | /* Output - not connected */
                HW_PORT_OUTPUT_L(PF6) | /* Output - not connected */
                HW_PORT_OUTPUT_L(PF5) | /* Output - not connected */
                HW_PORT_OUTPUT_L(PF4) | /* Output - not connected */
                HW_PORT_OUTPUT_L(PF3) | /* Output - not connected */
                HW_PORT_OUTPUT_L(PF2) | /* Output - not connected */
                HW_PORT_OUTPUT_L(PF1) | /* Output - not connected */
                HW_PORT_OUTPUT_L(PF0);  /* Output - not connected */

    /* Note: If the Watchdog is accidentally enabled, for example by a runaway pointer or
     * brown-out condition, the device will be reset and the Watchdog Timer will stay enabled.
     * If the code is not set up to handle the Watchdog, this might lead to an eternal loop of
     * time-out resets. To avoid this situation, the application software should always clear the
     * Watchdog System Reset Flag (WDRF) and the WDE control bit in the initialization routine,
     * even if the Watchdog is not in use.
     */
    BIT_CLR(MCUSR, WDRF);
    BIT_CLR(WDTCR, WDE);

    return;
}

/**
 * Get the state of the segment initialization button.
 * Note that the button state is not debounced!
 *
 * @return Button state
 * @return FALSE    Button is released
 * @return TRUE     Button is pressed
 */
extern BOOL hw_getSegInitButtonState(void)
{
    /* Button is low active */
    return (0 == (PINA & _BV(PA0)) ? TRUE : FALSE);
}

/*******************************************************************************
    LOCAL FUNCTIONS
*******************************************************************************/

/*
 * CAUTION! Older AVRs will have the watchdog timer disabled on a reset.
 * For these older AVRs, doing a soft reset by enabling the watchdog is easy,
 * as the watchdog will then be disabled after the reset. On newer AVRs, once
 * the watchdog is enabled, then it stays enabled, even after a reset!
 * For these newer AVRs a function needs to be added to the .init3 section
 * (i.e. during the startup code, before main()) to disable the watchdog early
 * enough so it does not continually reset the AVR.
*/

/* Disable the watchdog in the .init3 phase before main() is called. */
void hw_disableWatchdog(void) \
__attribute__((naked)) \
__attribute__((section(".init3")));

/**
 * Disable watchdog before any watchdog interrupt can happen.
 * @see http://www.nongnu.org/avr-libc/user-manual/group__avr__watchdog.html
 *
 * Important note: Don't declare this function static, otherwise it will be
 * removed by the linker.
 */
void hw_disableWatchdog(void)
{
    /* Save MCUSR so that the main program can access it later */
    GPIOR0 = MCUSR;
    MCUSR = 0;
    wdt_disable();

    return;
}
