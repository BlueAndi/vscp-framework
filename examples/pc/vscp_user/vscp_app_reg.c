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
@brief  VSCP application registers
@file   vscp_app_reg.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
@see vscp_app_reg.h

*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "vscp_app_reg.h"
#include "vscp_types.h"
#include "vscp_ps.h"
#include "vscp_config.h"

/*******************************************************************************
    COMPILER SWITCHES
*******************************************************************************/

/*******************************************************************************
    CONSTANTS
*******************************************************************************/

/** Application register offset of node zone. */
#define VSCP_APP_REG_NODE_ZONE                      0

/** Application register offset of node sub zone. */
#define VSCP_APP_REG_NODE_SUB_ZONE                  1

/** Application register offset of page 2 of dm ng. */
#define VSCP_APP_REG_PAGE_2_OFFSET_DM_NG            0

/** Application register offset of page 3 of a test string with size 5. */
#define VSCP_APP_REG_PAGE_3_OFFSET_TEST_STRING      0

/** Application register offset of page 3 of a test bitfield. */
#define VSCP_APP_REG_PAGE_3_OFFSET_TEST_BITFIELD    5

/** Application register offset of page 3 of a test boolean. */
#define VSCP_APP_REG_PAGE_3_OFFSET_TEST_BOOLEAN     6

/** Application register offset of page 3 of a test int8_t. */
#define VSCP_APP_REG_PAGE_3_OFFSET_TEST_INT8_T      7

/** Application register offset of page 3 of a test uint8_t. */
#define VSCP_APP_REG_PAGE_3_OFFSET_TEST_UINT8_T     8

/** Application register offset of page 3 of a test int16_t. */
#define VSCP_APP_REG_PAGE_3_OFFSET_TEST_INT16_T     9

/** Application register offset of page 3 of a test uint16_t. */
#define VSCP_APP_REG_PAGE_3_OFFSET_TEST_UINT16_T    11

/** Application register offset of page 3 of a test int32_t. */
#define VSCP_APP_REG_PAGE_3_OFFSET_TEST_INT32_T     13

/** Application register offset of page 3 of a test uint32_t. */
#define VSCP_APP_REG_PAGE_3_OFFSET_TEST_UINT32_T    17

/** Application register offset of page 3 of a test int64_t. */
#define VSCP_APP_REG_PAGE_3_OFFSET_TEST_INT64_T     25

/** Application register offset of page 3 of a test uint64_t. */
#define VSCP_APP_REG_PAGE_3_OFFSET_TEST_UINT64_T    29

/** Application register offset of page 3 of a test float. */
#define VSCP_APP_REG_PAGE_3_OFFSET_TEST_FLOAT_T     37

/** Application register offset of page 3 of a test double. */
#define VSCP_APP_REG_PAGE_3_OFFSET_TEST_DOUBLE_T    41

/** Application register offset of page 3 of a test date. */
#define VSCP_APP_REG_PAGE_3_OFFSET_TEST_DATE_T      49

/** Application register offset of page 3 of a test time. */
#define VSCP_APP_REG_PAGE_3_OFFSET_TEST_TIME_T      53

/** Application register offset of page 3 of a test GUID. */
#define VSCP_APP_REG_PAGE_3_OFFSET_TEST_GUID_T      57

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

/** Test string used to test the MDF abstraction */
static char     vscp_app_reg_testString[5]   = "test";

/** Test bitfield used to test the MDF abstraction */
static uint8_t  vscp_app_reg_testBitfield    = 0x55;

/** Test boolean used to test the MDF abstraction */
static BOOL     vscp_app_reg_testBoolean     = FALSE;

/** Test int8_t used to test the MDF abstraction */
static int8_t   vscp_app_reg_testInt8        = -2;

/** Test uint8_t used to test the MDF abstraction */
static uint8_t  vscp_app_reg_testUInt8       = 2;

