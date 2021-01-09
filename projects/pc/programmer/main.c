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

*******************************************************************************/
/**
@brief  Main entry point
@file   main.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc
This module contains the main entry point.

*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include <stdint.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"
#include "platform.h"
#include "vscphelperlib.h"
#include "cmdLineParser.h"
#include "intelHexParser.h"
#include "crc16ccitt.h"

/*******************************************************************************
    COMPILER SWITCHES
*******************************************************************************/

/*******************************************************************************
    CONSTANTS
*******************************************************************************/

/** Program name */
#define MAIN_PROG_NAME                  "VSCP L1 programmer"

/** Copyright */
#define MAIN_COPYRIGHT                  "(c) 2014 - 2021 Andreas Merkle"

/** Default log level */
#define MAIN_LOG_LEVEL_DEFAULT          (LOG_LEVEL_FATAL)

/** Daemon command response timeout in ms */
#define MAIN_RESPONSE_TIMEOUT           6000

/** Daemon wait time after a command was executed in ms */
#define MAIN_CMD_WAIT_TIME              250

/** Level 1 events threshold */
#define MAIN_VSCP_L1_THRESHOLD          512

/** Max. number of block transfer retries. */
#define MAIN_MAX_BLOCK_TRANSFER_RETRIES 3

/** Fill byte */
#define MAIN_BLOCK_FILL_BYTE            (0x00)

/*******************************************************************************
    MACROS
*******************************************************************************/

/** Use this macro to state that a variable is not used. */
#define MAIN_NOT_USED(__var)    (void)(__var)

/** Number of elements in a array */
#define MAIN_ARRAY_NUM(__array) (sizeof(__array) / sizeof((__array)[0]))

/** Build a uint32_t variable from bytes */
#define MAIN_BUILD_UINT32(__byteMSB, __byte2, __byte1, __byteLSB)   \
    (((uint32_t)(__byteMSB) & 0xff) << 24) |                        \
    (((uint32_t)(__byte2)   & 0xff) << 16) |                        \
    (((uint32_t)(__byte1)   & 0xff) <<  8) |                        \
    (((uint32_t)(__byteLSB) & 0xff) <<  0)

/*******************************************************************************
    TYPES AND STRUCTURES
*******************************************************************************/

/** This type defines the supported function return values. */
typedef enum
{
    MAIN_RET_OK = 0,            /**< Successful */
    MAIN_RET_INVALID_USER,      /**< Invalid user */
    MAIN_RET_INVALID_PASSWORD,  /**< Invalid password */
    MAIN_RET_TIMEOUT,           /**< Connection timeout */
    MAIN_RET_ERROR,             /**< Failed */
    MAIN_RET_ENULL              /**< Unexpected NULL pointer */

} MAIN_RET;

/** This type contains the information from the command line arguments. */
typedef struct
{
    char const *    progName;       /**< Program name without path */
    char const *    daemonAddr;     /**< Daemon ip address */
    char const *    daemonUser;     /**< User name for daemon ip access */
    char const *    daemonPassword; /**< Password for daemon ip access */
    char const *    iHexFileName;   /**< File name of a intel hex format file */
    char const *    nodeId;         /**< Nickname of the node, which shall be programmed */
    char const *    nodeGuid;       /**< GUID of the node, which shall be programmed */
    char const *    bootLoaderAlgo; /**< Boot loader algorithm */
    char const *    pageSelect;     /**< Register page select value, used for enter boot loader event */
    BOOL            showHelp;       /**< Show help to the user */
    BOOL            verbose;        /**< Verbose output */
    BOOL            fillBlock;      /**< If set, the last block will be filled with a fill byte. */

} main_CmdLineArgs;

/** This type defines a log message. */
typedef struct _main_Msg
{
    uint8_t             id;     /**< ID */
    char const *        text;   /**< Message text */
    struct _main_Msg    *next;  /**< Next message */

} main_Msg;

/** This type defines the programming state machine. */
typedef enum
{
    MAIN_PRG_STATE_READ_REG_PAGE_SELECT = 0,    /**< Read page select register */
    MAIN_PRG_STATE_READ_REG_PAGE_SELECT_RSP,    /**< Wait for read page select register response */
    MAIN_PRG_STATE_ENTER_BOOT_LOADER_MODE,      /**< Enter boot loader mode */
    MAIN_PRG_STATE_ENTER_BOOT_LOADER_MODE_ACK,  /**< Wait for enter boot loader mode acknowledge */
    MAIN_PRG_STATE_START_BLOCK_TRANSFER,        /**< Start block transfer */
    MAIN_PRG_STATE_START_BLOCK_TRANSFER_ACK,    /**< Wait for start block transfer acknowledge */
    MAIN_PRG_STATE_BLOCK_DATA,                  /**< Block transfer */
    MAIN_PRG_STATE_BLOCK_DATA_ACK,              /**< Wait for block transfer acknowledge */
    MAIN_PRG_STATE_PROGRAM_BLOCK,               /**< Program block */
    MAIN_PRG_STATE_PROGRAM_BLOCK_ACK,           /**< Wait for program block acknowledge */
    MAIN_PRG_STATE_ACTIVATE_NEW_IMAGE,          /**< Activate new image */
    MAIN_PRG_STATE_ACTIVATE_NEW_IMAGE_ACK,      /**< Wait for new image activation */
    MAIN_PRG_STATE_ERROR,                       /**< Error happened */
    MAIN_PRG_STATE_IDLE                         /**< Idle, nothing to do */

} MAIN_PRG_STATE;

