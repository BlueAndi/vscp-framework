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
@brief  VSCP bootloader adapter
@file   vscp_bl_adapter.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
@see vscp_bl_adapter.h

*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "vscp_bl_adapter.h"
#include "platform.h"
#include "log.h"
#include "eeprom.h"
#include "vscp_dev_data.h"
#include "stdio.h"

/*******************************************************************************
    COMPILER SWITCHES
*******************************************************************************/

/*******************************************************************************
    CONSTANTS
*******************************************************************************/

/** Address of the boot flag in the persistent memory as defined by VSCP. */
#define VSCP_BL_ADAPTER_PS_ADDR_BOOT_FLAG   0

/** Address of the node nickname in the persistent memory. */
#define VSCP_BL_ADAPTER_PS_ADDR_NICKNAME    1

/** Application section file name */
#define VSCP_BL_ADAPTER_APP_SEC_FILENAME    "appsec.bin"

/** Application section size in bytes */
#define VSCP_BL_ADAPTER_APP_SEC_SIZE        (VSCP_PLATFORM_PROG_MEM_NUM_BLOCKS * VSCP_PLATFORM_PROG_MEM_BLOCK_SIZE)

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

/*******************************************************************************
    GLOBAL VARIABLES
*******************************************************************************/

/*******************************************************************************
    GLOBAL FUNCTIONS
*******************************************************************************/

/**
 * This function initializes the module.
 */
extern void vscp_bl_adapter_init(void)
{
    FILE*   fd  = NULL;

    log_printf("Bootloader entered.\n");

    /* Create a empty application section file.
     * Empty means a file with 0xFF at any place.
     */
    fd = fopen(VSCP_BL_ADAPTER_APP_SEC_FILENAME, "wb");

    if (NULL == fd)
    {
        LOG_ERROR("Couldn't create appcliation section file.");
    }
    else
    {
        uint32_t run    = 0;
        uint8_t data    = 0xff;

        for(run = 0; run < VSCP_BL_ADAPTER_APP_SEC_SIZE; ++run)
        {
            (void)fwrite(&data, sizeof(data), 1, fd);
        }

        fclose(fd);
        fd = NULL;
    }

    return;
}

/**
 * This function reads the nickname id of the node.
 *
 * @return  Nickname id
 */
extern uint8_t  vscp_bl_adapter_readNicknameId(void)
{
    return eeprom_read8(VSCP_BL_ADAPTER_PS_ADDR_NICKNAME);
}

/**
 * This function jumps to the application and will never return.
 */
extern void vscp_bl_adapter_jumpToApp(void)
{
    log_printf("Jump to application.\n");

    return;
}

/**
 * This function enable or disable the initialization lamp.
 *
 * @param[in]   enableIt    Enable (true) or disable (false) lamp
 */
extern void vscp_bl_adapter_enableLamp(BOOL enableIt)
{
    if (FALSE == enableIt)
    {
        platform_setTextColor(PLATFORM_COLOR_YELLOW);
        log_printf("Lamp off\n");
        platform_restoreTextColor();
    }
    else
    {
        platform_setTextColor(PLATFORM_COLOR_LIGHT_YELLOW);
        log_printf("Lamp on\n");
        platform_restoreTextColor();
    }

    return;
}

/**
 * This function stops the MCU and will never return.
 * It is called in any error case.
 */
extern void vscp_bl_adapter_halt(void)
{
    log_printf("HALT!\n");

    return;
}

/**
 * This function reboot the MCU and will never return.
 */
extern void vscp_bl_adapter_reboot(void)
{
    log_printf("REBOOT!\n");

    return;
}

/**
 * This function returns the state of the segment initialization button.
 *
 * @return State
 * @retval FALSE    Released
 * @retval TRUE     Pressed
 */
extern BOOL vscp_bl_adapter_getSegInitButtonState(void)
{
    BOOL    state   = FALSE;

    return state;
}

/**
 * This function reads the boot flag from persistent memory.
 *
 * @return Boot flag
 */
extern uint8_t vscp_bl_adapter_readBootFlag(void)
{
    return eeprom_read8(VSCP_BL_ADAPTER_PS_ADDR_BOOT_FLAG);
}

/**
 * This function writes the boot flag to persistent memory.
 *
 * @param[in]   bootFlag    Boot flag
 */
extern void vscp_bl_adapter_writeBootFlag(uint8_t bootFlag)
{
    eeprom_write8(VSCP_BL_ADAPTER_PS_ADDR_BOOT_FLAG, bootFlag);
    return;
}

/**
 * Read byte @see index of the GUID from persistent memory.
 * Note that index 0 is the LSB and index 15 the MSB.
 *
 * @param[in]   index   Index of the GUID [0-15]
 * @return  GUID byte @see index
 */
extern uint8_t  vscp_bl_adapter_readGUID(uint8_t index)
{
    return vscp_dev_data_getGUID(index);
}

/**
 * This function writes a complete block to the program memory.
 *
 * @param[in]   blockNo Block number of block which shall be written
 * @param[in]   buffer  Pointer to the buffer with the data
 */
extern void vscp_bl_adapter_programBlock(uint32_t blockNo, uint8_t *buffer)
{
    FILE*   fd  = NULL;

    log_printf("Program block no. 0x%04X.\n", blockNo);

    fd = fopen(VSCP_BL_ADAPTER_APP_SEC_FILENAME, "r+b");

    if (NULL == fd)
    {
        LOG_ERROR("Couldn't open application section file.");
    }
    else
    {
        /* Seek successful? */
        if (0 == fseek(fd, blockNo * VSCP_PLATFORM_PROG_MEM_BLOCK_SIZE, SEEK_SET))
        {
            (void)fwrite(buffer, sizeof(uint8_t), VSCP_PLATFORM_PROG_MEM_BLOCK_SIZE, fd);
        }

        fclose(fd);
        fd = NULL;
    }

    return;
}

/**
 * This function read a byte from program memory.
 *
 * @param[in] address   Program memory address
 * @return Value
 */
extern uint8_t  vscp_bl_adapter_readProgMem(uint16_t address)
{
    uint8_t value = 0;
    FILE*   fd  = NULL;

    fd = fopen(VSCP_BL_ADAPTER_APP_SEC_FILENAME, "rb");

    if (NULL != fd)
    {
        /* Seek successful? */
        if (0 == fseek(fd, address, SEEK_SET))
        {
            /* Read failed? */
            if (1 != fread(&value, sizeof(value), 1, fd))
            {
                value = 0;
            }
        }

        fclose(fd);
        fd = NULL;
    }

    return value;
}

/*******************************************************************************
    LOCAL FUNCTIONS
*******************************************************************************/

