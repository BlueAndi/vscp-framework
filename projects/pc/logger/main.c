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
#include <string.h>
#include "log.h"
#include "platform.h"
#include "vscphelperlib.h"
#include "cmdLineParser.h"

/*******************************************************************************
    COMPILER SWITCHES
*******************************************************************************/

/*******************************************************************************
    CONSTANTS
*******************************************************************************/

/** Program name */
#define MAIN_PROG_NAME          "VSCP logger"

/** Copyright */
#define MAIN_COPYRIGHT          "(c) 2014 - 2021 Andreas Merkle"

/** Default log level */
#define MAIN_LOG_LEVEL_DEFAULT  (LOG_LEVEL_FATAL)

/** Daemon command response timeout in ms */
#define MAIN_RESPONSE_TIMEOUT   6000

/** Daemon wait time after a command was executed in ms */
#define MAIN_CMD_WAIT_TIME      250

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
    BOOL            showHelp;       /**< Show help to the user */
    BOOL            verbose;        /**< Verbose output */

} main_CmdLineArgs;

/** This type defines a log message. */
typedef struct _main_Msg
{
    uint8_t             id;     /**< ID */
    char const *        text;   /**< Message text */
    struct _main_Msg    *next;  /**< Next message */

} main_Msg;

/*******************************************************************************
    PROTOTYPES
*******************************************************************************/

static MAIN_RET main_init(void);
static void main_deInit(void);
static void main_showKeyTable(void);
static void main_loop(long hSession);
static MAIN_RET main_connect(long * const hSession, char const * const ipAddr, char const * const user, char const * const password);
static void main_disconnect(long * const hSession);
static CMDLINEPARSER_RET main_clpUnknown(void* const userData, char const * const arg, char const * const par);

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
    FALSE,  /* Show help */
    FALSE   /* Verbose output */
};

