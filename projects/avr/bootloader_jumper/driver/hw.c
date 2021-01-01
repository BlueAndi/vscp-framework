/* The MIT License (MIT)
 *
 * Copyright (c) 2014 - 2021, Andreas Merkle
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

    /* Port B */
    DDRB    =   HW_DDR_INPUT(DDB7) |    /* XTAL2 */
                HW_DDR_INPUT(DDB6) |    /* XTAL1 */
                HW_DDR_OUTPUT(DDB5) |   /* SPI SCK */
                HW_DDR_INPUT(DDB4) |    /* SPI MISO */
                HW_DDR_OUTPUT(DDB3) |   /* SPI MOSI */
                HW_DDR_OUTPUT(DDB2) |   /* CAN /CS */
                HW_DDR_OUTPUT(DDB1) |   /* RELAY_PWM */
                HW_DDR_INPUT(DDB0);     /* MCU_IN7 */

    PORTB   =   HW_PORT_INPUT_NO_PULLUP(PB7) |  /* XTAL2 */
                HW_PORT_INPUT_NO_PULLUP(PB6) |  /* XTAL1 */
                HW_PORT_OUTPUT_L(PB5) |         /* SPI SCK */
                HW_PORT_INPUT_NO_PULLUP(PB4) |  /* SPI MISO */
                HW_PORT_OUTPUT_L(PB3) |         /* SPI MOSI */
                HW_PORT_OUTPUT_H(PB2) |         /* CAN /CS */
                HW_PORT_OUTPUT_H(PB1) |         /* RELAY_PWM
                                                 * A high prevents that the relays are enabled
                                                 * after power up for a short time.
                                                 */
                HW_PORT_INPUT_PULLUP(PB0);      /* MCU_IN7 */

    /* Port C */
    DDRC    =   HW_DDR_INPUT(DDC6) |    /* Reset */
                HW_DDR_OUTPUT(DDC5) |   /* LED_CTRL */
                HW_DDR_INPUT(DDC4) |    /* Boot jumper / segment initialization button */
                HW_DDR_OUTPUT(DDC3) |   /* RELAY_SCL */
                HW_DDR_OUTPUT(DDC2) |   /* RELAY_SCK */
                HW_DDR_OUTPUT(DDC1) |   /* RELAY_RCK */
                HW_DDR_OUTPUT(DDC0);    /* RELAY_SER */

    PORTC   =   HW_PORT_INPUT_NO_PULLUP(PC6) |  /* Reset */
                HW_PORT_OUTPUT_L(PC5) |         /* LED_CTRL */
                HW_PORT_INPUT_PULLUP(PC4) |     /* Boot jumper / segment initialization button */
                HW_PORT_OUTPUT_L(PC3) |         /* RELAY_SCL (clear shift register at startup) */
                HW_PORT_OUTPUT_L(PC2) |         /* RELAY_SCK */
                HW_PORT_OUTPUT_L(PC1) |         /* RELAY_RCK */
                HW_PORT_OUTPUT_L(PC0);          /* RELAY_SER */

    /* Port D */
    DDRD    =   HW_DDR_INPUT(DDD7) |    /* MCU_IN6 */
                HW_DDR_INPUT(DDD6) |    /* MCU_IN5 */
                HW_DDR_INPUT(DDD5) |    /* MCU_IN4 */
                HW_DDR_INPUT(DDD4) |    /* MCU_IN3 */
                HW_DDR_INPUT(DDD3) |    /* MCU_IN8 */
                HW_DDR_INPUT(DDD2) |    /* CAN_INT */
                HW_DDR_INPUT(DDD1) |    /* MCU_IN2 */
                HW_DDR_INPUT(DDD0);     /* MCU_IN1 */

    PORTD   =   HW_PORT_INPUT_PULLUP(PD7) |     /* MCU_IN6 */
                HW_PORT_INPUT_PULLUP(PD6) |     /* MCU_IN5 */
                HW_PORT_INPUT_PULLUP(PD5) |     /* MCU_IN4 */
                HW_PORT_INPUT_PULLUP(PD4) |     /* MCU_IN3 */
                HW_PORT_INPUT_PULLUP(PD3) |     /* MCU_IN8 */
                HW_PORT_INPUT_NO_PULLUP(PD2) |  /* CAN_INT */
                HW_PORT_INPUT_PULLUP(PD1) |     /* MCU_IN2 */
                HW_PORT_INPUT_PULLUP(PD0);      /* MCU_IN1 */

    /* Note: If the Watchdog is accidentally enabled, for example by a runaway pointer or
     * brown-out condition, the device will be reset and the Watchdog Timer will stay enabled.
     * If the code is not set up to handle the Watchdog, this might lead to an eternal loop of
     * time-out resets. To avoid this situation, the application software should always clear the
     * Watchdog System Reset Flag (WDRF) and the WDE control bit in the initialization routine,
     * even if the Watchdog is not in use.
     */
    BIT_CLR(MCUSR, WDRF);
    BIT_CLR(WDTCSR, WDE);

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
    return (0 == (PINC & _BV(PC4))) ? TRUE : FALSE;
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
