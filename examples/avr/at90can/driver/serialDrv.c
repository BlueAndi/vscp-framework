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
@file   serialDrv.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
@see serialDrv.h

*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "serialDrv.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

/*******************************************************************************
    COMPILER SWITCHES
*******************************************************************************/

/*******************************************************************************
    CONSTANTS
*******************************************************************************/

/*******************************************************************************
    MACROS
*******************************************************************************/

/*******************************************************************************
    TYPES AND STRUCTURES
*******************************************************************************/

/** This type contains the low and high byte for baudrate configuration. */
typedef struct
{
    uint8_t high;   /**< High byte */
    uint8_t low;    /**< Low byte */
    uint8_t u2x;    /**< Shall U2X be set? */

} serialDrv_UBRR;

/** This type defines a cyclic buffer context. */
typedef struct
{
    uint8_t*            storage;    /**< Cyclic buffer storage */
    uint8_t             size;       /**< Size of the storage */
    volatile uint8_t    read;       /**< Read index */
    volatile uint8_t    write;      /**< Write index */

} serialDrv_CyclicBuffer;

/** This type defines a USART context. */
typedef struct
{
    serialDrv_CyclicBuffer  rxCyclicBuffer; /**< Cyclic buffer for receive data */
    serialDrv_CyclicBuffer  txCyclicBuffer; /**< Cyclic buffer to transmit data */

    volatile BOOL       rxOverflow; /**< Flag to signal a rx overflow */
    volatile uint8_t    rxStatus;   /**< USART rx status */

} serialDrv_Usart;

/*******************************************************************************
    PROTOTYPES
*******************************************************************************/

static uint8_t  serialDrv_readCyclicBuffer(serialDrv_CyclicBuffer* cyclicBuffer, uint8_t* const data);
static uint8_t  serialDrv_writeCyclicBuffer(serialDrv_CyclicBuffer* cyclicBuffer, uint8_t data);

/*******************************************************************************
    LOCAL VARIABLES
*******************************************************************************/

/** All USART context */
static serialDrv_Usart  serialDrv_gUsart[SERIALDRV_USART_MAX];

/** List with the UBRR register values for all supported baudrates. */
static serialDrv_UBRR   serialDrv_ubbr_baudrate[SERIALDRV_BAUD_MAX] =
{
#define BAUD    2400
#include <util/setbaud.h>
    {   UBRRH_VALUE, UBRRL_VALUE, USE_2X    },
#undef BAUD

#define BAUD    4800
#include <util/setbaud.h>
    {   UBRRH_VALUE, UBRRL_VALUE, USE_2X    },
#undef BAUD

#define BAUD    9600
#include <util/setbaud.h>
    {   UBRRH_VALUE, UBRRL_VALUE, USE_2X    },
#undef BAUD

#define BAUD    14400
#include <util/setbaud.h>
    {   UBRRH_VALUE, UBRRL_VALUE, USE_2X    },
#undef BAUD

#define BAUD    19200
#include <util/setbaud.h>
    {   UBRRH_VALUE, UBRRL_VALUE, USE_2X    },
#undef BAUD

#define BAUD    28800
#include <util/setbaud.h>
    {   UBRRH_VALUE, UBRRL_VALUE, USE_2X    },
#undef BAUD

#define BAUD    38400
#include <util/setbaud.h>
    {   UBRRH_VALUE, UBRRL_VALUE, USE_2X    },
#undef BAUD

#define BAUD    57600
#include <util/setbaud.h>
    {   UBRRH_VALUE, UBRRL_VALUE, USE_2X    },
#undef BAUD

#define BAUD    76800
#include <util/setbaud.h>
    {   UBRRH_VALUE, UBRRL_VALUE, USE_2X    },
#undef BAUD

#define BAUD    115200
#include <util/setbaud.h>
    {   UBRRH_VALUE, UBRRL_VALUE, USE_2X    },
#undef BAUD

#define BAUD    234000
#include <util/setbaud.h>
    {   UBRRH_VALUE, UBRRL_VALUE, USE_2X    },
#undef BAUD

#define BAUD    250000
#include <util/setbaud.h>
    {   UBRRH_VALUE, UBRRL_VALUE, USE_2X    },
#undef BAUD

};
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
extern void serialDrv_init(void)
{
    /* Reset every USART context */
    memset(serialDrv_gUsart, 0, sizeof(serialDrv_gUsart));

    return;
}

/**
 * This function returns the handle to the requested USART.
 *
 * @param[in] usart USART number
 * @return Handle to USART
 */
extern serialDrv_Handle serialDrv_get(SERIALDRV_USART usart)
{
    if (SERIALDRV_USART_MAX <= usart)
    {
        return SERIALDRV_INVALID_HANDLE;
    }

    return &serialDrv_gUsart[usart];
}

