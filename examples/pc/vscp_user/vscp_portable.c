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
@brief  VSCP portable support package
@file   vscp_portable.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
@see vscp_portable.h

*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "platform.h"
#include "vscp_portable.h"
#include "vscp_core.h"
#include "vscp_dm.h"
#include "vscp_dm_ng.h"
#include "vscp_ps.h"
#include "vscp_class_l1.h"
#include "vscp_type_information.h"
#include <stdio.h>
#include "eeprom.h"
#include "log.h"
#include "dm_std_xml.h"
#include "dm_ext_xml.h"
#include "vscp_bootloader.h"

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

/*******************************************************************************
    PROTOTYPES
*******************************************************************************/

#if VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM )

#if VSCP_CONFIG_BASE_IS_DISABLED( VSCP_CONFIG_ENABLE_DM_EXTENSION )

static void vscp_portable_loadDMStd(void);

#endif  /* VSCP_CONFIG_BASE_IS_DISABLED( VSCP_CONFIG_ENABLE_DM_EXTENSION ) */

#endif  /* VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM ) */

#if VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM_EXTENSION )

static void vscp_portable_loadDMExt(void);

#endif  /* VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM_EXTENSION ) */

/*******************************************************************************
    LOCAL VARIABLES
*******************************************************************************/

#if VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM_NEXT_GENERATION )

/** Decision matrix next generation rule set */
static uint8_t  vscp_portable_ruleSet[] =
{
    /* Number of rules */
    0x02,

    /* Rule 1: Execute action if node id is 4 or 5. */
    0x09, 0x01, 0x00,
        /* Condition 1 */
        VSCP_DM_NG_LOGIC_OP_OR   | VSCP_DM_NG_BASIC_OP_EQUAL, VSCP_DM_NG_EVENT_PAR_ID_OADDR, 0x04,
        /* Condition 2 */
        VSCP_DM_NG_LOGIC_OP_LAST | VSCP_DM_NG_BASIC_OP_EQUAL, VSCP_DM_NG_EVENT_PAR_ID_OADDR, 0x05,

    /* Rule 2: Execute action if class is information. */
    0x07, 0x02, 0x00,
        /* Condition 1 */
        VSCP_DM_NG_LOGIC_OP_LAST | VSCP_DM_NG_BASIC_OP_EQUAL, VSCP_DM_NG_EVENT_PAR_ID_CLASS, 0x00, 0x14
};

#endif  /* VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM_NEXT_GENERATION ) */

/** Flag to signal that the bootloader mode is requested or not. */
static BOOL vscp_portable_bootloaderRequested   = FALSE;

/*******************************************************************************
    GLOBAL VARIABLES
*******************************************************************************/

/*******************************************************************************
    GLOBAL FUNCTIONS
*******************************************************************************/

/**
 * This function initializes this module.
 */
extern void vscp_portable_init(void)
{
    /* Initialize the decision matrix xml load functionality */
#if VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM )
    dm_std_xml_init();
#endif  /* VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM ) */

#if VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM_EXTENSION )
    dm_ext_xml_init();
#endif  /* VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM_EXTENSION ) */

    return;
}

/**
 * Restore the application specific factory default settings.
 */
extern void vscp_portable_restoreFactoryDefaultSettings(void)
{
#if VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM_NEXT_GENERATION )

    uint8_t*    dmNG    = NULL;
    uint8_t     index   = 0;
    uint8_t*    ps      = eeprom_getBase(NULL);

#endif  /* VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM_NEXT_GENERATION ) */

    log_printf("Restore application factory default settings.\n");

#if VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM )

#if VSCP_CONFIG_BASE_IS_DISABLED( VSCP_CONFIG_ENABLE_DM_EXTENSION )

    vscp_portable_loadDMStd();

#endif  /* VSCP_CONFIG_BASE_IS_DISABLED( VSCP_CONFIG_ENABLE_DM_EXTENSION ) */

#endif  /* VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM ) */

#if VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM_EXTENSION )

    vscp_portable_loadDMExt();

#endif  /* VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM_EXTENSION ) */

#if VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM_NEXT_GENERATION )

    if (NULL == ps)
    {
        LOG_ERROR("Couldn't get EEPROM base address.");
        return;
    }

    dmNG = &ps[VSCP_PS_ADDR_DM_NEXT_GENERATION];

    for(index = 0; index < sizeof(vscp_portable_ruleSet); ++index)
    {
        dmNG[index] = vscp_portable_ruleSet[index];
    }

#endif  /* VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM_NEXT_GENERATION ) */

    return;
}

