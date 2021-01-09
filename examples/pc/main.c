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
#include <stdint.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include "eeprom.h"
#include "log.h"
#include "platform.h"
#include "cmdLineParser.h"
#include "vscp_core.h"
#include "vscp_ps_access.h"
#include "vscp_tp_adapter.h"
#include "vscp_class_l1.h"
#include "vscp_type_information.h"
#include "vscp_ps.h"
#include "vscp_thread.h"
#include "temperature_sim.h"
#include "vscphelperlib.h"
#include "lamp_sim.h"

/*******************************************************************************
    COMPILER SWITCHES
*******************************************************************************/

/*******************************************************************************
    CONSTANTS
*******************************************************************************/

/** Program name */
#define MAIN_PROG_NAME          "VSCP level 1 node"

/** Copyright */
#define MAIN_COPYRIGHT          "(c) 2014 - 2021 Andreas Merkle"

/** Persistent memory filename */
#define MAIN_EEPROM_FILENAME    "eeprom.asc"

/** Default log level */
#define MAIN_LOG_LEVEL_DEFAULT  (LOG_LEVEL_FATAL)

/*******************************************************************************
    MACROS
*******************************************************************************/

/** Use this macro to state that a variable is not used. */
#define MAIN_NOT_USED(__var)    (void)(__var)

/** Number of elements in a array */
#define MAIN_ARRAY_NUM(__array) (sizeof(__array) / sizeof((__array)[0]))

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

/** This type contains the information from the command line arguments. */
typedef struct
{
    char const *        progName;                   /**< Program name without path */
    char const *        daemonAddr;                 /**< Daemon ip address */
    char const *        daemonUser;                 /**< User name for daemon ip access */
    char const *        daemonPassword;             /**< Password for daemon ip access */
    char const *        nodeGuid;                   /**< Node GUID */
    BOOL                disableHeartbeat;           /**< Disable node heartbeat */
    BOOL                disableTemperature;         /**< Disable temperature simulation */
    BOOL                verbose;                    /**< Verbose information */
    BOOL                showHelp;                   /**< Show help to the user */
    VSCP_TP_ADAPTER_LVL lvl;                        /**< Network level */

} main_CmdLineArgs;

/*******************************************************************************
    PROTOTYPES
*******************************************************************************/

static MAIN_RET main_init(void);
static void main_deInit(void);
static void main_showKeyTable(void);
static void main_dumpEEPROM(void);
static void main_loop(main_CmdLineArgs *main_cmdLineArgs);
static CMDLINEPARSER_RET main_clpUnknown(void* const userData, char const * const arg, char const * const par);
static CMDLINEPARSER_RET main_clpLevel(void* const userData, char const * const arg, char const * const par);

/*******************************************************************************
    LOCAL VARIABLES
*******************************************************************************/

/** Command line argument structure, which is initialized after parsing. */
static main_CmdLineArgs         main_cmdLineArgs    =
{
    NULL,                           /* Program name */
    NULL,                           /* Daemon address */
    NULL,                           /* Daemon user name */
    NULL,                           /* Daemon password */
    NULL,                           /* Node GUID */
    FALSE,                          /* Disable node heartbeat */
    FALSE,                          /* Disable temperature simuluation */
    FALSE,                          /* Verbose output */
    FALSE,                          /* Show help */
    VSCP_TP_ADAPTER_LVL_1_OVER_2    /* Network level */
};