/**
 * This function initializes the USART.
 *
 * @param[in] hSerial       Handle to USART
 * @param[in] rxStorage     Storage for received data
 * @param[in] rxStorageSize Size of the received data storage
 * @param[in] txStorage     Storage for transmit data
 * @param[in] txStorageSize Size of the transmit data storage
 */
extern void serialDrv_initUsart(serialDrv_Handle hSerial, uint8_t* const rxStorage, uint8_t rxStorageSize, uint8_t* const txStorage, uint8_t txStorageSize)
{
    serialDrv_Usart*    usart   = NULL;

    if ((SERIALDRV_INVALID_HANDLE == hSerial) ||
        (NULL == rxStorage) ||
        (2 > rxStorageSize) ||
        (NULL == txStorage) ||
        (2 > txStorageSize))
    {
        return;
    }

    usart = (serialDrv_Usart*)hSerial;

    usart->rxCyclicBuffer.storage   = rxStorage;
    usart->rxCyclicBuffer.size      = rxStorageSize;
    usart->txCyclicBuffer.storage   = txStorage;
    usart->txCyclicBuffer.size      = txStorageSize;

    return;
}

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
extern SERIALDRV_RET serialDrv_open(serialDrv_Handle hSerial, SERIALDRV_BAUD baudrate, SERIALDRV_DATA data, SERIALDRV_PARITY parity, SERIALDRV_STOPBIT stopBits)
{
    serialDrv_Usart*    usart       = NULL;
    uint8_t             regUBRRH    = 0;
    uint8_t             regUBRRL    = 0;
    uint8_t             regUCSRC    = 0;
    uint8_t             regUCSRB    = 0;

    if (SERIALDRV_INVALID_HANDLE == hSerial)
    {
        return SERIALDRV_RET_ENULL;
    }

    usart = (serialDrv_Usart*)hSerial;

    /* Not initialized? */
    if ((NULL == usart->rxCyclicBuffer.storage) ||
        (NULL == usart->txCyclicBuffer.storage))
    {
        return SERIALDRV_RET_ERROR;
    }

    /* Configure baudrate */
    regUBRRH = serialDrv_ubbr_baudrate[baudrate].high;
    regUBRRL = serialDrv_ubbr_baudrate[baudrate].low;

    /* Configure data format */
    switch(data)
    {
    case SERIALDRV_DATA_5:
        regUCSRC |= (0 << UCSZ2) | (0 << UCSZ1) | (0 << UCSZ0);
        break;

    case SERIALDRV_DATA_6:
        regUCSRC |= (0 << UCSZ2) | (0 << UCSZ1) | (1 << UCSZ0);
        break;

    case SERIALDRV_DATA_7:
        regUCSRC |= (0 << UCSZ2) | (1 << UCSZ1) | (0 << UCSZ0);
        break;

    case SERIALDRV_DATA_8:
        regUCSRC |= (0 << UCSZ2) | (1 << UCSZ1) | (1 << UCSZ0);
        break;

    case SERIALDRV_DATA_9:
        regUCSRC |= (1 << UCSZ2) | (1 << UCSZ1) | (1 << UCSZ0);
        break;

    default:
        return SERIALDRV_RET_ERROR;
    }

    /* Configure parity */
    switch(parity)
    {
    case SERIALDRV_PARITY_NONE:
        regUCSRC |= (0 << UPM1) | (0 << UPM0);
        break;

    case SERIALDRV_PARITY_EVEN:
        regUCSRC |= (1 << UPM1) | (0 << UPM0);
        break;

    case SERIALDRV_PARITY_ODD:
        regUCSRC |= (1 << UPM1) | (1 << UPM0);
        break;

    default:
        return SERIALDRV_RET_ERROR;
    }

    /* Configure number of stop bits */
    switch(stopBits)
    {
    case SERIALDRV_STOPBIT_1:
        regUCSRC |= (0 << USBS);
        break;

    case SERIALDRV_STOPBIT_2:
        regUCSRC |= (1 << USBS);
        break;

    default:
        return SERIALDRV_RET_ERROR;
    }

    /* Enable receiver, transmitter and the receive interrupt */
    regUCSRB = (1 << RXEN) | (1 << TXEN) | (1 << RXCIE);

    if (usart == &serialDrv_gUsart[SERIALDRV_USART_0])
    {
        UBRR0H = regUBRRH;
        UBRR0L = regUBRRL;

        /* Normal mode? */
        if (0 == serialDrv_ubbr_baudrate[baudrate].u2x)
        {
            UCSR0A &= ~_BV(U2X);
        }
        else
        {
            UCSR0A |= _BV(U2X);
        }

        UCSR0C = regUCSRC;
        UCSR0B = regUCSRB;
    }
    else if (usart == &serialDrv_gUsart[SERIALDRV_USART_1])
    {
        UBRR1H = regUBRRH;
        UBRR1L = regUBRRL;

        /* Normal mode? */
        if (0 == serialDrv_ubbr_baudrate[baudrate].u2x)
        {
            UCSR1A &= ~_BV(U2X);
        }
        else
        {
            UCSR1A |= _BV(U2X);
        }

        UCSR1C = regUCSRC;
        UCSR1B = regUCSRB;
    }
    else
    {
        return SERIALDRV_RET_ERROR;
    }

    return SERIALDRV_RET_OK;
}

