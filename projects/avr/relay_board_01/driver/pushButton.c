/* The MIT License (MIT)
 *
 * Copyright (c) 2014 - 2019, Andreas Merkle
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
@file   pushButton.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
@see pushButton.h

*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "pushButton.h"
#include <avr/io.h>

/*******************************************************************************
    COMPILER SWITCHES
*******************************************************************************/

/*******************************************************************************
    CONSTANTS
*******************************************************************************/

/*******************************************************************************
    MACROS
*******************************************************************************/

/** Pushbutton 1 level */
#define PUSHBUTTON_PB1  ((PIND >> PD0) & 0x01)

/** Pushbutton 2 level */
#define PUSHBUTTON_PB2  ((PIND >> PD1) & 0x01)

/** Pushbutton 3 level */
#define PUSHBUTTON_PB3  ((PIND >> PD5) & 0x01)

/** Pushbutton 4 level */
#define PUSHBUTTON_PB4  ((PIND >> PD4) & 0x01)

/** Pushbutton 5 level */
#define PUSHBUTTON_PB5  ((PIND >> PD6) & 0x01)

/** Pushbutton 6 level */
#define PUSHBUTTON_PB6  ((PIND >> PD7) & 0x01)

/** Pushbutton 7 level */
#define PUSHBUTTON_PB7  ((PINB >> PB0) & 0x01)

/** Pushbutton 8 level */
#define PUSHBUTTON_PB8  ((PIND >> PD3) & 0x01)
    

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

/******************************************************************************\
    GLOBAL FUNCTIONS
*******************************************************************************/

/**
 * This function initialize the module (static initialization). Don't call
 * other functions before this function is executed.
 */
extern void pushButton_init(void)
{
    /* Nothing to do */

    return;
}

/**
 *   Read the current hardware status of all pushbuttons and return it.
 *
 *   MSB-------------------------------------------LSB
 *   | PB8 | PB7 | PB6 | PB5 | PB4 | PB3 | PB2 | PB1 |
 *   -------------------------------------------------
 *   A high bit means the pushbutton is pressed and a low bit means the
 *   pushbutton is released.
 *
 *   @return Status of the pushbuttons.
 */
extern uint8_t pushButton_readStatus(void)
{
    uint8_t status      =
        (PUSHBUTTON_PB8 << 7) |
        (PUSHBUTTON_PB7 << 6) |
        (PUSHBUTTON_PB6 << 5) |
        (PUSHBUTTON_PB5 << 4) |
        (PUSHBUTTON_PB4 << 3) |
        (PUSHBUTTON_PB3 << 2) |
        (PUSHBUTTON_PB2 << 1) |
        (PUSHBUTTON_PB1 << 0);

    /* Invert status, because the pushbuttons are low active. */
    status = (~status) & 0xff;

    return status;
}

/*******************************************************************************
    LOCAL FUNCTIONS
*******************************************************************************/