/** Test int16_t used to test the MDF abstraction */
static int16_t  vscp_app_reg_testInt16       = -30000;

/** Test uint16_t used to test the MDF abstraction */
static uint16_t vscp_app_reg_testUInt16      = 50000;

/** Test int32_t used to test the MDF abstraction */
static int32_t  vscp_app_reg_testInt32       = -210000000;

/** Test uint32_t used to test the MDF abstraction */
static uint32_t vscp_app_reg_testUInt32      = 4000000000;

/** Test int64_t used to test the MDF abstraction */
static int64_t  vscp_app_reg_testInt64       = -361700865234649924;

/** Test uint64_t used to test the MDF abstraction */
static uint64_t vscp_app_reg_testUInt64      = 361700865234649924;

/** Test float used to test the MDF abstraction */
static float    vscp_app_reg_testFloat       = 123.45;

/** Test double used to test the MDF abstraction */
static double   vscp_app_reg_testDouble      = 123.45;

/** Test date used to test the MDF abstraction */
static uint32_t vscp_app_reg_testDate        = 0x07df0711;

/** Test time used to test the MDF abstraction */
static uint32_t vscp_app_reg_testTime        = 0x120d18;

/** Test GUID used to test the MDF abstraction */
static uint8_t  vscp_app_reg_testGuid[16]    = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };

/*******************************************************************************
    GLOBAL VARIABLES
*******************************************************************************/

/*******************************************************************************
    GLOBAL FUNCTIONS
*******************************************************************************/

/**
 * This function initializes this module.
 */
extern void vscp_app_reg_init(void)
{
    /* Implement your code here ... */

    return;
}

/**
 * Restore the application specific factory default settings.
 */
extern void vscp_app_reg_restoreFactoryDefaultSettings(void)
{
    /* Node zone and sub-zone are restored in the device data.
     * No need to overwrite it here.
     */

    return;
}

/**
 * This function returns the number of used pages.
 * Its used in the register abstraction model.
 * Deprecated since VSCP spec. v1.10.2
 *
 * @return  Pages used
 * @retval  0   More than 255 pages are used.
 */
extern uint8_t  vscp_app_reg_getPagesUsed(void)
{
    /* Standard VSCP registers are at page 0,
     * DM is at page 1
     * DM NG is at page 2
     * Test stuff is at page 3
     */
    uint8_t pagesUsed   = 4;
    
    return pagesUsed;
}

/**
 * This function reads a application specific register and returns the value.
 *
 * @param[in]   page    Page
 * @param[in]   addr    Register address
 * @return  Register value
 */