/** Configuration for the command line parser. */
static const cmdLineParser_Arg  main_clpConfig[]    =
{
    /* Special to retrieve the program name without path */
    { CMDLINEPARSER_PROG_NAME_WP,   &main_cmdLineArgs.progName,         NULL,                                   NULL,               NULL,               NULL                                                },
    /* Get every unknown command line argument */
    { CMDLINEPARSER_UNKONWN,        NULL,                               NULL,                                   main_clpUnknown,    NULL,               NULL                                                },
    /* Possible command line arguments */
    { "-a <ip-address>",            &main_cmdLineArgs.daemonAddr,       NULL,                                   NULL,               NULL,               "IP address of VSCP daemon"                         },
    { "-dheart",                    NULL,                               &main_cmdLineArgs.disableHeartbeat,     NULL,               NULL,               "Disable node heartbeat"                            },
    { "-dtemp",                     NULL,                               &main_cmdLineArgs.disableTemperature,   NULL,               NULL,               "Disable node temperature simulation"               },
    { "-g <guid>",                  &main_cmdLineArgs.nodeGuid,         NULL,                                   NULL,               NULL,               "Node GUID,\ne.g. 00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:01"      },
    { "-h --help",                  NULL,                               &main_cmdLineArgs.showHelp,             NULL,               NULL,               "Show help"                                         },
    { "-v",                         NULL,                               &main_cmdLineArgs.verbose,              NULL,               NULL,               "Increase verbose level"                            },
    { NULL,                         NULL,                               NULL,                                   NULL,               NULL,               "Options only for daemon connection:"               },
    { "-l <level>",                 NULL,                               NULL,                                   main_clpLevel,      &main_cmdLineArgs,  "1: Support L1 events\n12: Support L1 and L1 over L2 events (default)"  },
    { "-p <password>",              &main_cmdLineArgs.daemonPassword,   NULL,                                   NULL,               NULL,               "Password for VSCP daemon access"                   },
    { "-u <user>",                  &main_cmdLineArgs.daemonUser,       NULL,                                   NULL,               NULL,               "User name for VSCP daemon access"                  }
};

/** User friendly names for persistent memory elements */
static const char*      main_psUserFriendlyName[]   =
{
#if (0 < VSCP_PS_SIZE_BOOT_FLAG)
    "Boot flag",
#endif  /* (0 < VSCP_PS_SIZE_BOOT_FLAG) */
    "Nickname id",
    "Segment controller CRC",
    "Node control flags",
    "User id",
#if (0 < VSCP_PS_SIZE_GUID)
    "GUID",
#endif  /* (0 < VSCP_PS_SIZE_GUID) */
#if (0 < VSCP_PS_SIZE_NODE_ZONE)
    "Node zone",
#endif  /* (0 < VSCP_PS_SIZE_NODE_ZONE) */
#if (0 < VSCP_PS_SIZE_NODE_SUB_ZONE)
    "Node sub-zone",
#endif  /* (0 < VSCP_PS_SIZE_NODE_SUB_ZONE) */
#if (0 < VSCP_PS_SIZE_MANUFACTURER_DEV_ID)
    "Manufacturer device id",
#endif  /* (0 < VSCP_PS_SIZE_MANUFACTURER_DEV_ID) */
#if (0 < VSCP_PS_SIZE_MANUFACTURER_SUB_DEV_ID)
    "Manufacturer sub device id",
#endif  /* (0 < VSCP_PS_SIZE_MANUFACTURER_SUB_DEV_ID) */
#if (0 < VSCP_PS_SIZE_MDF_URL)
    "MDF URL",
#endif  /* (0 < VSCP_PS_SIZE_MDF_URL) */
#if (0 < VSCP_PS_SIZE_STD_DEV_FAMILY_CODE)
    "Standard device familiy code",
#endif  /* (0 < VSCP_PS_SIZE_STD_DEV_FAMILY_CODE) */
#if (0 < VSCP_PS_SIZE_STD_DEV_TYPE)
    "Standard device type",
#endif  /* (0 < VSCP_PS_SIZE_STD_DEV_TYPE) */
#if (0 < VSCP_PS_SIZE_LOG_ID)
    "Log id",
#endif  /* (0 < VSCP_PS_SIZE_LOG_ID) */
#if (0 < VSCP_PS_SIZE_DM)
    "Decision matrix",
#endif  /* (0 < VSCP_PS_SIZE_DM) */
#if (0 < VSCP_PS_SIZE_DM_EXTENSION)
    "Decision matrix extension",
#endif  /* (0 < VSCP_PS_SIZE_DM_EXTENSION) */
#if (0 < VSCP_PS_SIZE_DM_NEXT_GENERATION)
    "Decision matrix next generation"
#endif  /* (0 < VSCP_PS_SIZE_DM_NEXT_GENERATION) */
};