/** Configuration for the command line parser. */
static const cmdLineParser_Arg  main_clpConfig[]    =
{
    /* Special to retrieve the program name without path */
    { CMDLINEPARSER_PROG_NAME_WP,   &main_cmdLineArgs.progName,         NULL,                       NULL,               NULL,   NULL                                },
    /* Get every unknown command line argument */
    { CMDLINEPARSER_UNKONWN,        NULL,                               NULL,                       main_clpUnknown,    NULL,   NULL                                },
    /* Possible command line arguments */
    { "-h --help",                  NULL,                               &main_cmdLineArgs.showHelp, NULL,               NULL,   "Show help"                         },
    { "-v",                         NULL,                               &main_cmdLineArgs.verbose,  NULL,               NULL,   "Increase verbose level"            },
    { "-a <ip-address>",            &main_cmdLineArgs.daemonAddr,       NULL,                       NULL,               NULL,   "IP address of VSCP daemon"         },
    { "-u <user>",                  &main_cmdLineArgs.daemonUser,       NULL,                       NULL,               NULL,   "User name for VSCP daemon access"  },
    { "-p <password>",              &main_cmdLineArgs.daemonPassword,   NULL,                       NULL,               NULL,   "Password for VSCP daemon access"   }
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
    int         status      = 0;
    BOOL        abort       = FALSE;
    long        hSession    = 0;
    MAIN_RET    ret         = MAIN_RET_OK;

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
    else
    {
        if (TRUE == main_cmdLineArgs.verbose)
        {
            /* Set log level */
            log_setLevel(LOG_LEVEL_INFO | LOG_LEVEL_DEBUG | LOG_LEVEL_WARNING | LOG_LEVEL_ERROR | LOG_LEVEL_FATAL);
        }
    
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
            
            /* If no error happened, start main loop. */
            if (FALSE == abort)
            {
                main_loop(hSession);
            }

            printf("Please wait ...\n");

            main_disconnect(&hSession);
            
            printf("Disconnected\n");
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
 * This function contains the main loop.
 *
 * @param[in] hSession Daemon connection session
 */
static void main_loop(long hSession)
{
    int     keyValue    = 0;
    BOOL    quit        = FALSE;

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
            uint32_t    count       = 0;
            vscpEventEx daemonEvent;
            int         vscphlpRet  = 0;

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
                else if (((VSCP_CLASS1_LOG == daemonEvent.vscp_class) || ((VSCP_CLASS1_LOG + 512) == daemonEvent.vscp_class)) &&
                         (VSCP_TYPE_LOG_MESSAGE == daemonEvent.vscp_type) &&
                         (8 == daemonEvent.sizeData))
                {
                    uint32_t index = 0;

                    log_printf("Rx: %s class=0x%04X type=0x%02X prio=%2d oAddr=0x%02X %c num=%u id=0x%02X level=0x%02X idx=0x%02X data=",
                        (VSCP_CLASS1_LOG == daemonEvent.vscp_class) ? "L1   " : "L1_L2",
                        daemonEvent.vscp_class,
                        daemonEvent.vscp_type,
                        (daemonEvent.head >> 5) & 0x07,
                        daemonEvent.GUID[15],
                        (0 == ((daemonEvent.head >> 4) & 0x01)) ? '-' : 'h',
                        daemonEvent.sizeData,
                        daemonEvent.data[0],
                        daemonEvent.data[1],
                        daemonEvent.data[2]);

                    for(index = 3; index < daemonEvent.sizeData; ++index)
                    {
                        printf("%02X", daemonEvent.data[index]);

                        if ((index + 1) < daemonEvent.sizeData)
                        {
                            printf(" ");
                        }
                    }

                    printf("\n");
                }
                else if (512 > daemonEvent.vscp_class)
                {
                    uint32_t index = 0;

                    log_printf("Rx: L1    class=0x%04X type=0x%02X prio=%2d oAddr=0x%02X %c num=%u data=",
                        daemonEvent.vscp_class,
                        daemonEvent.vscp_type,
                        (daemonEvent.head >> 5) & 0x07,
                        daemonEvent.GUID[15], /* Node GUID LSB */
                        (0 == ((daemonEvent.head >> 4) & 0x01)) ? '-' : 'h',
                        daemonEvent.sizeData);

                    for(index = 0; index < daemonEvent.sizeData; ++index)
                    {
                        printf("%02X", daemonEvent.data[index]);

                        if ((index + 1) < daemonEvent.sizeData)
                        {
                            printf(" ");
                        }
                    }

                    printf("\n");
                }
                else if (1024 > daemonEvent.vscp_class)
                {
                    uint32_t index = 0;

                    log_printf("Rx: L2_L1 class=0x%04X type=0x%02X prio=%2d oAddr=0x%02X %c num=%u data=",
                        daemonEvent.vscp_class,
                        daemonEvent.vscp_type,
                        (daemonEvent.head >> 5) & 0x07,
                        daemonEvent.GUID[15], /* Node GUID LSB */
                        (0 == ((daemonEvent.head >> 4) & 0x01)) ? '-' : 'h',
                        daemonEvent.sizeData);

                    for(index = 0; index < daemonEvent.sizeData; ++index)
                    {
                        printf("%02X", daemonEvent.data[index]);

                        if ((index + 1) < daemonEvent.sizeData)
                        {
                            printf(" ");
                        }
                    }

                    printf("\n");
                }
                else
                {
                    uint32_t index  = 0;
                    uint8_t  max    = 0;

                    log_printf("Rx: L2    class=0x%02X type=0x%02X prio=%2d oAddr=0x%02X %c num=%u data=",
                        daemonEvent.vscp_class,
                        daemonEvent.vscp_type,
                        (daemonEvent.head >> 5) & 0x07,
                        daemonEvent.GUID[15], /* Node GUID LSB */
                        (0 == ((daemonEvent.head >> 4) & 0x01)) ? '-' : 'h',
                        daemonEvent.sizeData);

                    if (8 < daemonEvent.sizeData)
                    {
                        max = 8;
                    }
                    else
                    {
                        max = daemonEvent.sizeData;
                    }

                    for(index = 0; index < max; ++index)
                    {
                        printf("%02X", daemonEvent.data[index]);

                        if ((index + 1) < max)
                        {
                            printf(" ");
                        }
                    }

                    if (8 < daemonEvent.sizeData)
                    {
                        printf("...");
                    }

                    printf("\n");
                }
            }

            /* Give other programs a chance. */
            platform_delay(1);
        }
    }

    platform_echoOn();
    
    return;
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