/**
 * This function close a serial port.
 *
 * @param[in] hSerial   Handle to USART
 */
extern void serialDrv_close(serialDrv_Handle hSerial)
{
    serialDrv_Usart*    usart   = NULL;

    if (SERIALDRV_INVALID_HANDLE == hSerial)
    {
        return;
    }

    usart = (serialDrv_Usart*)hSerial;

    /* Disable interrupts */
    if (usart == &serialDrv_gUsart[SERIALDRV_USART_0])
    {
        UCSR0B &= ~((1 << RXCIE0) | (1 << TXCIE0));
    }
    else if (usart == &serialDrv_gUsart[SERIALDRV_USART_1])
    {
        UCSR1B &= ~((1 << RXCIE1) | (1 << TXCIE1));
    }

    return;
}

/**
 * Read a single byte from the serial port.
 *
 * @param[in]   hSerial   Handle to USART
 * @param[out]  data      Data
 * @return Number of read data
 */
extern uint8_t  serialDrv_read(serialDrv_Handle hSerial, uint8_t* const data)
{
    serialDrv_Usart*    usart   = NULL;

    if (SERIALDRV_INVALID_HANDLE == hSerial)
    {
        return 0;
    }

    usart = (serialDrv_Usart*)hSerial;

    return serialDrv_readCyclicBuffer(&usart->rxCyclicBuffer, data);
}

/**
 * Write a single byte to the serial port.
 *
 * @param[in]   hSerial   Handle to USART
 * @param[out]  data      Data
 * @return Number of written data
 */
extern uint8_t  serialDrv_write(serialDrv_Handle hSerial, uint8_t data)
{
    serialDrv_Usart*    usart   = NULL;
    uint8_t             count   = 0;

    if (SERIALDRV_INVALID_HANDLE == hSerial)
    {
        return 0;
    }

    usart = (serialDrv_Usart*)hSerial;

    if (usart == &serialDrv_gUsart[SERIALDRV_USART_0])
    {
        if (0 == (UCSR0B & (1 << TXCIE0)))
        {
            /* Enable transmit interrupt */
            UCSR0B |= (1 << TXCIE0);

            /* Write data to transmit register. */
            UDR0 = data;
        }
        else
        {
            count = serialDrv_writeCyclicBuffer(&usart->txCyclicBuffer, data);
        }
    }
    else if (usart == &serialDrv_gUsart[SERIALDRV_USART_1])
    {
        if (0 == (UCSR1B & (1 << TXCIE1)))
        {
            /* Enable transmit interrupt */
            UCSR1B |= (1 << TXCIE1);

            /* Write data to transmit register. */
            UDR1 = data;
        }
        else
        {
            count = serialDrv_writeCyclicBuffer(&usart->txCyclicBuffer, data);
        }
    }

    return count;
}

/**
 * This function clears the receive buffer.
 *
 * @param[in]   hSerial   Handle to USART
 */
extern void serialDrv_clearRx(serialDrv_Handle hSerial)
{
    serialDrv_Usart*    usart   = NULL;

    if (SERIALDRV_INVALID_HANDLE == hSerial)
    {
        return;
    }

    usart = (serialDrv_Usart*)hSerial;

    usart->rxCyclicBuffer.write = usart->rxCyclicBuffer.read;

    return;
}

/**
 * This function returns the rx overflow flag.
 *
 * @param[in]   hSerial   Handle to USART
 * @return Status
 * @retval FALSE    No rx overflow
 * @retval TRUE     Rx overflow happened
 */
extern BOOL serialDrv_isRxOverflow(serialDrv_Handle hSerial)
{
    serialDrv_Usart*    usart       = NULL;
    BOOL                rxOverflow  = FALSE;

    if (SERIALDRV_INVALID_HANDLE == hSerial)
    {
        return FALSE;
    }

    usart = (serialDrv_Usart*)hSerial;

    rxOverflow = usart->rxOverflow;

    /* Clear overflow flag */
    usart->rxOverflow = FALSE;

    return rxOverflow;
}