/**
 * This function set the current lamp state.
 *
 * @param[in]   state   Lamp state to set
 */
extern void vscp_portable_setLampState(VSCP_LAMP_STATE state)
{
    static VSCP_LAMP_STATE  currentState    = VSCP_LAMP_STATE_OFF;

    if (state != currentState)
    {
        switch(state)
        {
        case VSCP_LAMP_STATE_OFF:
            platform_setTextColor(PLATFORM_COLOR_YELLOW);
            log_printf("Lamp off\n");
            platform_restoreTextColor();
            break;

        case VSCP_LAMP_STATE_ON:
            platform_setTextColor(PLATFORM_COLOR_LIGHT_YELLOW);
            log_printf("Lamp on\n");
            platform_restoreTextColor();
            break;

        case VSCP_LAMP_STATE_BLINK_SLOW:
            platform_setTextColor(PLATFORM_COLOR_LIGHT_YELLOW);
            log_printf("Lamp blinking slow\n");
            platform_restoreTextColor();
            break;

        case VSCP_LAMP_STATE_BLINK_FAST:
            platform_setTextColor(PLATFORM_COLOR_LIGHT_YELLOW);
            log_printf("Lamp blinking fast\n");
            platform_restoreTextColor();
            break;

        default:
            break;
        }

        currentState = state;
    }

    return;
}

#if VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_IDLE_CALLOUT )

/**
 * If VSCP stops its work and enters idle state, this function will be called.
 */
extern void vscp_portable_idleStateEntered(void)
{
    log_printf("Entered IDLE state.\n");

    return;
}

#endif  /* VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_IDLE_CALLOUT ) */

#if VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ERROR_CALLOUT )

/**
 * If VSCP stops its work and enters error state, this function will be called.
 */
extern void vscp_portable_errorStateEntered(void)
{
    log_printf("Entered ERROR state.\n");

    return;
}

#endif  /* VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ERROR_CALLOUT ) */

/**
 * This function requests a reset.
 * It requests it and doesn't expect that it will be immediately.
 * Because the application needs time to change to a safe state before.
 */
extern void vscp_portable_resetRequest(void)
{
    log_printf("Reset request.\n");

    return;
}

#if VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_BOOT_LOADER_SUPPORTED )

/**
 * This function returns the supported boot loader algorithm.
 *
 * @return  Boot loader algorithm
 * @retval  0xFF    No boot loader supported
 */
extern uint8_t  vscp_portable_getBootLoaderAlgorithm(void)
{
    return VSCP_BOOTLOADER_ALGORITHM;
}

/**
 * This function requests a jump to the bootloader.
 * It requests it and doesn't expect that it will be immediately.
 * Because the application needs time to change to a safe state before.
 */
extern void vscp_portable_bootLoaderRequest(void)
{
    log_printf("Bootloader mode requested.\n");

    vscp_portable_bootloaderRequested = TRUE;

    return;
}

#endif  /* VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_BOOT_LOADER_SUPPORTED ) */

/**
 * This function return the bootloader request flag.
 *
 * @return Bootloader mode requested or not
 */
extern BOOL vscp_portable_isBootloaderRequested(void)
{
    BOOL    flag    = vscp_portable_bootloaderRequested;

    vscp_portable_bootloaderRequested = FALSE;

    return flag;
}

/**
 * This function provides received VSCP events, except the PROTOCOL class.
 *
 * @param[in]   msg Message
 */
extern void vscp_portable_provideEvent(vscp_RxMessage const * const msg)
{
    /* Implement your code here ... */

    return;
}

#if VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_PROTOCOL_EVENT_NOTIFICATION )

/**
 * This function provides received VSCP PROTOCOL class events.
 *
 * Attention: Handling events which the core is waiting for can cause bad
 * behaviour.
 * 
 * @param[in]   msg Message
 * 
 * @return Event handled or not. If application handles event, the core won't handle it.
 * @retval FALSE    Event not handled
 * @retval TRUE     Event handled
 */
extern BOOL vscp_portable_provideProtocolEvent(vscp_RxMessage const * const msg)
{
    BOOL isEventHandled = FALSE;

    /* Implement your code here ... */

    return isEventHandled;
}

#endif  /* VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_PROTOCOL_EVENT_NOTIFICATION ) */

#if VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_SEGMENT_TIME_CALLOUT )

/**
 * This function is called for every received segment master heartbeat event,
 * in case it contains a new time since epoch.
 * 
 * @param timestamp Unix timestamp
 */