/** This type defines the programming context. */
typedef struct
{
    MAIN_PRG_STATE  state;              /**< Programming state */

    uint8_t         nodeId;             /**< Node id */
    uint8_t         nodeGuid[16];       /**< Node GUID */
    uint8_t         bootLoaderAlgo;     /**< Boot loader algorithm */

    uint16_t        pageSelect;         /**< Register page select content */
    uint32_t        blockSize;          /**< Block size in byte, received from the remote node */
    uint32_t        blockNum;           /**< Number of blocks, received from the remote node */
    uint32_t        blockIndex;         /**< Current block index, which is transfered */
    uint32_t        blockFragmentIndex; /**< Current block fragment index, which is transfered */
    BOOL            fillBlock;          /**< If set, the last block will be filled with a fill byte. */
    uint32_t        recIndex;           /**< Current used intel hex record */
    uint32_t        recDataIndex;       /**< Current intel hex record data index */
    Crc16CCITT      blockCrcCalculated; /**< Calculated CRC over a single block */
    uint8_t         blockRetry;         /**< Number of current block transfer retries */

    uint32_t        recIndexBackup;     /**< Backup of record index, needed for block transfer retry */
    uint32_t        recDataIndexBackup; /**< Backup of record data index, needed for block transfer retry */

} main_Programming;

/*******************************************************************************
    PROTOTYPES
*******************************************************************************/

static MAIN_RET main_init(void);
static void main_deInit(void);
static void main_showKeyTable(void);
static MAIN_RET main_programming(long hSession, intelHexParser_Record* recSet, uint32_t recNum);
static MAIN_RET main_connect(long * const hSession, char const * const ipAddr, char const * const user, char const * const password);
static void main_disconnect(long * const hSession);
static CMDLINEPARSER_RET main_clpUnknown(void* const userData, char const * const arg, char const * const par);
static Crc16CCITT main_calculateCrc(intelHexParser_Record* recSet, uint32_t recNum, uint32_t blockSize, BOOL fillBlock);
static uint32_t main_calculateDataSize(intelHexParser_Record* recSet, uint32_t recNum);
static void main_programNode(main_Programming * const progCon, long hSession, intelHexParser_Record* recSet, uint32_t recNum, vscpEventEx const * const rxEvent);

/*******************************************************************************
    LOCAL VARIABLES
*******************************************************************************/

/** Command line argument structure, which is initialized after parsing. */
static main_CmdLineArgs         main_cmdLineArgs    =
{
    NULL,   /* Program name */
    NULL,   /* Daemon address */
    NULL,   /* Daemon user name */
    NULL,   /* Daemon password */
    NULL,   /* File name of a intel hex format file */
    NULL,   /* Node id */
    NULL,   /* Node GUID */
    NULL,   /* Boot loader algorithm */
    NULL,   /* Register page select value */
    FALSE,  /* Show help */
    FALSE,  /* Verbose output */
    FALSE   /* Fill block */
};