/*******************************************************************************
    LOCAL FUNCTIONS
*******************************************************************************/

/**
 * Read a single byte from the cyclic buffer.
 *
 * @param[in]   cyclicBuffer    Cyclic buffer context
 * @param[out]  data            Data
 * @return Number of read data
 */
static uint8_t  serialDrv_readCyclicBuffer(serialDrv_CyclicBuffer* cyclicBuffer, uint8_t* const data)
{
    uint8_t count   = 0;

    if ((NULL == cyclicBuffer) ||
        (NULL == data))
    {
        return 0;
    }

    /* Cyclic buffer is not empty? */
    if (cyclicBuffer->read != cyclicBuffer->write)
    {
        /* Calculate next read index */
        uint8_t nextRead    = cyclicBuffer->read + 1;

        /* Check for wrap around */
        if (cyclicBuffer->size <= nextRead)
        {
            nextRead = 0;
        }

        /* Get value */
        *data = cyclicBuffer->storage[cyclicBuffer->read];
        ++count;

        /* Move read index */
        cyclicBuffer->read = nextRead;
    }

    return count;
}

/**
 * Write a single byte to the cyclic buffer.
 *
 * @param[in]   cyclicBuffer    Cyclic buffer context
 * @param[out]  data            Data
 * @return Number of written data
 */
static uint8_t  serialDrv_writeCyclicBuffer(serialDrv_CyclicBuffer* cyclicBuffer, uint8_t data)
{
    uint8_t count       = 0;
    uint8_t nextWrite   = 0;

    if (NULL == cyclicBuffer)
    {
        return 0;
    }

    /* Calculate next write index */
    nextWrite = cyclicBuffer->write + 1;

    /* Check for wrap around */
    if (cyclicBuffer->size <= nextWrite)
    {
        nextWrite = 0;
    }

    /* Cyclic buffer is not full? */
    if (cyclicBuffer->read != nextWrite)
    {
        /* Set value */
        cyclicBuffer->storage[cyclicBuffer->write] = data;
        ++count;

        /* Move write index */
        cyclicBuffer->write = nextWrite;
    }

    return count;
}

/**
 * Interrupt service routine for a received byte from USART 0.
 */
ISR(USART0_RX_vect)
{
    uint8_t             status  = UCSR0A;   /* Read status register (must be read before data register!) */
    uint8_t             data    = UDR0;     /* Read data register */
    serialDrv_Usart*    usart   = &serialDrv_gUsart[SERIALDRV_USART_0];

    /* Only error bits are interesting */
    status &= (1 << FE0) | (1 << DOR0) | (1 << UPE0);
    usart->rxStatus = status;

    /* Put it to the serial driver */
    if (1 != serialDrv_writeCyclicBuffer(&usart->rxCyclicBuffer, data))
    {
        usart->rxOverflow = TRUE;
    }
}

/**
 * Interrupt service routine for a received byte from USART 1.
 */
ISR(USART1_RX_vect)
{
    uint8_t             status  = UCSR1A;   /* Read status register (must be read before data register!) */
    uint8_t             data    = UDR1;     /* Read data register */
    serialDrv_Usart*    usart   = &serialDrv_gUsart[SERIALDRV_USART_1];

    /* Only error bits are interesting */
    status &= (1 << FE1) | (1 << DOR1) | (1 << UPE1);
    usart->rxStatus = status;

    /* Put it to the serial driver */
    if (1 != serialDrv_writeCyclicBuffer(&usart->rxCyclicBuffer, data))
    {
        usart->rxOverflow = TRUE;
    }
}

/**
 * Interrupt service routine for a transmitted byte to USART 0.
 */
ISR(USART0_TX_vect)
{
    uint8_t             data    = 0;
    serialDrv_Usart*    usart   = &serialDrv_gUsart[SERIALDRV_USART_0];

    /* Get data from the serial driver */
    if (0 == serialDrv_readCyclicBuffer(&usart->txCyclicBuffer, &data))
    {
        /* Nothing to send, disable transmit interrupt */
        UCSR0B &= ~(1 << TXCIE0);
    }
    else
    {
        /* Write data to data register */
        UDR0 = data;
    }
}

/**
 * Interrupt service routine for a transmitted byte to USART 1.
 */
ISR(USART1_TX_vect)
{
    uint8_t             data    = 0;
    serialDrv_Usart*    usart   = &serialDrv_gUsart[SERIALDRV_USART_1];

    /* Get data from the serial driver */
    if (0 == serialDrv_readCyclicBuffer(&usart->txCyclicBuffer, &data))
    {
        /* Nothing to send, disable transmit interrupt */
        UCSR1B &= ~(1 << TXCIE1);
    }
    else
    {
        /* Write data to data register */
        UDR1 = data;
    }
}
