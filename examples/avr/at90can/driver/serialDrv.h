/* The MIT License (MIT)
 *
 * Copyright (c) 2014 - 2024 Andreas Merkle
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
@brief  Serial driver
@file   serialDrv.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
This module provides an serial driver (USART).

*******************************************************************************/
/** @defgroup serialDrv Serial driver
 * This module provides an serial driver (USART).
 *
 * @{
 */

/*
 * Don't forget to set JAVADOC_AUTOBRIEF to YES in the doxygen file to generate
 * a correct module description.
*/

#ifndef __SERIALDRV_H__
#define __SERIALDRV_H__

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "system.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
    COMPILER SWITCHES
*******************************************************************************/

/*******************************************************************************
    CONSTANTS
*******************************************************************************/

/** Invalid handle value */
#define SERIALDRV_INVALID_HANDLE    ((serialDrv_Handle)NULL)

/*******************************************************************************
    MACROS
*******************************************************************************/

/*******************************************************************************
    TYPES AND STRUCTURES
*******************************************************************************/

/** This type defines the function return values. */
typedef enum
{
    SERIALDRV_RET_OK = 0,   /**< Successful */
    SERIALDRV_RET_ERROR,    /**< Failed */
    SERIALDRV_RET_ENULL     /**< Unexpected NULL pointer */

} SERIALDRV_RET;

/** This type defines the different USART's of the controller. */
typedef enum
{
    SERIALDRV_USART_0 = 0,  /**< USART 0 */
    SERIALDRV_USART_1,      /**< USART 1 */
    SERIALDRV_USART_MAX     /**< Max. number of USARTs */

} SERIALDRV_USART;

/** This type defines the different supported baudrates. */
typedef enum
{
    SERIALDRV_BAUD_2400 = 0,    /**< 2400 Baud */
    SERIALDRV_BAUD_4800,        /**< 4800 Baud */
    SERIALDRV_BAUD_9600,        /**< 9600 Baud */
    SERIALDRV_BAUD_14400,       /**< 14400 Baud */
    SERIALDRV_BAUD_19200,       /**< 19200 Baud */
    SERIALDRV_BAUD_28800,       /**< 28800 Baud */
    SERIALDRV_BAUD_38400,       /**< 38400 Baud */
    SERIALDRV_BAUD_57600,       /**< 57600 Baud */
    SERIALDRV_BAUD_76800,       /**< 76800 Baud */
    SERIALDRV_BAUD_115200,      /**< 115200 Baud */
    SERIALDRV_BAUD_230400,      /**< 230400 Baud */
    SERIALDRV_BAUD_250000,      /**< 250000 Baud */
    SERIALDRV_BAUD_MAX          /**< Max. number of baudrates */

} SERIALDRV_BAUD;

/** This type defines the different supported number of bits per data word. */
typedef enum
{
   SERIALDRV_DATA_5 = 0,    /**< 5 bit per data word */
   SERIALDRV_DATA_6,        /**< 6 bit per data word */
   SERIALDRV_DATA_7,        /**< 7 bit per data word */
   SERIALDRV_DATA_8,        /**< 8 bit per data word */
   SERIALDRV_DATA_9,        /**< 9 bit per data word */
   SERIALDRV_DATA_MAX       /**< Max. number of different data words */

} SERIALDRV_DATA;

/** This type defines the different supported parities. */
typedef enum
{
    SERIALDRV_PARITY_NONE = 0,  /**< None parity */
    SERIALDRV_PARITY_EVEN,      /**< Even parity */
    SERIALDRV_PARITY_ODD,       /**< Odd parity */
    SERIALDRV_PARITY_MAX        /**< Max. number of parities */

} SERIALDRV_PARITY;

/** This type defines the different supported number of stopbits. */
typedef enum
{
    SERIALDRV_STOPBIT_1 = 0,    /**< 1 stopbit */
    SERIALDRV_STOPBIT_2,        /**< 2 stopbit */
    SERIALDRV_STOPBIT_MAX       /**< Max. number of stopbits */

} SERIALDRV_STOPBIT;

/** This type defines the possible receive errors. */
typedef enum
{
    SERIALDRV_ERROR_NO_ERROR        = 0x00, /**< No error */
    SERIALDRV_ERROR_PARITY_ERROR    = 0x01, /**< Parity error */
    SERIALDRV_ERROR_DATA_OVERRUN    = 0x02, /**< Data overrun */
    SERIALDRV_ERROR_FRAME_ERROR     = 0x04  /**< Frame error */

} SERIALDRV_ERROR;

/** This type defines a serial driver handle. */
typedef void*   serialDrv_Handle;

/*******************************************************************************
    VARIABLES
*******************************************************************************/

/*******************************************************************************
    FUNCTIONS
*******************************************************************************/

/**
 * This function initialize the module (static initialization). Don't call
 * other functions before this function is executed.
 */
extern void serialDrv_init(void);

/**
 * This function returns the handle to the requested USART.
 *
 * @param[in] usart USART number
 * @return Handle to USART
 */
extern serialDrv_Handle serialDrv_get(SERIALDRV_USART usart);

/**
 * This function initializes the USART.
 *
 * @param[in] hSerial       Handle to USART
 * @param[in] rxStorage     Storage for received data
 * @param[in] rxStorageSize Size of the received data storage
 * @param[in] txStorage     Storage for transmit data
 * @param[in] txStorageSize Size of the transmit data storage
 */
extern void serialDrv_initUsart(serialDrv_Handle hSerial, uint8_t* const rxStorage, uint8_t rxStorageSize, uint8_t* const txStorage, uint8_t txStorageSize);

/**
 * This function open a serial port.
 *
 * @param[in] hSerial   Handle to USART
 * @param[in] baudrate  Baudrate
 * @param[in] data      Bit per data word
 * @param[in] parity    Parity
 * @param[in] stopBits  Number of stop bits
 * @return Status
 */
extern SERIALDRV_RET serialDrv_open(serialDrv_Handle hSerial, SERIALDRV_BAUD baudrate, SERIALDRV_DATA data, SERIALDRV_PARITY parity, SERIALDRV_STOPBIT stopBits);

/**
 * This function close a serial port.
 *
 * @param[in] hSerial   Handle to USART
 */
extern void serialDrv_close(serialDrv_Handle hSerial);

/**
 * Read a single byte from the serial port.
 *
 * @param[in]   hSerial   Handle to USART
 * @param[out]  data      Data
 * @return Number of read data
 */
extern uint8_t  serialDrv_read(serialDrv_Handle hSerial, uint8_t* const data);

/**
 * Write a single byte to the serial port.
 *
 * @param[in]   hSerial   Handle to USART
 * @param[out]  data      Data
 * @return Number of written data
 */
extern uint8_t  serialDrv_write(serialDrv_Handle hSerial, uint8_t data);

/**
 * This function clears the receive buffer.
 *
 * @param[in]   hSerial   Handle to USART
 */
extern void serialDrv_clearRx(serialDrv_Handle hSerial);

/**
 * This function returns the rx overflow flag.
 *
 * @param[in]   hSerial   Handle to USART
 * @return Status
 * @retval FALSE    No rx overflow
 * @retval TRUE     Rx overflow happened
 */
extern BOOL serialDrv_isRxOverflow(serialDrv_Handle hSerial);

#ifdef __cplusplus
}
#endif

#endif  /* __SERIALDRV_H__ */

/** @} */

