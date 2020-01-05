/* The MIT License (MIT)
 *
 * Copyright (c) 2014 - 2020, Andreas Merkle
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
@brief  VSCP persistent memory access driver
@file   vscp_ps_access.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
@see vscp_ps_access.h

*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "vscp_ps_access.h"
#include "fsl_dspi.h"

#include <stdlib.h>

/*******************************************************************************
    COMPILER SWITCHES
*******************************************************************************/

/*******************************************************************************
    CONSTANTS
*******************************************************************************/

/** Used SPI peripheral */
#define VSCP_PS_ACCESS_DSPI_MASTER_BASEADDR	SPI0

/** SPI clock source */
#define VSCP_PS_ACCESS_DSPI_CLKSRC			kCLOCK_BusClk

/** SPI clock frequency */
#define VSCP_PS_ACCESS_DSPI_CLK_FREQ		CLOCK_GetFreq(VSCP_PS_ACCESS_DSPI_CLKSRC)

/** SPI baudrate: 1 MBit/s */
#define VSCP_PS_ACCESS_DSPI_BAUDRATE		(1000000U)

/** SPI chip select */
#define VSCP_PS_ACCESS_DSPI_CS				kDSPI_Pcs0

/** EEPROM read transfer size in byte */
#define VSCP_PS_ACCESS_EEPROM_READ_TFR_SIZE	(3)

/*******************************************************************************
    MACROS
*******************************************************************************/

/*******************************************************************************
    TYPES AND STRUCTURES
*******************************************************************************/

/* Microchip 25AA02E48 SPI EEPROM commands */
typedef enum
{
	VSCP_PS_ACCESS_EEPROM_CMD_WRSR 	= 0x01,
	VSCP_PS_ACCESS_EEPROM_CMD_WRITE	= 0x02,
	VSCP_PS_ACCESS_EEPROM_CMD_READ	= 0x03,
	VSCP_PS_ACCESS_EEPROM_CMD_WRDI	= 0x04,
	VSCP_PS_ACCESS_EEPROM_CMD_RDSR	= 0x05,
	VSCP_PS_ACCESS_EEPROM_CMD_WREN	= 0x06

} VSCP_PS_ACCESS_EEPROM_CMD;

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
 * This function initializes the persistent memory access driver.
 * It doesn't write anything in the persistent memory! It only initializes
 * the module that read/write access is possible.
 */
extern void vscp_ps_access_init(void)
{
	dspi_master_config_t	masterConfig = { 0 };

	/* Initialize SPI here to access the EEPROM on the FRDM-VSCP-CAN shield. */
	masterConfig.whichCtar                                = kDSPI_Ctar0;
	masterConfig.ctarConfig.baudRate                      = VSCP_PS_ACCESS_DSPI_BAUDRATE;
	masterConfig.ctarConfig.bitsPerFrame                  = 8;
	masterConfig.ctarConfig.cpol                          = kDSPI_ClockPolarityActiveHigh;
	masterConfig.ctarConfig.cpha                          = kDSPI_ClockPhaseFirstEdge;
	masterConfig.ctarConfig.direction                     = kDSPI_MsbFirst;
	masterConfig.ctarConfig.pcsToSckDelayInNanoSec        = 1000000000U / masterConfig.ctarConfig.baudRate ;
	masterConfig.ctarConfig.lastSckToPcsDelayInNanoSec    = 1000000000U / masterConfig.ctarConfig.baudRate ;
	masterConfig.ctarConfig.betweenTransferDelayInNanoSec = 1000000000U / masterConfig.ctarConfig.baudRate ;
	masterConfig.whichPcs                                 = VSCP_PS_ACCESS_DSPI_CS;
	masterConfig.pcsActiveHighOrLow                       = kDSPI_PcsActiveLow;
	masterConfig.enableContinuousSCK                      = false;
	masterConfig.enableRxFifoOverWrite                    = false;
	masterConfig.enableModifiedTimingFormat               = false;
	masterConfig.samplePoint                              = kDSPI_SckToSin0Clock;

	DSPI_MasterInit(VSCP_PS_ACCESS_DSPI_MASTER_BASEADDR, &masterConfig, VSCP_PS_ACCESS_DSPI_CLK_FREQ);

    return;
}

/**
 * Read a single byte from the persistent memory.
 *
 * @param[in]   addr    Address in persistent memory
 * @return  Value
 */