/*******************************************************************************
    GLOBAL VARIABLES
*******************************************************************************/

/*******************************************************************************
    GLOBAL FUNCTIONS
*******************************************************************************/

/**
 * Main entry point.
 *
 * @param[in]   argc    Number of arguments
 * @param[in]   argv    Argument list
 * @return Program status
 * @retval  0       Successful
 * @retval  others  Failed
 */
int main(int argc, char* argv[])
{
    int     status  = 0;
    BOOL    abort   = FALSE;
    uint8_t index   = 0;

    printf("\n%s\n", MAIN_PROG_NAME);
    printf("Version: %s (%s)\n", VERSION, BUILDTYPE);
    printf("%s\n\n", MAIN_COPYRIGHT);

    /* Initialize all modules */
    if (MAIN_RET_OK != main_init())
    {
        abort = TRUE;
    }
    else
    /* Parse command line arguments */
    if (CMDLINEPARSER_RET_OK != cmdLineParser_parse(main_clpConfig, MAIN_ARRAY_NUM(main_clpConfig), argc, argv))
    {
        abort = TRUE;
    }
    else
    /* Show help? */
    if (TRUE == main_cmdLineArgs.showHelp)
    {
        printf("Usage: %s <options>\n\n", main_cmdLineArgs.progName);
        printf("Options:\n");
        cmdLineParser_show(main_clpConfig, MAIN_ARRAY_NUM(main_clpConfig));
    }
    /* Abort because of a invalid program argument or continue? */
    else
    {
        if (TRUE == main_cmdLineArgs.verbose)
        {
            /* Set log level */
            log_setLevel(LOG_LEVEL_INFO | LOG_LEVEL_DEBUG | LOG_LEVEL_WARNING | LOG_LEVEL_ERROR | LOG_LEVEL_FATAL);
        }

        /* Load EEPROM layout */
        eeprom_load(MAIN_EEPROM_FILENAME);
        
        /* Initialize VSCP framework */
        if (VSCP_CORE_RET_OK != vscp_core_init())
        {
            abort = TRUE;
        }
        else
        {
            /* Show the user which keys can be used. */
            main_showKeyTable();
            printf("\n");

            /* Shall a connection to a VSCP daemon be established? */
            if (NULL != main_cmdLineArgs.daemonAddr)
            {
                VSCP_TP_ADAPTER_RET ret = VSCP_TP_ADAPTER_RET_OK;

                log_printf("Connecting ...\n");

                ret = vscp_tp_adapter_connect(  main_cmdLineArgs.daemonAddr,
                                                main_cmdLineArgs.daemonUser,
                                                main_cmdLineArgs.daemonPassword,
                                                main_cmdLineArgs.lvl);

                if (VSCP_TP_ADAPTER_RET_OK != ret)
                {
                    printf("Connection failed to %s.\n", main_cmdLineArgs.daemonAddr);

                    if (VSCP_TP_ADAPTER_RET_INVALID_USER == ret)
                    {
                        log_printf("Invalid user.\n");
                    }
                    else if (VSCP_TP_ADAPTER_RET_INVALID_PASSWORD == ret)
                    {
                        log_printf("Invalid password.\n");
                    }
                    else if (VSCP_TP_ADAPTER_RET_TIMEOUT == ret)
                    {
                        log_printf("Connection timeout.\n");
                    }

                    abort = TRUE;
                }
                else
                {
                    log_printf("Connection successful.\n");
                }
            }
        }

        /* No error? */
        if (FALSE == abort)
        {
            /* Shall the node GUID be set? */
            if (NULL != main_cmdLineArgs.nodeGuid)
            {
                uint8_t nodeGuid[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
                
                /* Convert node GUID from string */
                if (VSCP_ERROR_SUCCESS != vscphlp_getGuidFromStringToArray(nodeGuid, main_cmdLineArgs.nodeGuid))
                {
                    log_printf("Invalid node GUID.\n");
                }
                else
                {
                    for(index = 0; index < MAIN_ARRAY_NUM(nodeGuid); ++index)
                    {
                        vscp_ps_writeGUID(MAIN_ARRAY_NUM(nodeGuid) - index - 1, nodeGuid[index]);
                    }
                }
            }
        
            /* Shall the node heartbeat be disabled? */
            if (TRUE == main_cmdLineArgs.disableHeartbeat)
            {
                vscp_core_enableHeartbeat(FALSE);
            }

            /* Start the whole VSCP framework */
            if (VSCP_THREAD_RET_OK != vscp_thread_start())
            {
                abort = TRUE;
            }
            /* Shall the temperature simulation be started? */
            else if (FALSE == main_cmdLineArgs.disableTemperature)
            {
                /* Start temperature simulation */
                if (TEMPERATURE_SIM_RET_OK != temperature_sim_start())
                {
                    LOG_ERROR("Temperature simulation failed.");
                }
            }
        }

        /* If no error happened, start main loop. */
        if (FALSE == abort)
        {
            main_loop(&main_cmdLineArgs);
        }

        printf("Please wait ...\n");

        /* Stop temperature simulation */
        temperature_sim_stop();

        /* Stop the whole VSCP framework */
        vscp_thread_stop();

        /* Shall a connection to a VSCP daemon be disconnected? */
        if (NULL != main_cmdLineArgs.daemonAddr)
        {
            vscp_tp_adapter_disconnect();
        }
    }

    if (TRUE == abort)
    {
        printf("\nAborted.\n");

        if (0 == status)
        {
            status = 1;
        }
    }

    main_deInit();
    
    return status;
}

/*******************************************************************************
    LOCAL FUNCTIONS
*******************************************************************************/

/**
 * This function initializes all relevant modules and the data.
 *
 * @return Status
 */
static MAIN_RET main_init(void)
{
    MAIN_RET    status  = MAIN_RET_OK;

    /* Initialize platform specific functions */
    platform_init();

    /* Initialize EEPROM simulation */
    eeprom_init(VSCP_PS_ADDR_NEXT);

    /* Initialize VSCP threads, which processing the VSCP framework and timers. */
    vscp_thread_init();

    /* Initialize temperature simulation */
    temperature_sim_init();

    return status;
}

/**
 * This function de-initializes all relevant modules and release all resources.
 */
static void main_deInit(void)
{
    eeprom_save(MAIN_EEPROM_FILENAME);
    eeprom_deInit();

    platform_deInit();
    
    return;
}

/**
 * This function prints the key table to the console.
 */
static void main_showKeyTable(void)
{
    printf("Keys:\n");
    printf("?       Show this key table\n");
    printf("e       Dump EEPROM\n");
    printf("h       Enable/Disable node heartbeat\n");
    printf("i       Start node segment initialization\n");
    printf("l       Show the state of all lamps\n");
    printf("q       Quit program\n");
    printf("1-5     Send info button event\n");

    return;
}

/**
 * Dump the EEPROM with user friendly description.
 */
static void main_dumpEEPROM(void)
{
    uint16_t    eepromSize  = 0;
    uint8_t*    eepromBase  = eeprom_getBase(&eepromSize);

    if (NULL == eepromBase)
    {
        printf("No EEPROM available.\n");
    }
    else
    {
        uint16_t        index       = 0;
        PLATFORM_COLOR  color       = PLATFORM_COLOR_RED;
        PLATFORM_COLOR  newColor    = PLATFORM_COLOR_RED;
        BOOL            nextColor   = FALSE;
        uint8_t         elementId   = 0;

        platform_setTextBgColor(PLATFORM_COLOR_BLACK);

        for(index = 0; index < eepromSize; ++index)
        {
            if (0 == (index % EEPROM_STORAGE_MULTIPLE))
            {
                platform_setTextColor(PLATFORM_COLOR_GREY);
                printf("%04X: ", index);
                platform_setTextColor(color);
            }

#if (0 < VSCP_PS_SIZE_BOOT_FLAG)
            if (VSCP_PS_ADDR_BOOT_FLAG == index)
            {
                nextColor = TRUE;
            }
            else
#endif  /* (0 < VSCP_PS_SIZE_BOOT_FLAG) */
            if (VSCP_PS_ADDR_NICKNAME == index)
            {
                nextColor = TRUE;
            }
            else if (VSCP_PS_ADDR_SEGMENT_CONTROLLER_CRC == index)
            {
                nextColor = TRUE;
            }
            else if (VSCP_PS_ADDR_NODE_CONTROL_FLAGS == index)
            {
                nextColor = TRUE;
            }
            else if (VSCP_PS_ADDR_USER_ID == index)
            {
                nextColor = TRUE;
            }
#if (0 < VSCP_PS_SIZE_GUID)
            else if (VSCP_PS_ADDR_GUID == index)
            {
                nextColor = TRUE;
            }
#endif  /* (0 < VSCP_PS_SIZE_GUID) */
#if (0 < VSCP_PS_SIZE_NODE_ZONE)
            else if (VSCP_PS_ADDR_NODE_ZONE == index)
            {
                nextColor = TRUE;
            }
#endif  /* (0 < VSCP_PS_SIZE_NODE_ZONE) */
#if (0 < VSCP_PS_SIZE_NODE_SUB_ZONE)
            else if (VSCP_PS_ADDR_NODE_SUB_ZONE == index)
            {
                nextColor = TRUE;
            }
#endif  /* (0 < VSCP_PS_SIZE_NODE_SUB_ZONE) */
#if (0 < VSCP_PS_SIZE_MANUFACTURER_DEV_ID)
            else if (VSCP_PS_ADDR_MANUFACTURER_DEV_ID == index)
            {
                nextColor = TRUE;
            }
#endif  /* (0 < VSCP_PS_SIZE_MANUFACTURER_DEV_ID) */
#if (0 < VSCP_PS_SIZE_MANUFACTURER_SUB_DEV_ID)
            else if (VSCP_PS_ADDR_MANUFACTURER_SUB_DEV_ID == index)
            {
                nextColor = TRUE;
            }
#endif  /* (0 < VSCP_PS_SIZE_MANUFACTURER_SUB_DEV_ID) */
#if (0 < VSCP_PS_SIZE_MDF_URL)
            else if (VSCP_PS_ADDR_MDF_URL == index)
            {
                nextColor = TRUE;
            }
#endif  /* (0 < VSCP_PS_SIZE_MDF_URL) */
#if (0 < VSCP_PS_SIZE_STD_DEV_FAMILY_CODE)
            else if (VSCP_PS_ADDR_STD_DEV_FAMILY_CODE == index)
            {
                nextColor = TRUE;
            }
#endif  /* (0 < VSCP_PS_SIZE_STD_DEV_FAMILY_CODE) */
#if (0 < VSCP_PS_SIZE_STD_DEV_TYPE)
            else if (VSCP_PS_ADDR_STD_DEV_TYPE == index)
            {
                nextColor = TRUE;
            }
#endif  /* (0 < VSCP_PS_SIZE_STD_DEV_TYPE) */
#if (0 < VSCP_PS_SIZE_LOG_ID)
            else if (VSCP_PS_ADDR_LOG_ID == index)
            {
                nextColor = TRUE;
            }
#endif  /* (0 < VSCP_PS_SIZE_LOG_ID) */
#if (0 < VSCP_PS_SIZE_DM)
            else if (VSCP_PS_ADDR_DM == index)
            {
                nextColor = TRUE;
            }
#endif  /* (0 < VSCP_PS_SIZE_DM) */
#if (0 < VSCP_PS_SIZE_DM_EXTENSION)
            else if (VSCP_PS_ADDR_DM_EXTENSION == index)
            {
                nextColor = TRUE;
            }
#endif  /* (0 < VSCP_PS_SIZE_DM_EXTENSION) */
#if (0 < VSCP_PS_SIZE_DM_NEXT_GENERATION)
            else if (VSCP_PS_ADDR_DM_NEXT_GENERATION == index)
            {
                nextColor = TRUE;
            }
#endif  /* (0 < VSCP_PS_SIZE_DM_NEXT_GENERATION) */
            else if (VSCP_PS_ADDR_NEXT <= index)
            {
                newColor = PLATFORM_COLOR_GREY;
                nextColor = TRUE;
            }

            if (TRUE == nextColor)
            {
                color = newColor;
                platform_setTextColor(color);
                ++newColor;

                if (PLATFORM_COLOR_WHITE < newColor)
                {
                    newColor = PLATFORM_COLOR_RED;
                }

                nextColor = FALSE;
            }

            printf("%02X", eepromBase[index]);

            if (0 != ((index + 1) % EEPROM_STORAGE_MULTIPLE))
            {
                printf(" ");
            }
            else
            {
                printf("\r\n");
            }
        }

        platform_restoreTextColor();

        color       = PLATFORM_COLOR_RED;
        newColor    = PLATFORM_COLOR_RED;

        for(index = 0; index < eepromSize; ++index)
        {
#if (0 < VSCP_PS_SIZE_BOOT_FLAG)
            if (VSCP_PS_ADDR_BOOT_FLAG == index)
            {
                nextColor = TRUE;
            }
            else
#endif  /* (0 < VSCP_PS_SIZE_BOOT_FLAG) */
            if (VSCP_PS_ADDR_NICKNAME == index)
            {
                nextColor = TRUE;
            }
            else if (VSCP_PS_ADDR_SEGMENT_CONTROLLER_CRC == index)
            {
                nextColor = TRUE;
            }
            else if (VSCP_PS_ADDR_NODE_CONTROL_FLAGS == index)
            {
                nextColor = TRUE;
            }
            else if (VSCP_PS_ADDR_USER_ID == index)
            {
                nextColor = TRUE;
            }
#if (0 < VSCP_PS_SIZE_GUID)
            else if (VSCP_PS_ADDR_GUID == index)
            {
                nextColor = TRUE;
            }
#endif  /* (0 < VSCP_PS_SIZE_GUID) */
#if (0 < VSCP_PS_SIZE_NODE_ZONE)
            else if (VSCP_PS_ADDR_NODE_ZONE == index)
            {
                nextColor = TRUE;
            }
#endif  /* (0 < VSCP_PS_SIZE_NODE_ZONE) */
#if (0 < VSCP_PS_SIZE_NODE_SUB_ZONE)
            else if (VSCP_PS_ADDR_NODE_SUB_ZONE == index)
            {
                nextColor = TRUE;
            }
#endif  /* (0 < VSCP_PS_SIZE_NODE_SUB_ZONE) */
#if (0 < VSCP_PS_SIZE_MANUFACTURER_DEV_ID)
            else if (VSCP_PS_ADDR_MANUFACTURER_DEV_ID == index)
            {
                nextColor = TRUE;
            }
#endif  /* (0 < VSCP_PS_SIZE_MANUFACTURER_DEV_ID) */
#if (0 < VSCP_PS_SIZE_MANUFACTURER_SUB_DEV_ID)
            else if (VSCP_PS_ADDR_MANUFACTURER_SUB_DEV_ID == index)
            {
                nextColor = TRUE;
            }
#endif  /* (0 < VSCP_PS_SIZE_MANUFACTURER_SUB_DEV_ID) */
#if (0 < VSCP_PS_SIZE_MDF_URL)
            else if (VSCP_PS_ADDR_MDF_URL == index)
            {
                nextColor = TRUE;
            }
#endif  /* (0 < VSCP_PS_SIZE_MDF_URL) */
#if (0 < VSCP_PS_SIZE_STD_DEV_FAMILY_CODE)
            else if (VSCP_PS_ADDR_STD_DEV_FAMILY_CODE == index)
            {
                nextColor = TRUE;
            }
#endif  /* (0 < VSCP_PS_SIZE_STD_DEV_FAMILY_CODE) */
#if (0 < VSCP_PS_SIZE_STD_DEV_TYPE)
            else if (VSCP_PS_ADDR_STD_DEV_TYPE == index)
            {
                nextColor = TRUE;
            }
#endif  /* (0 < VSCP_PS_SIZE_STD_DEV_TYPE) */
#if (0 < VSCP_PS_SIZE_LOG_ID)
            else if (VSCP_PS_ADDR_LOG_ID == index)
            {
                nextColor = TRUE;
            }
#endif  /* (0 < VSCP_PS_SIZE_LOG_ID) */
#if (0 < VSCP_PS_SIZE_DM)
            else if (VSCP_PS_ADDR_DM == index)
            {
                nextColor = TRUE;
            }
#endif  /* (0 < VSCP_PS_SIZE_DM) */
#if (0 < VSCP_PS_SIZE_DM_EXTENSION)
            else if (VSCP_PS_ADDR_DM_EXTENSION == index)
            {
                nextColor = TRUE;
            }
#endif  /* (0 < VSCP_PS_SIZE_DM_EXTENSION) */
#if (0 < VSCP_PS_SIZE_DM_NEXT_GENERATION)
            else if (VSCP_PS_ADDR_DM_NEXT_GENERATION == index)
            {
                nextColor = TRUE;
            }
#endif  /* (0 < VSCP_PS_SIZE_DM_NEXT_GENERATION) */
            else if (VSCP_PS_ADDR_NEXT <= index)
            {
                newColor = PLATFORM_COLOR_GREY;
                nextColor = TRUE;
            }

            if (TRUE == nextColor)
            {
                color = newColor;
                platform_setTextColor(color);
                ++newColor;

                if (MAIN_ARRAY_NUM(main_psUserFriendlyName) > elementId)
                {
                    printf("- %s\n", main_psUserFriendlyName[elementId]);
                    ++elementId;
                }

                if (PLATFORM_COLOR_WHITE < color)
                {
                    color = PLATFORM_COLOR_RED;
                }

                nextColor = FALSE;
            }
        }

        platform_restoreTextColor();
        platform_restoreTextBgColor();
    }

    return;
}

/**
 * This function contains the main loop where every user key press is
 * interpreted.
 *
 * @param[in] main_cmdLineArgs   Command line arguments
 */
static void main_loop(main_CmdLineArgs *main_cmdLineArgs)
{
    int     keyValue                = 0;
    BOOL    isNodeHeartbeatEnabled  = TRUE;

    if (NULL == main_cmdLineArgs)
    {
        return;
    }
    
    if (TRUE == main_cmdLineArgs->disableHeartbeat)
    {
        isNodeHeartbeatEnabled = FALSE;
    }
    
    platform_echoOff();

    /* Execute simple terminal */
    while('q' != keyValue)
    {
        /* Any button pressed? */
        if (0 < platform_kbhit())
        {
            keyValue = platform_getch();

            platform_echoOn();

            /* Show keys */
            if ('?' == keyValue)
            {
                vscp_thread_lock();
                main_showKeyTable();
                vscp_thread_unlock();
            }
            /* Dump EEPROM */
            else if ('e' == keyValue)
            {
                vscp_thread_lock();
                main_dumpEEPROM();
                vscp_thread_unlock();
            }
            /* Enable/Disable node hearbeat */
            else if ('h' == keyValue)
            {
                vscp_thread_lock();
                if (TRUE == isNodeHeartbeatEnabled)
                {
                    printf("Disable node heartbeat.\n");
                    isNodeHeartbeatEnabled = FALSE;
                }
                else
                {
                    printf("Enable node heartbeat.\n");
                    isNodeHeartbeatEnabled = TRUE;
                }
                vscp_core_enableHeartbeat(isNodeHeartbeatEnabled);
                vscp_thread_unlock();
            }
            /* Start node segment initialization */
            else if ('i' == keyValue)
            {
                vscp_thread_lock();
                printf("Start node segment initialization.\n");
                vscp_core_startNodeSegmentInit();
                vscp_thread_unlock();
            }
            /* Show the state of all lamps */
            else if ('l' == keyValue)
            {
                printf("\n");
                vscp_thread_lock();
                lamp_sim_show();
                vscp_thread_unlock();
                printf("\n");
            }
            /* Quit program */
            else if ('q' == keyValue)
            {
                vscp_thread_lock();
                printf("Quit.\n");
                vscp_thread_unlock();
            }
            /* Send a button message to the VSCP thread */
            else if (('1' <= keyValue) && ('5' >= keyValue))
            {
                vscp_RxMessage  rxMsg;

                rxMsg.vscpClass = VSCP_CLASS_L1_INFORMATION;
                rxMsg.vscpType  = VSCP_TYPE_INFORMATION_BUTTON;
                rxMsg.priority  = VSCP_PRIORITY_3_NORMAL;
                rxMsg.oAddr     = (uint8_t)(keyValue - '0') + 1;
                rxMsg.hardCoded = FALSE;
                rxMsg.dataNum   = 7;
                rxMsg.data[0]   = (uint8_t)(keyValue - '0');
                rxMsg.data[1]   = 0xff;
                rxMsg.data[2]   = 0xff;
                rxMsg.data[3]   = 0;
                rxMsg.data[4]   = 0;
                rxMsg.data[5]   = 0;
                rxMsg.data[6]   = 0;

                vscp_thread_lock();
                vscp_tp_adapter_simulateReceivedMessage(&rxMsg);
                vscp_thread_unlock();
            }

            platform_echoOff();
        }
        else
        {
            /* Give other programs a chance. */
            platform_delay(10);
        }
    }

    platform_echoOn();

    return;
}

/**
 * This function is called by the command line parser for every unknown argument.
 *
 * @param[in]   userData    User data
 * @param[in]   arg         Argument name
 * @param[in]   par         Array of parameter
 * @param[in]   num         Number of parameters in the array
 *
 * @return Status
 */
static CMDLINEPARSER_RET main_clpUnknown(void* const userData, char const * const arg, char const * const par)
{
    printf("Unknown command line argument: %s\n", arg);

    return CMDLINEPARSER_RET_ERROR;
}

/**
 * This function is called by the command line parser for network level.
 *
 * @param[in]   userData    User data
 * @param[in]   arg         Argument name
 * @param[in]   par         Array of parameter
 * @param[in]   num         Number of parameters in the array
 *
 * @return Status
 */
static CMDLINEPARSER_RET main_clpLevel(void* const userData, char const * const arg, char const * const par)
{
    CMDLINEPARSER_RET       ret         = CMDLINEPARSER_RET_OK;
    main_CmdLineArgs* const cmdLineArgs = (main_CmdLineArgs* const)userData;
    int                     value       = 0;
    
    if ((NULL == userData) ||
        (NULL == arg) ||
        (NULL == par))
    {
        ret = CMDLINEPARSER_RET_ERROR;
    }
    else
    {
        value = atoi(par);
        
        if (1 == value)
        {
            cmdLineArgs->lvl = VSCP_TP_ADAPTER_LVL_1;
        }
        else if (12 == value)
        {
            cmdLineArgs->lvl = VSCP_TP_ADAPTER_LVL_1_OVER_2;
        }
        else
        {
            printf("Invalid network level %s\n", par);
            ret = CMDLINEPARSER_RET_ERROR;
        }
    }

    return ret;
}
