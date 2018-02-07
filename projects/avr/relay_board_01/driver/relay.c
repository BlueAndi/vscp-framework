/* The MIT License (MIT)
 *
 * Copyright (c) 2014 - 2018, Andreas Merkle
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
@file   relay.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
@see relay.h

*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "relay.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/atomic.h>

/*******************************************************************************
    COMPILER SWITCHES
*******************************************************************************/

/*******************************************************************************
    CONSTANTS
*******************************************************************************/

/** Port where all relay related pins are connected to */
#define RELAY_PORT      PORTC

/** Shift register clear */
#define RELAY_PIN_SCL   PC3

/** Shift register clock pin */
#define RELAY_PIN_SCK   PC2

/** Storage register clock pin */
#define RELAY_PIN_RCK   PC1

/** Serial data pin */
#define RELAY_PIN_SER   PC0

/** PWM TOP value, calculated for a 20 kHz PWM */
#define RELAY_PWM_TOP   400

/** Minimum PWM value, as specified in the datasheet */
#define RELAY_PWM_MIN   3

#if (RELAY_PWM_MIN > RELAY_PWM_TOP)
#error The minimum resolution is 3.
#endif  /* (3 > RELAY_PWM_TOP) */

/** Time in 0,05 ms after changing from switching current to holding current */
#define RELAY_DURATION  (200 * 20)

/*******************************************************************************
    MACROS
*******************************************************************************/

/** This macro set the inverted duty cycle of the PWM, which is necessary
 * because of the PWM output is connected to the low active output enable
 * pin of the 74HC595 (serial to parallel shift register ic).
 */
#define RELAY_SET_DUTY_CYCLE(__dutyCycle)   do{ OCR1A = RELAY_PWM_TOP - (__dutyCycle); }while(0)

/*******************************************************************************
    TYPES AND STRUCTURES
*******************************************************************************/

/*******************************************************************************
    PROTOTYPES
*******************************************************************************/

static inline void relay_clearShiftRegister(void);
static inline void relay_setBit(uint8_t bit);
static inline void relay_store(void);

/*******************************************************************************
    LOCAL VARIABLES
*******************************************************************************/

/** Bitwise activate/deactivate the relays. Bit 0 corresponds to relay 1 and etc. */
static uint8_t          relay_stateBitField         = 0;

/** PWM value for switching a relay on */
static uint16_t         relay_switchingPwm          = RELAY_PWM_TOP;

/** PWM value for holding a relay on (60% of switching PWM) */
static uint16_t         relay_holdingPwm            = (RELAY_PWM_TOP * 3) / 5;

/** Change to switching current */
static volatile BOOL    relay_enableSwitchingPwm    = FALSE;

/** Relay enable mask. A 1 means the relay is enabled, a 0 the relay is disabled */
static uint8_t          relay_enableMask            = 0xff;

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
extern void relay_init(void)
{
    /* Clear shift register */
    relay_clearShiftRegister();

#if (16000000UL == F_CPU)

    /* Clear timer counter register */
    TCNT1 = 0;

    /* Configure timer control register A */
    TCCR1A =    (1 << COM1A1) | /* Clear OC1A on compare match when counting up, set when counting down */
                (0 << COM1A0) |
                (0 << WGM11) |  /* Set PWM, phase and frequency correct, ICR1 defines the TOP */
                (0 << WGM10);

    /* Configure timer control register B */
    TCCR1B =    (1 << WGM13) |  /* Set PWM, phase and frequency correct, ICR1 defines the TOP */
                (0 << WGM12) |
                (0 << CS12) |   /* Set prescaler to 1 */
                (0 << CS11) |
                (1 << CS10);

    /* Configure timer control register C */
    TCCR1C =    (1 << FOC1A);   /* Force output compare for channel A */

    /* Set PWM resolution */
    ICR1 = RELAY_PWM_TOP;

    /* Calculated PWM frequency:
     * f_pwm = f_clk_io / (2 * N * TOP)
     * f_pwm = 16 MHz / (2 * 1 * 400)
     * f_pwm = 20 kHz
     */

     /* Set holding current */
     RELAY_SET_DUTY_CYCLE(relay_holdingPwm);

     /* Enable overflow interrupt */
     TIMSK1 |= (1 << TOIE1);

#else

#error CPU clock not supported!

#endif

    return;
}

/**
 * Enable/Disable a relay.
 *
 * @param[in]   index       Index of the relay
 * @param[in]   enableIt    Enable/Disable it
 */
 void relay_enable(uint8_t index, BOOL enableIt)
 {
    if (RELAY_NUM > index)
    {
        if (FALSE == enableIt)
        {
            BIT_CLR(relay_enableMask, index);
        }
        else
        {
            BIT_SET(relay_enableMask, index);
        }
    }

    return;
 }

/**
 * This function returns the relay enable/disable status.
 *
 * @return Relay is enabled (TRUE) or not (FALSE)
 */
extern BOOL relay_isEnabled(uint8_t index)
{
    BOOL status = FALSE;

    if (RELAY_NUM > index)
    {
        status = IS_BIT_SET(relay_enableMask, index);
    }

    return status;
}

/**
 * Activate or deactivate a relay.
 *
 * @param[in]   index       Index of the relay
 * @param[in]   activate    Activate or deactivate it
 */