extern void vscp_portable_updateTimeSinceEpoch(uint32_t timestamp)
{
    /* Implement your code here ... */

    return;
}

#endif  /* VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_SEGMENT_TIME_CALLOUT ) */

#if VSCP_CONFIG_BASE_IS_ENABLED( VSCP_DEV_DATA_CONFIG_ENABLE_GUID_STORAGE_EXT )

/**
 * This function returns one byte of the GUID, which is selected by the index.
 * Index 0 corresponds with the GUID LSB byte, index 15 with the GUID MSB byte.
 * 
 * Note, this function can be used to get the MCU stored GUID.
 *
 * @param[in]   index   Index in the GUID [0-15]
 * @return  GUID byte
 */
extern uint8_t  vscp_portable_readGUID(uint8_t index)
{
    uint8_t value = 0;

    /* Implement your code here ... */

    return value;
}

#endif  /* VSCP_CONFIG_BASE_IS_ENABLED( VSCP_DEV_DATA_CONFIG_ENABLE_GUID_STORAGE_EXT ) */

#if VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_CUSTOM_HEARTBEAT )

/**
 * Custom node heartbeat event function implementation.
 *
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 */
extern BOOL vscp_portable_sendNodeHeartbeatEvent() {
   
    /* Implement your version of vscp_information_sendNodeHeartbeatEvent() */

    return TRUE;
}

#endif  /* VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_CUSTOM_HEARTBEAT ) */

/*******************************************************************************
    LOCAL FUNCTIONS
*******************************************************************************/

#if VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM )

#if VSCP_CONFIG_BASE_IS_DISABLED( VSCP_CONFIG_ENABLE_DM_EXTENSION )

/**
 * This function loads a standard decision matrix xml file.
 */
static void vscp_portable_loadDMStd(void)
{
    uint8_t*            ps      = eeprom_getBase(NULL);
    DM_STD_XML_RET      ret     = DM_STD_XML_RET_OK;
    vscp_dm_MatrixRow*  dm      = NULL;

    if (NULL == ps)
    {
        LOG_ERROR("Couldn't get EEPROM base address.");
        return;
    }

    dm = (vscp_dm_MatrixRow*)&ps[VSCP_PS_ADDR_DM];

    /* If a decision matrix file is available, load it and configure the internal
     * decision matrix with the content.
     *
     * Otherwise the decision matrix will be empty.
     */
    ret = dm_std_xml_load("dmStd.xml", dm, VSCP_CONFIG_DM_ROWS);

    if (ret == DM_STD_XML_RET_EFILE)
    {
        LOG_INFO("No standard decision matrix xml file found.");
    }
    else if (ret != DM_STD_XML_RET_OK)
    {
        LOG_ERROR("Standard decision matrix xml file is invalid.");
    }
    else
    {
        LOG_INFO("Standard decision matrix xml file successful loaded.");
    }

    return;
}

#endif  /* VSCP_CONFIG_BASE_IS_DISABLED( VSCP_CONFIG_ENABLE_DM_EXTENSION ) */

#endif  /* VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM ) */

#if VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM_EXTENSION )

/**
 * This function loads a extended decision matrix xml file.
 */
static void vscp_portable_loadDMExt(void)
{
    uint8_t*            ps      = eeprom_getBase(NULL);
    DM_EXT_XML_RET      ret     = DM_EXT_XML_RET_OK;
    vscp_dm_MatrixRow*  dm      = NULL;
    vscp_dm_ExtRow*     dmExt   = NULL;

    if (NULL == ps)
    {
        LOG_ERROR("Couldn't get EEPROM base address.");
        return;
    }

    dm = (vscp_dm_MatrixRow*)&ps[VSCP_PS_ADDR_DM];

    dmExt = (vscp_dm_ExtRow*)&ps[VSCP_PS_ADDR_DM_EXTENSION];

    /* If a extended decision matrix file is available, load it and configure the internal
     * decision matrix with the content.
     *
     * Otherwise the decision matrix will be empty.
     */
    ret = dm_ext_xml_load("dmExt.xml", dm, dmExt, VSCP_CONFIG_DM_ROWS);

    if (ret == DM_EXT_XML_RET_EFILE)
    {
        LOG_INFO("No extended decision matrix xml file found.");
    }
    else if (ret != DM_EXT_XML_RET_OK)
    {
        LOG_ERROR("Extended decision matrix xml file is invalid.");
    }
    else
    {
        LOG_INFO("Extended decision matrix xml file successful loaded.");
    }

    return;
}

#endif  /* VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM_EXTENSION ) */
