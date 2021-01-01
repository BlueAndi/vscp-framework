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
#include <util/delay.h>
#include <avr/boot.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include "hw.h"

/*******************************************************************************
    COMPILER SWITCHES
*******************************************************************************/

/*******************************************************************************
    CONSTANTS
*******************************************************************************/

/** Application section start address */
#define VSCP_BL_ADAPTER_APPLICATION_START_ADDR  ((void*)0x0000u)

/** Address of the boot flag in the persistent memory as defined by VSCP. */
#define VSCP_BL_ADAPTER_PS_ADDR_BOOT_FLAG       0

/** Address of the node nickname in the persistent memory. */
#define VSCP_BL_ADAPTER_PS_ADDR_NICKNAME        1

/*******************************************************************************
    MACROS
*******************************************************************************/

/** Number of elements in a array */
#define VSCP_BL_ADAPTER_ARRAY_NUM(__arr)    (sizeof(__arr) / sizeof((__arr)[0]))

/** Set interrupt vectors to bootloader section */
#define VSCP_BL_ADAPTER_ISR_VECTOR_TO_BL()  \
    {                                       \
        uint8_t value   = MCUCR;            \
        MCUCR = value | _BV(IVCE);          \
        MCUCR = value | _BV(IVSEL);         \
    }

/** Set interrupt vectors to application section */
#define VSCP_BL_ADAPTER_ISR_VECTOR_TO_APP() \
    {                                       \
        uint8_t value   = MCUCR;            \
        MCUCR = value | _BV(IVCE);          \
        MCUCR = value & ~_BV(IVSEL);        \
    }

/*******************************************************************************
    TYPES AND STRUCTURES
*******************************************************************************/

/*******************************************************************************
    PROTOTYPES
*******************************************************************************/

/*******************************************************************************
    LOCAL VARIABLES
*******************************************************************************/

/** Node GUID */
static const uint8_t    vscp_bl_adapter_nodeGuid[]  =
{
    /* MSB <------------------------------------------------------------------------------> LSB */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE
};

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
    /* Move interrupt vectors to bootloader section */
    VSCP_BL_ADAPTER_ISR_VECTOR_TO_BL();

    return;
}

/**
 * This function reads the nickname id of the node.
 *
 * @return  Nickname id
 */
extern uint8_t  vscp_bl_adapter_readNicknameId(void)
{
    return eeprom_read_byte((uint8_t*)VSCP_BL_ADAPTER_PS_ADDR_NICKNAME);
}

/**
 * This function jumps to the application and will never return.
 */
extern void vscp_bl_adapter_jumpToApp(void)
{
    void    (*jump)(void)   = VSCP_BL_ADAPTER_APPLICATION_START_ADDR;

    /* Reset SPI interface to power-up state */
    SPCR = 0;
    SPSR = 0;

    /* Move interrupt vectors to application section */
    VSCP_BL_ADAPTER_ISR_VECTOR_TO_APP();

    /* Jump to application */
    jump();

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
        HW_DISABLE_STATUS_LED();
    }
    else
    {
        HW_ENABLE_STATUS_LED();
    }

    return;
}

/**
 * This function stops the MCU and will never return.
 * It is called in any error case.
 */
extern void vscp_bl_adapter_halt(void)
{
    /* Move interrupt vectors to application section */
    VSCP_BL_ADAPTER_ISR_VECTOR_TO_APP();

    HALT();

    return;
}

/**
 * This function reboot the MCU and will never return.
 */
extern void vscp_bl_adapter_reboot(void)
{
    /* Move interrupt vectors to application section */
    VSCP_BL_ADAPTER_ISR_VECTOR_TO_APP();

    REBOOT();

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
    uint8_t index   = 0;
    BOOL    state   = hw_getSegInitButtonState();

    /* Simple debouncing */
    do
    {
        _delay_ms(10);
        if (state != hw_getSegInitButtonState())
        {
            index = 0;
        }
        else
        {
            ++index;
        }
    }
    while(3 > index);

    return state;
}

/**
 * This function reads the boot flag from persistent memory.
 *
 * @return Boot flag
 */
extern uint8_t vscp_bl_adapter_readBootFlag(void)
{
    return eeprom_read_byte((uint8_t*)VSCP_BL_ADAPTER_PS_ADDR_BOOT_FLAG);
}

/**
 * This function writes the boot flag to persistent memory.
 *
 * @param[in]   bootFlag    Boot flag
 */
extern void vscp_bl_adapter_writeBootFlag(uint8_t bootFlag)
{
    eeprom_write_byte((uint8_t*)VSCP_BL_ADAPTER_PS_ADDR_BOOT_FLAG, bootFlag);
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
    uint8_t         value       = 0;
    const uint8_t   cGuidSize   = VSCP_BL_ADAPTER_ARRAY_NUM(vscp_bl_adapter_nodeGuid);

    if (cGuidSize > index)
    {
        value = vscp_bl_adapter_nodeGuid[cGuidSize - index - 1];
    }

    return value;
}

/**
 * This function writes a complete block to the program memory.
 *
 * @param[in]   blockNo Block number of block which shall be written
 * @param[in]   buffer  Pointer to the buffer with the data
 */
extern void vscp_bl_adapter_programBlock(uint32_t blockNo, uint8_t *buffer)
{
    uint32_t    addr    = blockNo * VSCP_PLATFORM_PROG_MEM_BLOCK_SIZE;
    uint16_t    index   = 0u;
    uint8_t     sreg    = 0;

    /* Disable interrupts */
    sreg = SREG;
    cli();

    /* No EEPROM activity allowed */
    eeprom_busy_wait();

    /* Erase flash page */
    boot_page_erase(addr);

    /* Wait until the memory is erased. */
    boot_spm_busy_wait();

    for (index = 0u; index < VSCP_PLATFORM_PROG_MEM_BLOCK_SIZE; index += 2u)
    {
        /* Set up little-endian word. */
        uint16_t leWord = *buffer;
        ++buffer;
        leWord += (*buffer) << 8u;
        ++buffer;

        boot_page_fill(addr + index, leWord);
    }

    /* Store buffer in flash page. */
    boot_page_write(addr);

    /* Wait until the memory is written. */
    boot_spm_busy_wait();

    /* Re-enable RWW-section again. We need this if we want to jump back
     * to the application after the boot loader.
     */
    boot_rww_enable();

    /* Enable interrupts (if they were ever enabled). */
    SREG = sreg;

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
    return pgm_read_byte(address);
}

/*******************************************************************************
    LOCAL FUNCTIONS
*******************************************************************************/