extern uint8_t  vscp_app_reg_readRegister(uint16_t page, uint8_t addr)
{
    uint8_t value   = 0;
    
    if (0 == page)
    {
        if ((VSCP_REGISTER_APP_START_ADDR <= addr) &&
            (VSCP_REGISTER_APP_END_ADDR >= addr))
        {
            switch(addr)
            {
            case VSCP_APP_REG_NODE_ZONE:
                value = vscp_ps_readNodeZone();
                break;

            case VSCP_APP_REG_NODE_SUB_ZONE:
                value = vscp_ps_readNodeSubZone();
                break;

            default:
                break;
            }
        }
    }
    else if (1 == page )
    {
        /* DM register access is handled by DM directly */
    }
    
#if VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM_NEXT_GENERATION )
    
    else if (2 == page)
    {
        if ((VSCP_APP_REG_PAGE_2_OFFSET_DM_NG <= addr) &&
            ((VSCP_APP_REG_PAGE_2_OFFSET_DM_NG + VSCP_CONFIG_DM_NG_RULE_SET_SIZE) > addr))
        {
            value = vscp_ps_readDMNextGeneration(addr - VSCP_APP_REG_PAGE_2_OFFSET_DM_NG);
        }
    }

#endif  /* VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM_NEXT_GENERATION ) */
    
    else if (3 == page)
    {
        if ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_STRING <= addr) &&
            ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_STRING + sizeof(vscp_app_reg_testString)) > addr))
        {
            value = vscp_app_reg_testString[addr - VSCP_APP_REG_PAGE_3_OFFSET_TEST_STRING];
        }
        else if (VSCP_APP_REG_PAGE_3_OFFSET_TEST_BITFIELD == addr)
        {
            value = vscp_app_reg_testBitfield;
        }
        else if (VSCP_APP_REG_PAGE_3_OFFSET_TEST_BOOLEAN == addr)
        {
            value = vscp_app_reg_testBoolean;
        }
        else if (VSCP_APP_REG_PAGE_3_OFFSET_TEST_INT8_T == addr)
        {
            value = vscp_app_reg_testInt8;
        }
        else if (VSCP_APP_REG_PAGE_3_OFFSET_TEST_UINT8_T == addr)
        {
            value = vscp_app_reg_testUInt8;
        }
        else if ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_INT16_T <= addr) &&
                ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_INT16_T + sizeof(vscp_app_reg_testInt16)) > addr))
        {
            /* MSB first */
            value = ((uint8_t*)&vscp_app_reg_testInt16)[sizeof(vscp_app_reg_testInt16) - addr + VSCP_APP_REG_PAGE_3_OFFSET_TEST_INT16_T - 1];
        }
        else if ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_UINT16_T <= addr) &&
                ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_UINT16_T + sizeof(vscp_app_reg_testUInt16)) > addr))
        {
            /* MSB first */
            value = ((uint8_t*)&vscp_app_reg_testUInt16)[sizeof(vscp_app_reg_testUInt16) - addr + VSCP_APP_REG_PAGE_3_OFFSET_TEST_UINT16_T - 1];
        }
        else if ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_INT32_T <= addr) &&
                ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_INT32_T + sizeof(vscp_app_reg_testInt32)) > addr))
        {
            /* MSB first */
            value = ((uint8_t*)&vscp_app_reg_testInt32)[sizeof(vscp_app_reg_testInt32) - addr + VSCP_APP_REG_PAGE_3_OFFSET_TEST_INT32_T - 1];
        }
        else if ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_UINT32_T <= addr) &&
                ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_UINT32_T + sizeof(vscp_app_reg_testUInt32)) > addr))
        {
            /* MSB first */
            value = ((uint8_t*)&vscp_app_reg_testUInt32)[sizeof(vscp_app_reg_testUInt32) - addr + VSCP_APP_REG_PAGE_3_OFFSET_TEST_UINT32_T - 1];
        }
        else if ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_INT64_T <= addr) &&
                ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_INT64_T + sizeof(vscp_app_reg_testInt64)) > addr))
        {
            /* MSB first */
            value = ((uint8_t*)&vscp_app_reg_testInt64)[sizeof(vscp_app_reg_testInt64) - addr + VSCP_APP_REG_PAGE_3_OFFSET_TEST_INT64_T - 1];
        }
        else if ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_UINT64_T <= addr) &&
                ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_UINT64_T + sizeof(vscp_app_reg_testUInt64)) > addr))
        {
            /* MSB first */
            value = ((uint8_t*)&vscp_app_reg_testUInt64)[sizeof(vscp_app_reg_testUInt64) - addr + VSCP_APP_REG_PAGE_3_OFFSET_TEST_UINT64_T - 1];
        }
        else if ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_FLOAT_T <= addr) &&
                ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_FLOAT_T + sizeof(vscp_app_reg_testFloat)) > addr))
        {
            /* MSB first */
            value = ((uint8_t*)&vscp_app_reg_testFloat)[sizeof(vscp_app_reg_testFloat) - addr + VSCP_APP_REG_PAGE_3_OFFSET_TEST_FLOAT_T - 1];
        }
        else if ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_DOUBLE_T <= addr) &&
                ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_DOUBLE_T + sizeof(vscp_app_reg_testDouble)) > addr))
        {
            /* MSB first */
            value = ((uint8_t*)&vscp_app_reg_testDouble)[sizeof(vscp_app_reg_testDouble) - addr + VSCP_APP_REG_PAGE_3_OFFSET_TEST_DOUBLE_T - 1];
        }
        else if ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_DATE_T <= addr) &&
                ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_DATE_T + sizeof(vscp_app_reg_testDate)) > addr))
        {
            /* MSB first */
            value = ((uint8_t*)&vscp_app_reg_testDate)[sizeof(vscp_app_reg_testDate) - addr + VSCP_APP_REG_PAGE_3_OFFSET_TEST_DATE_T - 1];
        }
        else if ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_TIME_T <= addr) &&
                ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_TIME_T + sizeof(vscp_app_reg_testTime)) > addr))
        {
            /* MSB first */
            value = ((uint8_t*)&vscp_app_reg_testTime)[sizeof(vscp_app_reg_testTime) - addr + VSCP_APP_REG_PAGE_3_OFFSET_TEST_TIME_T - 1];
        }
        else if ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_GUID_T <= addr) &&
                ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_GUID_T + sizeof(vscp_app_reg_testGuid)) > addr))
        {
            /* MSB first */
            value = vscp_app_reg_testGuid[sizeof(vscp_app_reg_testGuid) - addr + VSCP_APP_REG_PAGE_3_OFFSET_TEST_GUID_T - 1];
        }
    }
    
    return value;
}