/** Configuration for the command line parser. */
static const cmdLineParser_Arg  main_clpConfig[]    =
{
    /* Special to retrieve the program name without path */
    { CMDLINEPARSER_PROG_NAME_WP,   &main_cmdLineArgs.progName,         NULL,                           NULL,               NULL,   NULL                                                },
    /* Get every unknown command line argument */
    { CMDLINEPARSER_UNKONWN,        NULL,                               NULL,                           main_clpUnknown,    NULL,   NULL                                                },
    /* Possible command line arguments */
    { "-a <ip-address>",            &main_cmdLineArgs.daemonAddr,       NULL,                           NULL,               NULL,   "IP address of VSCP daemon"                         },
    { "-b <algorithm>",             &main_cmdLineArgs.bootLoaderAlgo,   NULL,                           NULL,               NULL,   "Bootloader algorithm (default: 0)"                 },
    { "-f <file name>",             &main_cmdLineArgs.iHexFileName,     NULL,                           NULL,               NULL,   "Intel hex format file"                             },
    { "-fillBlock",                 NULL,                               &main_cmdLineArgs.fillBlock,    NULL,               NULL,   "Fill block with 0x00 up (default: not filling)"    },
    { "-g <guid>",                  &main_cmdLineArgs.nodeGuid,         NULL,                           NULL,               NULL,   "Node GUID,\ne.g. 00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:01"      },
    { "-h --help",                  NULL,                               &main_cmdLineArgs.showHelp,     NULL,               NULL,   "Show help"                                         },
    { "-n <node nickname>",         &main_cmdLineArgs.nodeId,           NULL,                           NULL,               NULL,   "Nickname of the node, which shall be programmed."  },
    { "-p <password>",              &main_cmdLineArgs.daemonPassword,   NULL,                           NULL,               NULL,   "Password for VSCP daemon access"                   },
    { "-ps <page select value>",    &main_cmdLineArgs.pageSelect,       NULL,                           NULL,               NULL,   "Use the given page select register value for enter boot loader event." },
    { "-u <user>",                  &main_cmdLineArgs.daemonUser,       NULL,                           NULL,               NULL,   "User name for VSCP daemon access"                  },
    { "-v",                         NULL,                               &main_cmdLineArgs.verbose,      NULL,               NULL,   "Increase verbose level"                            }
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
    int                     status          = 0;
    BOOL                    abort           = FALSE;
    long                    hSession        = 0;
    MAIN_RET                ret             = MAIN_RET_OK;
    INTELHEXPARSER_RET      iHexParserRet   = INTELHEXPARSER_RET_OK;
    intelHexParser_Record*  iHexRecordSet   = NULL;
    uint32_t                iHexRecordNum   = 0;

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
    /* Is the daemon ip-address missing? */
    else if (NULL == main_cmdLineArgs.daemonAddr)
    {
        printf("Where to connect to?\n");
        abort = TRUE;
    }
    /* Is the file name of a intel hex format file missing? */
    else if (NULL == main_cmdLineArgs.iHexFileName)
    {
        printf("What to program?\n");
        abort = TRUE;
    }
    /* Is the node id missing? */
    else if ((NULL == main_cmdLineArgs.nodeId) ||
             (NULL == main_cmdLineArgs.nodeGuid))
    {
        printf("Which node to program?\n");
        abort = TRUE;
    }
    else
    {
        if (TRUE == main_cmdLineArgs.verbose)
        {
            /* Set log level */
            log_setLevel(LOG_LEVEL_INFO | LOG_LEVEL_DEBUG | LOG_LEVEL_WARNING | LOG_LEVEL_ERROR | LOG_LEVEL_FATAL);
        }

        /* Load intel hex format file, parse it and get it as a set of records. */
        iHexParserRet = intelHexParser_load(main_cmdLineArgs.iHexFileName, &iHexRecordSet, &iHexRecordNum);

        if (INTELHEXPARSER_RET_OK != iHexParserRet)
        {
            uint32_t    column  = 0;
            uint32_t    line    = 0;

            intelHexParser_getLastError(&line, &column);

            if (INTELHEXPARSER_RET_FILE_NOT_FOUND == iHexParserRet)
            {
                printf("File not found: %s\n", main_cmdLineArgs.iHexFileName);
            }
            else
            {
                printf("Invalid intel hex format file (error %d, line %u, column %u).\n", iHexParserRet, line, column);
            }

            abort = TRUE;
        }
        else
        {
            printf("Connecting to %s", main_cmdLineArgs.daemonAddr);
            printf(" with credentials: %s@%s\n", main_cmdLineArgs.daemonUser, main_cmdLineArgs.daemonPassword);

            ret = main_connect( &hSession,
                                main_cmdLineArgs.daemonAddr,
                                main_cmdLineArgs.daemonUser,
                                main_cmdLineArgs.daemonPassword);

            if (MAIN_RET_OK != ret)
            {
                printf("Connection failed to %s.\n", main_cmdLineArgs.daemonAddr);

                if (MAIN_RET_INVALID_USER == ret)
                {
                    printf("Invalid user.\n");
                }
                else if (MAIN_RET_INVALID_PASSWORD == ret)
                {
                    printf("Invalid password.\n");
                }
                else if (MAIN_RET_TIMEOUT == ret)
                {
                    printf("Connection timeout.\n");
                }

                abort = TRUE;
            }
            else
            {
                printf("Connection successful.\n");

                /* If no error happened, start programming. */
                if (FALSE == abort)
                {
                    if (MAIN_RET_OK != main_programming(hSession, iHexRecordSet, iHexRecordNum))
                    {
                        printf("Programming the device failed.\n");
                        abort = TRUE;
                    }
                }

                printf("Please wait ...\n");

                main_disconnect(&hSession);

                printf("Disconnected\n");
            }

            intelHexParser_freeRecSet(&iHexRecordSet);
            iHexRecordNum = 0;
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

    /* Set log level */
    log_setLevel(MAIN_LOG_LEVEL_DEFAULT);

    /* Initialize command line parser */
    cmdLineParser_init();

    return status;
}

/**
 * This function de-initializes all relevant modules and release all resources.
 */
static void main_deInit(void)
{
    platform_deInit();

    return;
}

/**
 * This function prints the key table to the console.
 */
static void main_showKeyTable(void)
{
    printf("Keys:\n");
    printf("?   Show this key table\n");
    printf("q   Quit program\n");

    return;
}

/**
 * This function programs the device with the given records.
 *
 * @param[in] hSession  Daemon connection session
 * @param[in] recSet    Record set (intel hex format)
 * @param[in] recNum    Number of records in the record set
 *
 * @return Status of the operation
 */
static MAIN_RET main_programming(long hSession, intelHexParser_Record* recSet, uint32_t recNum)
{
    int                 keyValue    = 0;
    BOOL                quit        = FALSE;
    MAIN_RET            ret         = MAIN_RET_OK;
    main_Programming    progCon;
    int                 tmp         = 0;

    if ((NULL == recSet) ||
        (0 == recNum))
    {
        return MAIN_RET_ENULL;
    }

    /* Initialize programming context */
    memset(&progCon, 0, sizeof(progCon));

    /* Node id missing? */
    if (NULL == main_cmdLineArgs.nodeId)
    {
        return MAIN_RET_ERROR;
    }
    
    /* Node GUID missing? */
    if (NULL == main_cmdLineArgs.nodeGuid)
    {
        return MAIN_RET_ERROR;
    }

    /* Convert node id from string */
    tmp = atoi(main_cmdLineArgs.nodeId);
    if ((0 > tmp) ||
        (255 < tmp))
    {
        printf("Invalid node id.\n");
        return MAIN_RET_ERROR;
    }
    progCon.nodeId = tmp;

    /* Convert node GUID from string */
    if (VSCP_ERROR_SUCCESS != vscphlp_getGuidFromStringToArray(progCon.nodeGuid, main_cmdLineArgs.nodeGuid))
    {
        printf("Invalid node GUID.\n");
        return MAIN_RET_ERROR;
    }
    
    /* Boot loader algorithm missing? */
    if (NULL == main_cmdLineArgs.bootLoaderAlgo)
    {
        /* Default */
        progCon.bootLoaderAlgo = 0;
    }
    else
    {
        /* Convert boot loader algorithm from string */
        tmp = atoi(main_cmdLineArgs.bootLoaderAlgo);
        if ((0 > tmp) ||
            (255 < tmp))
        {
            printf("Invalid boot loader algorithm.\n");
            return MAIN_RET_ERROR;
        }
        progCon.bootLoaderAlgo = tmp;
    }
    
    /* Is a page select value available? */
    if (NULL != main_cmdLineArgs.pageSelect)
    {
        /* Convert page select value from string */
        tmp = atoi(main_cmdLineArgs.pageSelect);
        if ((0x0000 > tmp) ||
            (0xffff < tmp))
        {
            printf("Invalid page select value.\n");
            return MAIN_RET_ERROR;
        }
        progCon.state = MAIN_PRG_STATE_ENTER_BOOT_LOADER_MODE;
    }
    
    /* Fill block up? */
    progCon.fillBlock = main_cmdLineArgs.fillBlock;

    platform_echoOff();

    /* Execute simple terminal */
    while(FALSE == quit)
    {
        /* Any button pressed? */
        if (0 < platform_kbhit())
        {
            keyValue = platform_getch();

            platform_echoOn();

            /* Show keys */
            if ('?' == keyValue)
            {
                main_showKeyTable();
            }
            /* Quit program */
            else if ('q' == keyValue)
            {
                printf("Quit.\n");
                quit = TRUE;
            }

            platform_echoOff();
        }
        else
        {
            uint32_t        count       = 0;
            vscpEventEx     daemonEvent;
            vscpEventEx*    rxEvent     = NULL;
            int             vscphlpRet  = 0;

            /* Check for available events. */
            if (VSCP_ERROR_SUCCESS != (vscphlpRet = vscphlp_isDataAvailable(hSession, &count)))
            {
                LOG_ERROR_INT32("Couldn't check for available data: ", vscphlpRet);

                printf("Connection lost.\n");
                
                /* Abort */
                quit = TRUE;
            }
            /* Any event available? */
            else if (0 < count)
            {
                if (VSCP_ERROR_SUCCESS != (vscphlpRet = vscphlp_receiveEventEx(hSession, &daemonEvent)))
                {
                    LOG_WARNING_INT32("Couldn't receive event: ", vscphlpRet);
                }
                else if (MAIN_VSCP_L1_THRESHOLD > daemonEvent.vscp_class)
                {
                    rxEvent = &daemonEvent;
                }
            }

            main_programNode(&progCon, hSession, recSet, recNum, rxEvent);

            /* Give other programs a chance. */
            platform_delay(1);
        }
    }

    platform_echoOn();

    return ret;
}

/**
 * This function connects to a VSCP daemon.
 *
 * @param[out]  hSession    Session handle
 * @param[in]   ipAddr      IP address of the daemon
 * @param[in]   user        User name
 * @param[in]   password    Password
 *
 * @return Status
 */
static MAIN_RET main_connect(long * const hSession, char const * const ipAddr, char const * const user, char const * const password)
{
    MAIN_RET        status      = MAIN_RET_OK;
    char const *    pUser       = "";
    char const *    pPassword   = "";
    int             vscphlpRet  = 0;

    if (NULL == hSession)
    {
        return MAIN_RET_ENULL;
    }

    *hSession = 0;

    /* No daemon address given? */
    if (NULL == ipAddr)
    {
        return MAIN_RET_ENULL;
    }

    if (NULL != user)
    {
        pUser = user;
    }

    if (NULL != password)
    {
        pPassword = password;
    }

    /* Create a session */
    *hSession = vscphlp_newSession();

    /* No session handle? */
    if (0 == *hSession)
    {
        LOG_ERROR("Couldn't get a session handle.");
        status = MAIN_RET_ERROR;
    }
    /* Set response timeout */
    else if (VSCP_ERROR_SUCCESS != vscphlp_setResponseTimeout(*hSession, MAIN_RESPONSE_TIMEOUT))
    {
        LOG_ERROR("Couldn't set response timeout.");
        status = MAIN_RET_ERROR;
    }
    /* Set wait time after command execution */
    else if (VSCP_ERROR_SUCCESS != vscphlp_setAfterCommandSleep(*hSession, MAIN_CMD_WAIT_TIME))
    {
        LOG_ERROR("Couldn't set wait time after command.");
        status = MAIN_RET_ERROR;
    }
    /* Open a channel */
    else if (VSCP_ERROR_SUCCESS != (vscphlpRet = vscphlp_open(*hSession, ipAddr, pUser, pPassword)))
    {
        if (VSCP_ERROR_INVALID_HANDLE == vscphlpRet)
        {
            LOG_ERROR("Invalid handle.");
            status = MAIN_RET_ERROR;
        }
        else if (VSCP_ERROR_USER == vscphlpRet)
        {
            LOG_ERROR("Invalid user.");
            status = MAIN_RET_INVALID_USER;
        }
        else if (VSCP_ERROR_PASSWORD == vscphlpRet)
        {
            LOG_ERROR("Invalid password");
            status = MAIN_RET_INVALID_PASSWORD;
        }
        else if (VSCP_ERROR_TIMEOUT == vscphlpRet)
        {
            LOG_ERROR("Connection timeout.");
            status = MAIN_RET_TIMEOUT;
        }
        else
        {
            LOG_ERROR_INT32("Failed to open a channel:", vscphlpRet);
            status = MAIN_RET_ERROR;
        }
    }
    /* Check channel */
    else if (VSCP_ERROR_SUCCESS != (vscphlpRet = vscphlp_noop(*hSession)))
    {
        LOG_ERROR_INT32("Channel communication error:", vscphlpRet);
        status = MAIN_RET_ERROR;
    }
    /* Clear rx event queue and go sure that no L2 event is in the queue right now. */
    else if (VSCP_ERROR_SUCCESS != (vscphlpRet = vscphlp_clearDaemonEventQueue(*hSession)))
    {
        LOG_ERROR_INT32("Couldn't clear rx event queue:", vscphlpRet);
        status = MAIN_RET_ERROR;
    }
    else
    /* Successful connected */
    {
        unsigned char   major       = 0;
        unsigned char   minor       = 0;
        unsigned char   subMinor    = 0;
        unsigned long   dllVersion  = 0;
        char            vendorStr[120];

        LOG_INFO("Connected.");

        /* Get version of remote VSCP daemon */
        if (VSCP_ERROR_SUCCESS != (vscphlpRet = vscphlp_getVersion(*hSession, &major, &minor, &subMinor)))
        {
            LOG_WARNING_INT32("vscphlp_getVersion failed:", vscphlpRet);
        }
        else
        {
            printf("Remote VSCP daemon v%u.%u.%u\n", major, minor, subMinor);
        }

        /* Get dll version */
        if (VSCP_ERROR_SUCCESS != (vscphlpRet = vscphlp_getDLLVersion(*hSession, &dllVersion)))
        {
            LOG_WARNING_INT32("vscphlp_getDLLVersion failed:", vscphlpRet);
        }
        else
        {
            printf("Used VSCP dll version 0x%08lX\n", dllVersion);
        }

        /* Get vendor from driver */
        if (VSCP_ERROR_SUCCESS != (vscphlpRet = vscphlp_getVendorString(*hSession, vendorStr, sizeof(vendorStr))))
        {
            LOG_WARNING_INT32("vscphlp_getVendorString failed:", vscphlpRet);
        }
        else
        {
            printf("Vendor of driver: %s\n", vendorStr);
        }
    }

    /* Any error happened? */
    if (MAIN_RET_OK != status)
    {
        /* If a session is opened, close it. */
        if (0 != *hSession)
        {
            (void)vscphlp_close(*hSession);
            vscphlp_closeSession(*hSession);
            *hSession = 0;
        }
    }

    return status;
}

/**
 * This function disconnects a connection to a VSCP daemon.
 *
 * @param[in,out]   hSession    Session handle
 */
static void main_disconnect(long * const hSession)
{
    if (NULL == hSession)
    {
        return;
    }

    /* If a session is opened, close it. */
    if (0 != *hSession)
    {
        (void)vscphlp_close(*hSession);
        vscphlp_closeSession(*hSession);
        *hSession = 0;

        LOG_INFO("Disconnected.");
    }

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
 * Calculates the CRC16-CCITT for all intel hex records.
 *
 * @param[in]   recSet      Intel hex record set
 * @param[in]   recNum      Number of intel hex records
 * @param[in]   blockSize   Block size in bytes
 * @param[in]   fillBlock   Shall a block be filled up (TRUE) or not (FALSE)
 *
 * @return CRC16-CCITT
 */
static Crc16CCITT main_calculateCrc(intelHexParser_Record* recSet, uint32_t recNum, uint32_t blockSize, BOOL fillBlock)
{
    uint32_t    recIndex    = 0;
    uint32_t    dataIndex   = 0;
    uint32_t    blockIndex  = 0;
    uint8_t     fillByte    = MAIN_BLOCK_FILL_BYTE;
    Crc16CCITT  crc         = crc16ccitt_init();
    
    if (NULL == recSet)
    {
        return crc;
    }
    
    for(recIndex = 0; recIndex < recNum; ++recIndex)
    {
        if (INTELHEXPARSER_REC_TYPE_DATA == recSet[recIndex].type)
        {
            for(dataIndex = 0; dataIndex < recSet[recIndex].dataSize; ++dataIndex)
            {    
                crc = crc16ccitt_update(crc, &recSet[recIndex].data[dataIndex], 1);
                
                ++blockIndex;
                blockIndex %= blockSize;
            }
        }
        else
        {
            break;
        }
    }

    if (TRUE == fillBlock)
    {
        while(0 < (blockIndex % blockSize))
        {
            crc = crc16ccitt_update(crc, &fillByte, 1);
            
            ++blockIndex;
        }
    }
    
    crc = crc16ccitt_finalize(crc);
        
    return crc;
}

/**
 * Calculates the complete data size in bytes for the intel hex records.
 *
 * @param[in]   recSet      Intel hex record set
 * @param[in]   recNum      Number of intel hex records
 *
 * @return Size in bytes
 */
static uint32_t main_calculateDataSize(intelHexParser_Record* recSet, uint32_t recNum)
{
    uint32_t    recIndex    = 0;
    uint32_t    cnt         = 0;
    
    if (NULL == recSet)
    {
        return cnt;
    }
    
    for(recIndex = 0; recIndex < recNum; ++recIndex)
    {
        if (INTELHEXPARSER_REC_TYPE_DATA == recSet[recIndex].type)
        {
            cnt += recSet[recIndex].dataSize;
        }
        else
        {
            break;
        }
    }
        
    return cnt;
}

/**
 * This function proceeds the programming of a single node.
 *
 * @param[in,out]   progCon     Programming context
 * @param[in]       hSession    VSCP daemon connection session
 * @param[in]       recSet      Intel hex record set
 * @param[in]       recNum      Number of intel hex records
 * @param[in]       rxEvent     Received VSCP event (may be NULL)
 */
static void main_programNode(main_Programming * const progCon, long hSession, intelHexParser_Record* recSet, uint32_t recNum, vscpEventEx const * const rxEvent)
{
    uint32_t    index       = 0;
    vscpEventEx txEvent;
    Crc16CCITT  imageCrc    = 0;

    if ((NULL == progCon) ||
        (NULL == recSet))
    {
        return;
    }

    /* Clear tx event */
    memset(&txEvent, 0, sizeof(txEvent));

    /* Process programming state machine */
    switch(progCon->state)
    {
    case MAIN_PRG_STATE_READ_REG_PAGE_SELECT:
        log_printf("Read register page select.\n");
        
        txEvent.vscp_class  = VSCP_CLASS1_PROTOCOL;
        txEvent.vscp_type   = VSCP_TYPE_PROTOCOL_EXTENDED_PAGE_READ;
        txEvent.head        = VSCP_PRIORITY_0 << 5;
        txEvent.sizeData    = 5;
        txEvent.data[0]     = progCon->nodeId;
        txEvent.data[1]     = 0;    /* MSB page 0 */
        txEvent.data[2]     = 0;    /* LSB page 0 */
        txEvent.data[3]     = 0x92; /* Page select register */
        txEvent.data[4]     = 2;    /* Page select is a 16-bit value */

        if (VSCP_ERROR_SUCCESS != vscphlp_sendEventEx(hSession, &txEvent))
        {
            log_printf("Error!\n");
            
            progCon->state = MAIN_PRG_STATE_ERROR;
        }
        else
        {
            log_printf("Wait for response.\n");

            progCon->state = MAIN_PRG_STATE_READ_REG_PAGE_SELECT_RSP;
        }
        
        break;
        
    case MAIN_PRG_STATE_READ_REG_PAGE_SELECT_RSP:
        if ((NULL != rxEvent) &&
            (VSCP_CLASS1_PROTOCOL == rxEvent->vscp_class))
        {
            if ((VSCP_TYPE_PROTOCOL_EXTENDED_PAGE_RESPONSE == rxEvent->vscp_type) &&
                (6 == rxEvent->sizeData))
            {
                log_printf("Response received.\n");

                /* Reconstruct selected page */
                progCon->pageSelect = rxEvent->data[4];
                progCon->pageSelect <<= 8;
                progCon->pageSelect |= rxEvent->data[5];

                progCon->state = MAIN_PRG_STATE_ENTER_BOOT_LOADER_MODE;
            }
        }
        break;
        
    case MAIN_PRG_STATE_ENTER_BOOT_LOADER_MODE:

        log_printf("Enter boot loader mode.\n");

        txEvent.vscp_class  = VSCP_CLASS1_PROTOCOL;
        txEvent.vscp_type   = VSCP_TYPE_PROTOCOL_ENTER_BOOT_LOADER;
        txEvent.head        = VSCP_PRIORITY_0 << 5;
        txEvent.sizeData    = 8;
        txEvent.data[0]     = progCon->nodeId;
        txEvent.data[1]     = progCon->bootLoaderAlgo;
        txEvent.data[2]     = progCon->nodeGuid[0];
        txEvent.data[3]     = progCon->nodeGuid[3];
        txEvent.data[4]     = progCon->nodeGuid[5];
        txEvent.data[5]     = progCon->nodeGuid[7];
        txEvent.data[6]     = (progCon->pageSelect >> 8) & 0xff;    /* Register 0x92, page select msb */
        txEvent.data[7]     = (progCon->pageSelect >> 0) & 0xff;    /* Register 0x93, page select lsb */

        if (VSCP_ERROR_SUCCESS != vscphlp_sendEventEx(hSession, &txEvent))
        {
            log_printf("Error!\n");
            
            progCon->state = MAIN_PRG_STATE_ERROR;
        }
        else
        {
            log_printf("Wait for acknowledge.\n");

            progCon->state = MAIN_PRG_STATE_ENTER_BOOT_LOADER_MODE_ACK;
        }
        break;

    case MAIN_PRG_STATE_ENTER_BOOT_LOADER_MODE_ACK:
        if ((NULL != rxEvent) &&
            (VSCP_CLASS1_PROTOCOL == rxEvent->vscp_class))
        {
            if ((VSCP_TYPE_PROTOCOL_ACK_BOOT_LOADER == rxEvent->vscp_type) &&
                (8 == rxEvent->sizeData))
            {
                uint32_t    dataSize    = 0;
                uint32_t    blockNum    = 0;
                
                log_printf("Node entered boot loader mode.\n");

                /* Reconstruct block size */
                progCon->blockSize = MAIN_BUILD_UINT32( rxEvent->data[0],
                                                        rxEvent->data[1],
                                                        rxEvent->data[2],
                                                        rxEvent->data[3]);

                /* Reconstruct number of blocks */
                progCon->blockNum = MAIN_BUILD_UINT32(  rxEvent->data[4],
                                                        rxEvent->data[5],
                                                        rxEvent->data[6],
                                                        rxEvent->data[7]);

                log_printf("Block size: %u bytes\n", progCon->blockSize);
                log_printf("Max. number of blocks: %u\n", progCon->blockNum);

                /* Calculate number of needed blocks to transfer */
                dataSize = main_calculateDataSize(recSet, recNum);
                blockNum = dataSize / progCon->blockSize;
                if (0 < (dataSize % progCon->blockSize))
                {
                    ++blockNum;
                }
                
                log_printf("Blocks to transfer: %u\n", blockNum);
                
                /* More block to transfer, than max. possible? */
                if (progCon->blockNum < blockNum)
                {
                    log_printf("More blocks to transfer, than max. possible!\n");
                    
                    progCon->state = MAIN_PRG_STATE_ERROR;
                }
                else
                {
                    progCon->state = MAIN_PRG_STATE_START_BLOCK_TRANSFER;
                }
            }
            else if ((VSCP_TYPE_PROTOCOL_NACK_BOOT_LOADER == rxEvent->vscp_type) &&
                     (1 == rxEvent->sizeData))
            {
                log_printf("Node failed to enter boot loader mode.\n");

                if (1 == rxEvent->sizeData)
                {
                    log_printf("Error code: %u\n", rxEvent->data[0]);
                }

                progCon->state = MAIN_PRG_STATE_ERROR;
            }
        }
        break;

    case MAIN_PRG_STATE_START_BLOCK_TRANSFER:

        log_printf("Start block data transfer.\n");

        txEvent.vscp_class  = VSCP_CLASS1_PROTOCOL;
        txEvent.vscp_type   = VSCP_TYPE_PROTOCOL_START_BLOCK;
        txEvent.head        = VSCP_PRIORITY_0 << 5;
        txEvent.sizeData    = 6;
        txEvent.data[0]     = (progCon->blockIndex >> 24) & 0xff;
        txEvent.data[1]     = (progCon->blockIndex >> 16) & 0xff;
        txEvent.data[2]     = (progCon->blockIndex >>  8) & 0xff;
        txEvent.data[3]     = (progCon->blockIndex >>  0) & 0xff;
        txEvent.data[4]     = 0;    /* Type of memory to write: Program flash */
        txEvent.data[5]     = 0;    /* Bank/Image to write: Internal flash */

        if (VSCP_ERROR_SUCCESS != vscphlp_sendEventEx(hSession, &txEvent))
        {
            log_printf("Error!\n");
            
            progCon->state = MAIN_PRG_STATE_ERROR;
        }
        else
        {
            log_printf("Wait for acknowledge.\n");

            progCon->state = MAIN_PRG_STATE_START_BLOCK_TRANSFER_ACK;
        }
        break;

    case MAIN_PRG_STATE_START_BLOCK_TRANSFER_ACK:
        if ((NULL != rxEvent) &&
            (VSCP_CLASS1_PROTOCOL == rxEvent->vscp_class))
        {
            if ((VSCP_TYPE_PROTOCOL_START_BLOCK_ACK == rxEvent->vscp_type) &&
                (0 == rxEvent->sizeData))
            {
                log_printf("Start block transfer acknowleded.\n");

                /* Reset block CRC */
                progCon->blockCrcCalculated = crc16ccitt_init();

                /* Initialize block fragment index */
                progCon->blockFragmentIndex  = 0;

                /* Remember some stuff, to be able to retry the block transfer later. */
                progCon->recIndexBackup     = progCon->recIndex;
                progCon->recDataIndexBackup = progCon->recDataIndex;

                progCon->state = MAIN_PRG_STATE_BLOCK_DATA;
            }
            else if ((VSCP_TYPE_PROTOCOL_START_BLOCK_NACK == rxEvent->vscp_type) &&
                     (0 == rxEvent->sizeData))
            {
                log_printf("Failed to start block transfer.\n");

                progCon->state = MAIN_PRG_STATE_ERROR;
            }
        }
        break;

    case MAIN_PRG_STATE_BLOCK_DATA:
        log_printf("Block %u data transfer %u\n", progCon->blockIndex, progCon->blockFragmentIndex);

        txEvent.vscp_class  = VSCP_CLASS1_PROTOCOL;
        txEvent.vscp_type   = VSCP_TYPE_PROTOCOL_BLOCK_DATA;
        txEvent.head        = VSCP_PRIORITY_0 << 5;
        txEvent.sizeData    = 0;

        for(index = 0; index < 8; ++index)
        {
            /* Records available? */
            if (INTELHEXPARSER_REC_TYPE_DATA == recSet[progCon->recIndex].type)
            {
                txEvent.data[index] = recSet[progCon->recIndex].data[progCon->recDataIndex];

                progCon->blockCrcCalculated = crc16ccitt_update(progCon->blockCrcCalculated, &txEvent.data[index], 1);

                ++progCon->recDataIndex;
                ++txEvent.sizeData;

                /* Next record? */
                if (recSet[progCon->recIndex].dataSize <= progCon->recDataIndex)
                {
                    ++progCon->recIndex;                    
                    progCon->recDataIndex = 0;
                }
            }
            /* Records are empty, shall the block be filled up? */
            else if (TRUE == progCon->fillBlock)
            {
                txEvent.data[index] = MAIN_BLOCK_FILL_BYTE;

                progCon->blockCrcCalculated = crc16ccitt_update(progCon->blockCrcCalculated, &txEvent.data[index], 1);
                
                ++txEvent.sizeData;
            }
            /* Error */
            else
            {
                log_printf("Less number of intel hex records (%u).\n", recNum);

                progCon->state = MAIN_PRG_STATE_ERROR;
                break;
            }
        }

        if (MAIN_PRG_STATE_ERROR != progCon->state)
        {
            if (VSCP_ERROR_SUCCESS != vscphlp_sendEventEx(hSession, &txEvent))
            {
                log_printf("Error!\n");
                
                progCon->state = MAIN_PRG_STATE_ERROR;
            }
            else
            {
                ++progCon->blockFragmentIndex;

                /* Block finished? */
                if (progCon->blockSize <= (progCon->blockFragmentIndex * 8))
                {
                    log_printf("Wait for acknowledge.\n");

                    progCon->state = MAIN_PRG_STATE_BLOCK_DATA_ACK;
                }
            }
        }
        break;

    case MAIN_PRG_STATE_BLOCK_DATA_ACK:
        if ((NULL != rxEvent) &&
            (VSCP_CLASS1_PROTOCOL == rxEvent->vscp_class))
        {
            if ((VSCP_TYPE_PROTOCOL_BLOCK_DATA_ACK == rxEvent->vscp_type) &&
                (6 == rxEvent->sizeData))
            {
                Crc16CCITT  receivedCrc = 0;

                /* Reconstruct received block CRC */
                receivedCrc = rxEvent->data[0];
                receivedCrc <<= 8;
                receivedCrc |= rxEvent->data[1];

                /* Finalize block CRC */
                progCon->blockCrcCalculated = crc16ccitt_finalize(progCon->blockCrcCalculated);

                /* Invalid CRC received? */
                if (receivedCrc != progCon->blockCrcCalculated)
                {
                    log_printf("Block %u failed to transfer (invalid CRC 0x%04X, expected 0x%04X).\n", progCon->blockIndex, receivedCrc, progCon->blockCrcCalculated);

                    ++progCon->blockRetry;
                    progCon->state = MAIN_PRG_STATE_START_BLOCK_TRANSFER;
                }
                else
                {
                    log_printf("Block %u successful transfered.\n", progCon->blockIndex);

                    progCon->state = MAIN_PRG_STATE_PROGRAM_BLOCK;
                }
            }
            else if ((VSCP_TYPE_PROTOCOL_BLOCK_DATA_NACK == rxEvent->vscp_type) &&
                     (5 == rxEvent->sizeData))
            {
                log_printf("Block transfer failed.\n");
                log_printf("Error code: %u\n", rxEvent->data[0]);

                ++progCon->blockRetry;
                progCon->state = MAIN_PRG_STATE_START_BLOCK_TRANSFER;
            }

            /* Transfer block again? */
            if (MAIN_PRG_STATE_START_BLOCK_TRANSFER == progCon->state)
            {
                /* Maximum number of retries reached? */
                if (MAIN_MAX_BLOCK_TRANSFER_RETRIES <= progCon->blockRetry)
                {
                    progCon->state = MAIN_PRG_STATE_ERROR;
                }
                else
                {
                    log_printf("Transfer block %u again (%u).", progCon->blockIndex, progCon->blockRetry);

                    /* Restore the position in the intel hex records */
                    progCon->recIndex        = progCon->recIndexBackup;
                    progCon->recDataIndex    = progCon->recDataIndexBackup;
                }
            }
            else
            {
                progCon->blockRetry = 0;
            }
        }
        break;

    case MAIN_PRG_STATE_PROGRAM_BLOCK:

        log_printf("Program block.\n");

        txEvent.vscp_class  = VSCP_CLASS1_PROTOCOL;
        txEvent.vscp_type   = VSCP_TYPE_PROTOCOL_PROGRAM_BLOCK_DATA;
        txEvent.head        = VSCP_PRIORITY_0 << 5;
        txEvent.sizeData    = 4;
        txEvent.data[0]     = (progCon->blockIndex >> 24) & 0xff;
        txEvent.data[1]     = (progCon->blockIndex >> 16) & 0xff;
        txEvent.data[2]     = (progCon->blockIndex >>  8) & 0xff;
        txEvent.data[3]     = (progCon->blockIndex >>  0) & 0xff;

        if (VSCP_ERROR_SUCCESS != vscphlp_sendEventEx(hSession, &txEvent))
        {
            log_printf("Error!\n");
            
            progCon->state = MAIN_PRG_STATE_ERROR;
        }
        else
        {
            log_printf("Wait for acknowledge.\n");

            progCon->state = MAIN_PRG_STATE_PROGRAM_BLOCK_ACK;
        }
        break;

    case MAIN_PRG_STATE_PROGRAM_BLOCK_ACK:
        if ((NULL != rxEvent) &&
            (VSCP_CLASS1_PROTOCOL == rxEvent->vscp_class))
        {
            if ((VSCP_TYPE_PROTOCOL_PROGRAM_BLOCK_DATA_ACK == rxEvent->vscp_type) &&
                (4 == rxEvent->sizeData))
            {
                log_printf("Block %u successful programmed.\n", progCon->blockIndex);

                /* Next block */
                ++progCon->blockIndex;

                /* All records transfered? */
                if (INTELHEXPARSER_REC_TYPE_DATA != recSet[progCon->recIndex].type)
                {
                    progCon->state = MAIN_PRG_STATE_ACTIVATE_NEW_IMAGE;
                }
                /* Max. number of blocks transfered? */
                else if (progCon->blockNum <= progCon->blockIndex)
                {
                    progCon->state = MAIN_PRG_STATE_ACTIVATE_NEW_IMAGE;
                }
                else
                {
                    progCon->state = MAIN_PRG_STATE_START_BLOCK_TRANSFER;
                }
            }
            else if ((VSCP_TYPE_PROTOCOL_PROGRAM_BLOCK_DATA_NACK == rxEvent->vscp_type) &&
                     (5 == rxEvent->sizeData))
            {
                log_printf("Block %u failed to program.\n", progCon->blockIndex);
                log_printf("Error code: %u\n", rxEvent->data[0]);

                progCon->state = MAIN_PRG_STATE_ERROR;
            }
        }
        break;

    case MAIN_PRG_STATE_ACTIVATE_NEW_IMAGE:

        log_printf("Activate new image.\n");

        /* Calculate image for the whole image */
        imageCrc = main_calculateCrc(recSet, recNum, progCon->blockSize, progCon->fillBlock);

        txEvent.vscp_class  = VSCP_CLASS1_PROTOCOL;
        txEvent.vscp_type   = VSCP_TYPE_PROTOCOL_ACTIVATE_NEW_IMAGE;
        txEvent.head        = VSCP_PRIORITY_0 << 5;
        txEvent.sizeData    = 2;
        txEvent.data[0]     = (imageCrc >> 8) & 0xff;  /* CRC MSB */
        txEvent.data[1]     = (imageCrc >> 0) & 0xff;  /* CRC MSB */

        if (VSCP_ERROR_SUCCESS != vscphlp_sendEventEx(hSession, &txEvent))
        {
            log_printf("Error!\n");
            
            progCon->state = MAIN_PRG_STATE_ERROR;
        }
        else
        {
            log_printf("Wait for acknowledge.\n");

            progCon->state = MAIN_PRG_STATE_ACTIVATE_NEW_IMAGE_ACK;
        }
        break;

    case MAIN_PRG_STATE_ACTIVATE_NEW_IMAGE_ACK:
        if ((NULL != rxEvent) &&
            (VSCP_CLASS1_PROTOCOL == rxEvent->vscp_class))
        {
            if (VSCP_TYPE_PROTOCOL_ACTIVATE_NEW_IMAGE_ACK == rxEvent->vscp_type)
            {
                log_printf("New image activated.\n");
                
                progCon->state = MAIN_PRG_STATE_IDLE;
            }
            else if (VSCP_TYPE_PROTOCOL_ACTIVATE_NEW_IMAGE_NACK == rxEvent->vscp_type)
            {
                log_printf("Failed to activate new image. CRC may be wrong.\n");

                progCon->state = MAIN_PRG_STATE_ERROR;
            }
        }
        break;

    case MAIN_PRG_STATE_ERROR:
        log_printf("Programming aborted, caused by an error.\n");

        txEvent.vscp_class  = VSCP_CLASS1_PROTOCOL;
        txEvent.vscp_type   = VSCP_TYPE_PROTOCOL_DROP_NICKNAME;
        txEvent.head        = VSCP_PRIORITY_0 << 5;
        txEvent.sizeData    = 1;
        txEvent.data[0]     = progCon->nodeId;

        (void)vscphlp_sendEventEx(hSession, &txEvent);
        
        progCon->state = MAIN_PRG_STATE_IDLE;
        break;

    case MAIN_PRG_STATE_IDLE:
        break;

    default:
        break;
    }

    return;
}