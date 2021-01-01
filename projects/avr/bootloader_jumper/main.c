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
@brief  Main entry point
@file   main.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
This module contains the main entry point.

*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "system.h"
#include <stdint.h>
#include "hw.h"
#include "util/delay.h"

/*******************************************************************************
    COMPILER SWITCHES
*******************************************************************************/

/*******************************************************************************
    CONSTANTS
*******************************************************************************/

/** Application section start address */
#define MAIN_APPLICATION_START_ADDR (0x0000u)

/*******************************************************************************
    MACROS
*******************************************************************************/

/** Set interrupt vectors to bootloader section */
#define MAIN_ISR_VECTOR_TO_BL()  \
{                                \
    uint8_t value   = MCUCR;     \
    MCUCR = value | _BV(IVCE);   \
    MCUCR = value | _BV(IVSEL);  \
}

/** Set interrupt vectors to application section */
#define MAIN_ISR_VECTOR_TO_APP() \
{                                \
    uint8_t value   = MCUCR;     \
    MCUCR = value | _BV(IVCE);   \
    MCUCR = value & ~_BV(IVSEL); \
}

/*******************************************************************************
    TYPES AND STRUCTURES
*******************************************************************************/

/** This type defines the supported function return values. */
typedef enum
{
    MAIN_RET_OK = 0,    /**< Successful */
    MAIN_RET_ERROR,     /**< Failed */
    MAIN_RET_ENULL      /**< Unexpected NULL pointer */

} MAIN_RET;

/*******************************************************************************
    PROTOTYPES
*******************************************************************************/

extern void main_smoothStatusLamp(void);

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
 * Main entry point.
 *
 * @return Not used
 */
int main(void)
{
    void    (*jumpToApp)(void)   = MAIN_APPLICATION_START_ADDR;

    /* Initialize the hardware. This is very important to do at first, because
     * it disables the interrupts and the watchdog.
     */
    hw_init();

    MAIN_ISR_VECTOR_TO_BL();

    for(;;)
    {
        /* Raise the status lamp brightness up and down */
        main_smoothStatusLamp();
    }    

    MAIN_ISR_VECTOR_TO_APP();

    /* Jump to application */
    jumpToApp();

    return 0;
}

/*******************************************************************************
    LOCAL FUNCTIONS
*******************************************************************************/

/**
 * This function raises the lamp brightness slowly up and down once.
 */
extern void main_smoothStatusLamp(void)
{
    uint8_t level       = 0;
    uint8_t changeLevel = 0;
    BOOL    up          = FALSE;
    BOOL    quit        = FALSE;

    do
    {
        /* Start from begin? */
        if (0 == level)
        {
            HW_DISABLE_STATUS_LED();

            if (TRUE == up)
            {
                ++changeLevel;

                if (255 == changeLevel)
                {
                    up = FALSE;

                    /* Quit */
                    quit = TRUE;
                }
            }
            else
            {
                --changeLevel;

                if (1 == changeLevel)
                {
                    up = TRUE;
                }
            }
        }

        /* Level reached to change the LED status? */
        if (changeLevel == level)
        {
            HW_ENABLE_STATUS_LED();
        }

        /* Minimum time which a LED state is shown. */
        _delay_us(10);

        level++;
    }
    while(FALSE == quit);

    return;
}