extern uint8_t  vscp_ps_access_read8(uint16_t addr)
{
	dspi_transfer_t	masterXfer										= { 0 };
	uint8_t			txBuffer[VSCP_PS_ACCESS_EEPROM_READ_TFR_SIZE]	= { 0 }; /* [  op-code ] [  address ] [     dummy   ] */
	uint8_t			rxBuffer[VSCP_PS_ACCESS_EEPROM_READ_TFR_SIZE]	= { 0 }; /* [  dummy   ] [  dummy   ] [returned data] */
	uint8_t			data											= 0;

	if (0xff >= addr)
	{
		txBuffer[0] = VSCP_PS_ACCESS_EEPROM_CMD_READ;	/* Opcode */
		txBuffer[1] = (uint8_t)addr;					/* Address */
		txBuffer[2] = 0x00;								/* Dummy */

		masterXfer.txData = txBuffer;
		masterXfer.rxData = rxBuffer;
		masterXfer.dataSize = VSCP_PS_ACCESS_EEPROM_READ_TFR_SIZE;
		masterXfer.configFlags = kDSPI_MasterCtar0 | VSCP_PS_ACCESS_DSPI_CS | kDSPI_MasterPcsContinuous;

		if (kStatus_Success == DSPI_MasterTransferBlocking(VSCP_PS_ACCESS_DSPI_MASTER_BASEADDR, &masterXfer))
		{
			data = rxBuffer[2];
		}
	}

    return data;
}

/**
 * Write a single byte to the persistent memory.
 *
 * @param[in]   addr    Address in persistent memory
 * @param[in]   value   Value to write
 */
extern void vscp_ps_access_write8(uint16_t addr, uint8_t value)
{
	dspi_transfer_t	masterXfer		= { 0 };
	const uint8_t	size			= 3;
	uint8_t			txBuffer[size]; /* [  op-code ] [  address ] [     dummy   ] */
	uint8_t			rxBuffer[size]; /* [  dummy   ] [  dummy   ] [returned data] */

	if (0xff >= addr)
	{
		masterXfer.txData = txBuffer;
		masterXfer.rxData = rxBuffer;
		masterXfer.configFlags = kDSPI_MasterCtar0 | VSCP_PS_ACCESS_DSPI_CS | kDSPI_MasterPcsContinuous;

		/* The WREN instruction must be sent separately to enable writes to the eeprom. This is done
		 * with its own API call to make the CS line comes back up which sets the WREN latch.
		 */
		txBuffer[0] = VSCP_PS_ACCESS_EEPROM_CMD_WREN;	/* Opcode */

		masterXfer.dataSize = 1;

		(void)DSPI_MasterTransferBlocking(VSCP_PS_ACCESS_DSPI_MASTER_BASEADDR, &masterXfer);

		/* Write data */
		txBuffer[0] = VSCP_PS_ACCESS_EEPROM_CMD_WRITE;	/* Opcode */
		txBuffer[1] = (uint8_t)addr;					/* Address */
		txBuffer[2] = value;							/* Data */

		masterXfer.dataSize = size;

		(void)DSPI_MasterTransferBlocking(VSCP_PS_ACCESS_DSPI_MASTER_BASEADDR, &masterXfer);
	}

    return;
}

/**
 * Read several bytes from the persistent memory.
 *
 * @param[in]   addr    Address in persistent memory
 * @param[in]   buffer  Buffer which to read in
 * @param[in]   size    Buffer size in byte (Number of data to read)
 */
extern void vscp_ps_access_readMultiple(uint16_t addr, uint8_t* const buffer, uint8_t size)
{
    /* Note, this function uses single byte access by default.
     * If necessary, adapt this for optimized access according to your needs.
     */
    if ((NULL != buffer) &&
        (0 < size))
    {
        uint8_t index   = 0;

        for(index = 0; index < size; ++index)
        {
            buffer[index] = vscp_ps_access_read8(addr + index);
        }
    }

    return;
}

/**
 * Write several bytes to the persistent memory.
 *
 * @param[in]   addr    Address in persistent memory
 * @param[in]   buffer  Buffer which to write
 * @param[in]   size    Bufer size in byte (Number of data to write)
 */
extern void vscp_ps_access_writeMultiple(uint16_t addr, const uint8_t* const buffer, uint8_t size)
{
    /* Note, this function uses single byte access by default.
     * If necessary, adapt this for optimized access according to your needs.
     */
    if ((NULL != buffer) &&
        (0 < size))
    {
        uint8_t index   = 0;

        for(index = 0; index < size; ++index)
        {
            vscp_ps_access_write8(addr + index, buffer[index]);
        }
    }

    return;
}

/*******************************************************************************
    LOCAL FUNCTIONS
*******************************************************************************/