extern void relay_activate(uint8_t index, BOOL activate)
{
    /* The index must be valid and the relay must be enabled. */
    if ((RELAY_NUM > index) &&
        (TRUE == relay_isEnabled(index)))
    {
        uint8_t run                 = 0;
        BOOL    enableSwitchingPwm  = FALSE;
        BOOL    dirty               = FALSE;

        /* Deactivate a relay? */
        if (FALSE == activate)
        {
            /* Is relay activated? */
            if (TRUE == IS_BIT_SET(relay_stateBitField, index))
            {
                /* Deactivate relay */
                BIT_CLR(relay_stateBitField, index);

                dirty = TRUE;
            }
        }
        /* Activate a relay */
        else
        {
            /* Is relay deactivated? */
            if (FALSE == IS_BIT_SET(relay_stateBitField, index))
            {
                /* Activate relay */
                BIT_SET(relay_stateBitField, index);

                dirty = TRUE;

                /* Increase current to switching current */
                enableSwitchingPwm = TRUE;
            }
        }

        /* Anything to change? */
        if (TRUE == dirty)
        {
            /* Shift data from relay 1 to relay 8, depended on requested relay state. */
            for(run = 0; run < RELAY_NUM; ++run)
            {
                /* Relay disabled? */
                if (FALSE == IS_BIT_SET(relay_stateBitField, run))
                {
                    relay_setBit(0);
                }
                /* Relay enabled */
                else
                {
                    relay_setBit(1);
                }
            }

            ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
            {
                /* Latch the shift register to output */
                relay_store();

                /* Signal ISR to change to switching current */
                relay_enableSwitchingPwm = enableSwitchingPwm;
            }
        }
    }

    return;
}

/**
 * Is relay activated or not.
 *
 * @return Relay status
 * @retval FALSE    Relay is deactivated
 * @retval TRUE     Relay is activated
 */
extern BOOL relay_isActivated(uint8_t index)
{
    BOOL    status  = FALSE;

    if (RELAY_NUM > index)
    {
        status = IS_BIT_SET(relay_stateBitField, index);
    }

    return status;
}

/**
 * Set PWM switching value, so that the relay goes from off to on.
 *
 * @param[in]   value   Switching PWM value
 */
extern void relay_setSwitchingPwm(uint16_t value)
{
    /* Limit value */
    if (RELAY_PWM_TOP < value)
    {
        value = RELAY_PWM_TOP;
    }

    if (RELAY_PWM_MIN > value)
    {
        value = RELAY_PWM_MIN;
    }

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        relay_switchingPwm = value;

        /* Force changing the duty cycle */
        relay_enableSwitchingPwm = TRUE;
    }

    return;
}

/**
 * Set PWM holding value, so that the relay stays on.
 *
 * @param[in]   value   Holding PWM value
 */
extern void relay_setHoldingPwm(uint16_t value)
{
    /* Limit value */
    if (RELAY_PWM_TOP < value)
    {
        value = RELAY_PWM_TOP;
    }

    if (RELAY_PWM_MIN > value)
    {
        value = RELAY_PWM_MIN;
    }

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        relay_holdingPwm = value;

        /* Force changing the duty cycle */
        relay_enableSwitchingPwm = TRUE;
    }

    return;
}

/*******************************************************************************
    LOCAL FUNCTIONS
*******************************************************************************/

/**
 * Clear external shift register.
 */
static inline void relay_clearShiftRegister(void)
{
    /* SCL is low active */
    BIT_CLR(RELAY_PORT, RELAY_PIN_SCL);

    BIT_SET(RELAY_PORT, RELAY_PIN_SCK);
    BIT_CLR(RELAY_PORT, RELAY_PIN_SCK);

    BIT_SET(RELAY_PORT, RELAY_PIN_RCK);
    BIT_CLR(RELAY_PORT, RELAY_PIN_RCK);

    BIT_SET(RELAY_PORT, RELAY_PIN_SCL);

    return;
}

/**
 * Set a single bit in the external shift register.
 *
 * @param[in]   bit Data bit, which to set.
 */
static inline void relay_setBit(uint8_t bit)
{
    if (0 == bit)
    {
        BIT_CLR(RELAY_PORT, RELAY_PIN_SER);
    }
    else
    {
        BIT_SET(RELAY_PORT, RELAY_PIN_SER);
    }

    BIT_SET(RELAY_PORT, RELAY_PIN_SCK);
    BIT_CLR(RELAY_PORT, RELAY_PIN_SCK);

    return;
}

/**
 * Store the data in the external shift register and latch them to the output.
 */
static inline void relay_store(void)
{
    BIT_SET(RELAY_PORT, RELAY_PIN_RCK);
    BIT_CLR(RELAY_PORT, RELAY_PIN_RCK);

    return;
}

/**
 * Interrupt service routine will be called if the timer counter is at the BOTTOM.
 * It is used to change the PWM value.
 *
 * Period: PWM frequency
 * ISR is called every 50 us
 */
ISR(TIMER1_OVF_vect)
{
    static uint16_t duration    = 0;
    static BOOL     once        = FALSE;

    /* Relay switching current requested? */
    if (FALSE != relay_enableSwitchingPwm)
    {
        duration = RELAY_DURATION;

        relay_enableSwitchingPwm    = FALSE;
        once                        = TRUE;

        RELAY_SET_DUTY_CYCLE(relay_switchingPwm);
    }
    /* Count down? */
    else if (0 < duration)
    {
        --duration;
    }
    /* Relay holding current requested? */
    else if (FALSE != once)
    {
        once = FALSE;

        RELAY_SET_DUTY_CYCLE(relay_holdingPwm);
    }
}