/**
 * This function writes a value to an application specific register.
 *
 * @param[in]   page    Page
 * @param[in]   addr    Register address
 * @param[in]   value   Value to write
 * @return  Register value
 */
extern uint8_t  vscp_app_reg_writeRegister(uint16_t page, uint8_t addr, uint8_t value)
{
    uint8_t readBackValue   = 0;

    if (0 == page)
    {
        if ((VSCP_REGISTER_APP_START_ADDR <= addr) &&
            (VSCP_REGISTER_APP_END_ADDR >= addr))
        {
            switch(addr)
            {
            case VSCP_APP_REG_NODE_ZONE:
                vscp_ps_writeNodeZone(value);
                readBackValue = vscp_ps_readNodeZone();
                break;

            case VSCP_APP_REG_NODE_SUB_ZONE:
                vscp_ps_writeNodeSubZone(value);
                readBackValue = vscp_ps_readNodeSubZone();
                break;

            default:
                break;
            }
        }
    }
    else if (1 == page )
    {
        /* DM register access is handled by DM directly */
    }
    
#if VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM_NEXT_GENERATION )
    
    else if (2 == page)
    {
        if ((VSCP_APP_REG_PAGE_2_OFFSET_DM_NG <= addr) &&
            ((VSCP_APP_REG_PAGE_2_OFFSET_DM_NG + VSCP_CONFIG_DM_NG_RULE_SET_SIZE) > addr))
        {
            vscp_ps_writeDMNextGeneration(addr - VSCP_APP_REG_PAGE_2_OFFSET_DM_NG, value);
            readBackValue = vscp_ps_readDMNextGeneration(addr - VSCP_APP_REG_PAGE_2_OFFSET_DM_NG);
        }
    }

#endif  /* VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM_NEXT_GENERATION ) */
    
    else if (3 == page)
    {
        if ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_STRING <= addr) &&
            ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_STRING + sizeof(vscp_app_reg_testString)) > addr))
        {
            vscp_app_reg_testString[addr - VSCP_APP_REG_PAGE_3_OFFSET_TEST_STRING] = value;
            readBackValue = vscp_app_reg_testString[addr - VSCP_APP_REG_PAGE_3_OFFSET_TEST_STRING];
        }
        else if (VSCP_APP_REG_PAGE_3_OFFSET_TEST_BITFIELD == addr)
        {
            vscp_app_reg_testBitfield = value;
            readBackValue = vscp_app_reg_testBitfield;
        }
        else if (VSCP_APP_REG_PAGE_3_OFFSET_TEST_BOOLEAN == addr)
        {
            vscp_app_reg_testBoolean = value;
            readBackValue = vscp_app_reg_testBoolean;
        }
        else if (VSCP_APP_REG_PAGE_3_OFFSET_TEST_INT8_T == addr)
        {
            vscp_app_reg_testInt8 = (int8_t)value;
            readBackValue = vscp_app_reg_testInt8;
        }
        else if (VSCP_APP_REG_PAGE_3_OFFSET_TEST_UINT8_T == addr)
        {
            vscp_app_reg_testUInt8 = value;
            readBackValue = vscp_app_reg_testUInt8;
        }
        else if ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_INT16_T <= addr) &&
                ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_INT16_T + sizeof(vscp_app_reg_testInt16)) > addr))
        {
            /* MSB first */
            ((uint8_t*)&vscp_app_reg_testInt16)[sizeof(vscp_app_reg_testInt16) - addr + VSCP_APP_REG_PAGE_3_OFFSET_TEST_INT16_T - 1] = value;
            readBackValue = ((uint8_t*)&vscp_app_reg_testInt16)[sizeof(vscp_app_reg_testInt16) - addr + VSCP_APP_REG_PAGE_3_OFFSET_TEST_INT16_T - 1];
        }
        else if ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_UINT16_T <= addr) &&
                ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_UINT16_T + sizeof(vscp_app_reg_testUInt16)) > addr))
        {
            /* MSB first */
            ((uint8_t*)&vscp_app_reg_testUInt16)[sizeof(vscp_app_reg_testUInt16) - addr + VSCP_APP_REG_PAGE_3_OFFSET_TEST_UINT16_T - 1] = value;
            readBackValue = ((uint8_t*)&vscp_app_reg_testUInt16)[sizeof(vscp_app_reg_testUInt16) - addr + VSCP_APP_REG_PAGE_3_OFFSET_TEST_UINT16_T - 1];
        }
        else if ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_INT32_T <= addr) &&
                ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_INT32_T + sizeof(vscp_app_reg_testInt32)) > addr))
        {
            /* MSB first */
            ((uint8_t*)&vscp_app_reg_testInt32)[sizeof(vscp_app_reg_testInt32) - addr + VSCP_APP_REG_PAGE_3_OFFSET_TEST_INT32_T - 1] = value;
            readBackValue = ((uint8_t*)&vscp_app_reg_testInt32)[sizeof(vscp_app_reg_testInt32) - addr + VSCP_APP_REG_PAGE_3_OFFSET_TEST_INT32_T - 1];
        }
        else if ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_UINT32_T <= addr) &&
                ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_UINT32_T + sizeof(vscp_app_reg_testUInt32)) > addr))
        {
            /* MSB first */
            ((uint8_t*)&vscp_app_reg_testUInt32)[sizeof(vscp_app_reg_testUInt32) - addr + VSCP_APP_REG_PAGE_3_OFFSET_TEST_UINT32_T - 1] = value;
            readBackValue = ((uint8_t*)&vscp_app_reg_testUInt32)[sizeof(vscp_app_reg_testUInt32) - addr + VSCP_APP_REG_PAGE_3_OFFSET_TEST_UINT32_T - 1];
        }
        else if ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_INT64_T <= addr) &&
                ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_INT64_T + sizeof(vscp_app_reg_testInt64)) > addr))
        {
            /* MSB first */
            ((uint8_t*)&vscp_app_reg_testInt64)[sizeof(vscp_app_reg_testInt64) - addr + VSCP_APP_REG_PAGE_3_OFFSET_TEST_INT64_T - 1] = value;
            readBackValue = ((uint8_t*)&vscp_app_reg_testInt64)[sizeof(vscp_app_reg_testInt64) - addr + VSCP_APP_REG_PAGE_3_OFFSET_TEST_INT64_T - 1];
        }
        else if ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_UINT64_T <= addr) &&
                ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_UINT64_T + sizeof(vscp_app_reg_testUInt64)) > addr))
        {
            /* MSB first */
            ((uint8_t*)&vscp_app_reg_testUInt64)[sizeof(vscp_app_reg_testUInt64) - addr + VSCP_APP_REG_PAGE_3_OFFSET_TEST_UINT64_T - 1] = value;
            readBackValue = ((uint8_t*)&vscp_app_reg_testUInt64)[sizeof(vscp_app_reg_testUInt64) - addr + VSCP_APP_REG_PAGE_3_OFFSET_TEST_UINT64_T - 1];
        }
        else if ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_FLOAT_T <= addr) &&
                ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_FLOAT_T + sizeof(vscp_app_reg_testFloat)) > addr))
        {
            /* MSB first */
            ((uint8_t*)&vscp_app_reg_testFloat)[sizeof(vscp_app_reg_testFloat) - addr + VSCP_APP_REG_PAGE_3_OFFSET_TEST_FLOAT_T - 1] = value;
            readBackValue = ((uint8_t*)&vscp_app_reg_testFloat)[sizeof(vscp_app_reg_testFloat) - addr + VSCP_APP_REG_PAGE_3_OFFSET_TEST_FLOAT_T - 1];
        }
        else if ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_DOUBLE_T <= addr) &&
                ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_DOUBLE_T + sizeof(vscp_app_reg_testDouble)) > addr))
        {
            /* MSB first */
            ((uint8_t*)&vscp_app_reg_testDouble)[sizeof(vscp_app_reg_testDouble) - addr + VSCP_APP_REG_PAGE_3_OFFSET_TEST_DOUBLE_T - 1] = value;
            readBackValue = ((uint8_t*)&vscp_app_reg_testDouble)[sizeof(vscp_app_reg_testDouble) - addr + VSCP_APP_REG_PAGE_3_OFFSET_TEST_DOUBLE_T - 1];
        }
        else if ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_DATE_T <= addr) &&
                ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_DATE_T + sizeof(vscp_app_reg_testDate)) > addr))
        {
            /* MSB first */
            ((uint8_t*)&vscp_app_reg_testDate)[sizeof(vscp_app_reg_testDate) - addr + VSCP_APP_REG_PAGE_3_OFFSET_TEST_DATE_T - 1] = value;
            readBackValue = ((uint8_t*)&vscp_app_reg_testDate)[sizeof(vscp_app_reg_testDate) - addr + VSCP_APP_REG_PAGE_3_OFFSET_TEST_DATE_T - 1];
        }
        else if ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_TIME_T <= addr) &&
                ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_TIME_T + sizeof(vscp_app_reg_testTime)) > addr))
        {
            /* MSB first */
            ((uint8_t*)&vscp_app_reg_testTime)[sizeof(vscp_app_reg_testTime) - addr + VSCP_APP_REG_PAGE_3_OFFSET_TEST_TIME_T - 1] = value;
            readBackValue = ((uint8_t*)&vscp_app_reg_testTime)[sizeof(vscp_app_reg_testTime) - addr + VSCP_APP_REG_PAGE_3_OFFSET_TEST_TIME_T - 1];
        }
        else if ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_GUID_T <= addr) &&
                ((VSCP_APP_REG_PAGE_3_OFFSET_TEST_GUID_T + sizeof(vscp_app_reg_testGuid)) > addr))
        {
            /* MSB first */
            vscp_app_reg_testGuid[sizeof(vscp_app_reg_testGuid) - addr + VSCP_APP_REG_PAGE_3_OFFSET_TEST_GUID_T - 1] = value;
            readBackValue = vscp_app_reg_testGuid[sizeof(vscp_app_reg_testGuid) - addr + VSCP_APP_REG_PAGE_3_OFFSET_TEST_GUID_T - 1];
        }
    }
    
    return readBackValue;
}

/*******************************************************************************
    LOCAL FUNCTIONS
*******************************************************************************/

