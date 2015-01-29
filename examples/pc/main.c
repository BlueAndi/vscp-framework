/* The MIT License (MIT)
 * 
 * Copyright (c) 2014 - 2015, Andreas Merkle
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

@section svn Subversion
$Author: amerkle $
$Rev: 449 $
$Date: 2015-01-05 20:23:52 +0100 (Mo, 05 Jan 2015) $
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
#include "vscp_core.h"
#include "vscp_timer.h"
#include "vscp_ps_access.h"
#include "vscp_tp_adapter.h"
#include "vscp_class_l1.h"
#include "vscp_type_information.h"
#include "vscp_ps.h"

/*******************************************************************************
    COMPILER SWITCHES
*******************************************************************************/

/*******************************************************************************
    CONSTANTS
*******************************************************************************/

/** Persistent memory filename */
#define MAIN_EEPROM_FILENAME    "eeprom.asc"

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
    char const *        progName;       /**< Program name without path */
    char const *        daemonAddr;     /**< Daemon ip address */
    char const *        daemonUser;     /**< User name for daemon ip access */
    char const *        daemonPassword; /**< Password for daemon ip access */
    BOOL                verbose;        /**< Verbose information */
    BOOL                showHelp;       /**< Show help to the user */
    VSCP_TP_ADAPTER_LVL lvl;            /**< Network level */
    
} main_CmdLineArgs;

/** This type contains the necessary thread data. */
typedef struct
{
    pthread_t       id;             /**< Thread id */
    int             status;         /**< Thread creation status */
    pthread_mutex_t mutex;          /**< Mutex to protect the thread data */
    BOOL            quitFlag;       /**< Flag to signal the thread to quit */

} main_ThreadData;

/*******************************************************************************
    PROTOTYPES
*******************************************************************************/

static MAIN_RET main_init(void);
static void main_deInit(void);
static MAIN_RET main_setupVscpThreads(void);
static MAIN_RET main_getCmdLineArgs(main_CmdLineArgs * const cmdLineArgs, int argc, char ** const argv);
static void main_showHelp(char const * const progName);
static void main_showKeyTable(void);
static void* main_vscpFrameworkThread(void* par);
static void* main_vscpTimerThread(void* par);
static void main_dumpEEPROM(void);
static void main_loop(void);

/*******************************************************************************
    LOCAL VARIABLES
*******************************************************************************/

/** Mutex used to protect VSCP framework thread, VSCP timer thread and main thread. */
static pthread_mutex_t  main_vscpMutex              = PTHREAD_MUTEX_INITIALIZER;

/** VSCP framework thread data */
static main_ThreadData  main_vscpFrameworkThreadData;

/** VSCP framework timer thread data */
static main_ThreadData  main_vscpTimerThreadData;

/** User friendly names for persistent memory elements */
static const char*      main_psUserFriendlyName[]   =
{
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
    int                 status          = 0;
    BOOL                abort           = FALSE;
    main_CmdLineArgs    cmdLineArgs;
    
    printf("\nVirtual VSCP level 1 node\n");
    printf("Version: %s\n\n", VERSION);

    /* Set log level */
    log_setLevel(LOG_LEVEL_FATAL);
    
    /* Parse command line arguments */
    if (MAIN_RET_OK != main_getCmdLineArgs(&cmdLineArgs, argc, argv))
    {
        abort = TRUE;
    }
    else
    {
        /* Increase log level for more detail? */
        if (TRUE == cmdLineArgs.verbose)
        {
            /* Set log level */
            log_setLevel(LOG_LEVEL_INFO | LOG_LEVEL_DEBUG | LOG_LEVEL_WARNING | LOG_LEVEL_ERROR | LOG_LEVEL_FATAL);
        }
    }
    
    /* Show help? */
    if (TRUE == cmdLineArgs.showHelp)
    {
        main_showHelp(cmdLineArgs.progName);
    }
    /* Abort because of a invalid program argument or continue? */
    else if (FALSE == status)
    {
        if (MAIN_RET_OK != main_init())
        {
            abort = TRUE;
        }
        else
        {
            /* Show the user which keys can be used. */
            main_showKeyTable();
            printf("\n");
        
            /* Shall a connection to a VSCP daemon be established? */
            if (NULL != cmdLineArgs.daemonAddr)
            {
                VSCP_TP_ADAPTER_RET ret = VSCP_TP_ADAPTER_RET_OK;
            
                ret = vscp_tp_adapter_connect(  cmdLineArgs.daemonAddr,
                                                cmdLineArgs.daemonUser,
                                                cmdLineArgs.daemonPassword,
                                                cmdLineArgs.lvl);
                
                if (VSCP_TP_ADAPTER_RET_OK != ret)
                {
                    printf("Failed to connect to %s.\n", cmdLineArgs.daemonAddr);
                    
                    if (VSCP_TP_ADAPTER_RET_INVALID_USER == ret)
                    {
                        printf("Invalid user.\n");
                    }
                    else if (VSCP_TP_ADAPTER_RET_INVALID_PASSWORD == ret)
                    {
                        printf("Invalid password.\n");
                    }
                    else if (VSCP_TP_ADAPTER_RET_TIMEOUT == ret)
                    {
                        printf("Connection timeout.\n");
                    }
                    
                    abort = TRUE;
                }
            }
            
            /* No error? */
            if (FALSE == abort)
            {
                /* Start VSCP threads */
                if (MAIN_RET_OK != main_setupVscpThreads())
                {
                    abort = TRUE;
                }
            }
            
            /* If no error happened, start main loop. */
            if (FALSE == abort)
            {
                main_loop();
            }
            
            printf("Please wait ...");
            
            /* Is VSCP framework thread running? */
            if (0 == main_vscpFrameworkThreadData.status)
            {
                (void)pthread_mutex_lock(&main_vscpFrameworkThreadData.mutex);
                main_vscpFrameworkThreadData.quitFlag = TRUE;
                (void)pthread_mutex_unlock(&main_vscpFrameworkThreadData.mutex);
                
                /* Wait for the VSCP framework thread until its finished. */
                printf(".");
                (void)pthread_join(main_vscpFrameworkThreadData.id, NULL);
            }
            
            /* Is VSCP timer thread running? */
            if (0 == main_vscpTimerThreadData.status)
            {
                (void)pthread_mutex_lock(&main_vscpTimerThreadData.mutex);
                main_vscpTimerThreadData.quitFlag = TRUE;
                (void)pthread_mutex_unlock(&main_vscpTimerThreadData.mutex);
                
                /* Wait for the VSCP timer thread until its finished. */
                printf(".");
                (void)pthread_join(main_vscpTimerThreadData.id, NULL);
            }
            
            printf("\n");
            
            /* Shall a connection to a VSCP daemon be disconnected? */
            if (NULL != cmdLineArgs.daemonAddr)
            {
                vscp_tp_adapter_disconnect();
            }
        }
        
        main_deInit();
    }
    
    (void)pthread_mutex_destroy(&main_vscpMutex);
    
    if (TRUE == abort)
    {
        printf("\nAborted.\n");
        
        if (0 == status)
        {
            status = 1;
        }
    }

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
    eeprom_load(MAIN_EEPROM_FILENAME);
    
    /* Initialize VSCP framework thread data */
    memset(&main_vscpFrameworkThreadData, 0, sizeof(main_vscpFrameworkThreadData));
    main_vscpFrameworkThreadData.status = 1; /* Set to error value */

    /* Initialize VSCP timer thread data */
    memset(&main_vscpTimerThreadData, 0, sizeof(main_vscpTimerThreadData));
    main_vscpTimerThreadData.status = 1; /* Set to error value */

    /* Initialize the VSCP framework */
    if (VSCP_CORE_RET_OK != vscp_core_init())
    {
        LOG_FATAL("VSCP init failed.");
        status = MAIN_RET_ERROR;
    }
    
    return status;
}

/**
 * This function de-initializes all relevant modules and release all resources.
 */
static void main_deInit(void)
{
    platform_deInit();
    
    eeprom_save(MAIN_EEPROM_FILENAME);
    eeprom_deInit();
        
    return;
}

/**
 * This function setup the VSCP framework and timer thread.
 *
 * @return Status
 */
static MAIN_RET main_setupVscpThreads(void)
{
    MAIN_RET    status  = MAIN_RET_OK;
        
    /* Initialize VSCP framework and timer thread data */
    main_vscpFrameworkThreadData.quitFlag   = FALSE;
    main_vscpFrameworkThreadData.mutex      = main_vscpMutex;
    
    main_vscpTimerThreadData.quitFlag       = FALSE;
    main_vscpTimerThreadData.mutex          = main_vscpMutex;
    
    /* Create vscp framework thread with default attributes */
    main_vscpFrameworkThreadData.status = pthread_create(&main_vscpFrameworkThreadData.id, NULL, main_vscpFrameworkThread, (void*)&main_vscpFrameworkThreadData);
    
    if (0 != main_vscpFrameworkThreadData.status)
    {
        LOG_FATAL_INT32("Couldn't create vscp framework thread:", main_vscpFrameworkThreadData.status);
        
        status = MAIN_RET_ERROR;
    }
    else
    {
        /* Create vscp timer thread with default attributes */
        main_vscpTimerThreadData.status = pthread_create(&main_vscpTimerThreadData.id, NULL, main_vscpTimerThread, (void*)&main_vscpTimerThreadData);
        
        if (0 != main_vscpTimerThreadData.status)
        {
            LOG_FATAL_INT32("Couldn't create vscp timer thread:", main_vscpTimerThreadData.status);
            
            (void)pthread_mutex_lock(&main_vscpMutex);               
            main_vscpFrameworkThreadData.quitFlag = TRUE;
            (void)pthread_mutex_unlock(&main_vscpMutex);
            
            (void)pthread_join(main_vscpFrameworkThreadData.id, NULL);
            
            status = MAIN_RET_ERROR;
        }
    }

    return status;
}

/**
 * This function parses the command line arguments and fills the appropriate structure.
 *
 * @param[out]  cmdLineArgs Command line arguments structure
 * @param[in]   argc        Number of arguments
 * @param[in]   argv        Array of argument strings
 * @return Status
 */
static MAIN_RET main_getCmdLineArgs(main_CmdLineArgs * const cmdLineArgs, int argc, char ** const argv)
{
    MAIN_RET    status  = MAIN_RET_OK;
    
    if ((NULL == cmdLineArgs) ||
        (NULL == argv))
    {
        return MAIN_RET_ENULL;
    }
    
    memset(cmdLineArgs, 0, sizeof(main_CmdLineArgs));
    
    /* Determine program file name */
    cmdLineArgs->progName = strrchr(argv[0], '/');
    
    if (NULL == cmdLineArgs->progName)
    {
        cmdLineArgs->progName = strrchr(argv[0], '\\');
    }
    
    if (NULL == cmdLineArgs->progName)
    {
        cmdLineArgs->progName = argv[0];
    }
    else
    {
        ++(cmdLineArgs->progName);
    }
    
    /* Set network level default value */
    cmdLineArgs->lvl = VSCP_TP_ADAPTER_LVL_1_OVER_2;
    
    /* Any further program argument available? */
    if (1 < argc)
    {
        uint32_t    index       = 0;
        BOOL        abort       = FALSE;
        
        for(index = 1; index < argc; ++index)
        {
            /* Show help? */
            if ((0 == strcmp(argv[index], "-h")) ||
                (0 == strcmp(argv[index], "--help")))
            {
                cmdLineArgs->showHelp = TRUE;
            }
            /* Increase verbose level? */
            else if (0 == strcmp(argv[index], "-v"))
            {
                cmdLineArgs->verbose = TRUE;
            }
            /* Support level 1 events? */
            else if (0 == strcmp(argv[index], "-l1"))
            {
                cmdLineArgs->lvl = VSCP_TP_ADAPTER_LVL_1;
            }
            /* Support level 1 and level 1 over level 2 events? */
            else if (0 == strcmp(argv[index], "-l12"))
            {
                cmdLineArgs->lvl = VSCP_TP_ADAPTER_LVL_1_OVER_2;
            }
            /* Daemon user name? */
            else if (0 == strncmp(argv[index], "-u", 2))
            {
                cmdLineArgs->daemonUser = &argv[index][2];
                
                if ('\0' == cmdLineArgs->daemonUser)
                {
                    printf("User name missing.\n");
                    abort = TRUE;
                }
            }
            /* Daemon password? */
            else if (0 == strncmp(argv[index], "-p", 2))
            {
                cmdLineArgs->daemonPassword = &argv[index][2];
                
                if ('\0' == cmdLineArgs->daemonPassword)
                {
                    printf("Password missing.\n");
                    abort = TRUE;
                }
            }
            /* Unknown option? */
            else if ('-' == argv[index][0])
            {
                printf("Unknown option: %s\n", argv[index]);
                abort = TRUE;
            }
            /* Daemon address? */
            else if (NULL == cmdLineArgs->daemonAddr)
            {
                cmdLineArgs->daemonAddr = argv[index];
            }
            else
            {
                printf("Unknown option: %s\n", argv[index]);
                abort = TRUE;
            }
            
            if (FALSE != abort)
            {
                break;
            }
        }
        
        if (FALSE != abort)
        {
            status = MAIN_RET_ERROR;
        }
    }
    
    return status;
}

/**
 * This function prints the help to the console.
 *
 * @param[in] progName  Program name
 */
static void main_showHelp(char const * const progName)
{
    printf("%s <options> [<daemon ip address>[:<port>]]\n\n", progName);
    printf("General options:\n");
    printf("-h        Show help\n");
    printf("--help    Show help\n");
    printf("-v        Increase verbose level\n");
    printf("\n");
    printf("Options only for daemon connection:\n");
    printf("-l<level>  1: Support L1 events\n");
    printf("          12: Support L1 and L1 over L2 events (default)\n");
    printf("-u<user>  User name for VSCP daemon access\n");
    printf("-p<pass>  Password for VSCP daemon access\n");
                
    return;
}

/**
 * This function prints the key table to the console.
 */
static void main_showKeyTable(void)
{
    printf("?       Show this key table\n");
    printf("e       Dump EEPROM\n");
    printf("h       Enable/Disable node heartbeat\n");
    printf("i       Start node segment initialization\n");
    printf("q       Quit program\n");
    printf("0-9     Send info button event\n");

    return;
}

/**
 * This thread process the VSCP framework.
 *
 * @param[in]   par Thread parameters
 * @return  Not used
 */
static void* main_vscpFrameworkThread(void* par)
{
    BOOL                quitFlag    = FALSE;
    main_ThreadData*    threadData  = (main_ThreadData*)par;
    
    (void)pthread_mutex_lock(&threadData->mutex);
    quitFlag = threadData->quitFlag;
    (void)pthread_mutex_unlock(&threadData->mutex);
    
    while(FALSE == quitFlag)
    {
        /* Process the whole VSCP framework */
        (void)pthread_mutex_lock(&threadData->mutex);
        
        vscp_core_process();
        
        quitFlag = threadData->quitFlag;
        
        (void)pthread_mutex_unlock(&threadData->mutex);

        /* Give a other threads a minimal chance. */
        platform_delay(1);
    }
    
    pthread_exit(NULL);
    
    return NULL;
}

/**
 * This thread process the VSCP framework.
 *
 * @param[in]   par Thread parameters
 * @return  Not used
 */
static void* main_vscpTimerThread(void* par)
{
    BOOL                quitFlag    = FALSE;
    main_ThreadData*    threadData  = (main_ThreadData*)par;
    uint16_t            waitTime    = 100;
    
    (void)pthread_mutex_lock(&threadData->mutex);
    quitFlag = threadData->quitFlag;
    (void)pthread_mutex_unlock(&threadData->mutex);
    
    while(FALSE == quitFlag)
    {
        /* Wait a specific time, until the timers are processed again. */
        platform_delay(waitTime);

        (void)pthread_mutex_lock(&threadData->mutex);
        
        /* Process the VSCP framework timers */
        vscp_timer_process(waitTime);
        
        quitFlag = threadData->quitFlag;

        (void)pthread_mutex_unlock(&threadData->mutex);
    }
    
    pthread_exit(NULL);
    
    return NULL;
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

        for(index = 0; index < eepromSize; ++index)
        {
            if (0 == (index % EEPROM_STORAGE_MULTIPLE))
            {
                platform_setTextColor(PLATFORM_COLOR_GREY);
                printf("%04X: ", index);
                platform_setTextColor(color);
            }

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
#endif	/* (0 < VSCP_PS_SIZE_GUID) */
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
#endif	/* (0 < VSCP_PS_SIZE_MANUFACTURER_DEV_ID) */
#if (0 < VSCP_PS_SIZE_MANUFACTURER_SUB_DEV_ID)
            else if (VSCP_PS_ADDR_MANUFACTURER_SUB_DEV_ID == index)
            {
                nextColor = TRUE;
            }
#endif	/* (0 < VSCP_PS_SIZE_MANUFACTURER_SUB_DEV_ID) */
#if (0 < VSCP_PS_SIZE_MDF_URL)
            else if (VSCP_PS_ADDR_MDF_URL == index)
            {
                nextColor = TRUE;
            }
#endif	/* (0 < VSCP_PS_SIZE_MDF_URL) */
#if (0 < VSCP_PS_SIZE_STD_DEV_FAMILY_CODE)
            else if (VSCP_PS_ADDR_STD_DEV_FAMILY_CODE == index)
            {
                nextColor = TRUE;
            }
#endif	/* (0 < VSCP_PS_SIZE_STD_DEV_FAMILY_CODE) */
#if (0 < VSCP_PS_SIZE_STD_DEV_TYPE)
            else if (VSCP_PS_ADDR_STD_DEV_TYPE == index)
            {
                nextColor = TRUE;
            }
#endif	/* (0 < VSCP_PS_SIZE_STD_DEV_TYPE) */
#if (0 < VSCP_PS_SIZE_DM)
            else if (VSCP_PS_ADDR_DM == index)
            {
                nextColor = TRUE;
            }
#endif	/* (0 < VSCP_PS_SIZE_DM) */
#if (0 < VSCP_PS_SIZE_DM_EXTENSION)
            else if (VSCP_PS_ADDR_DM_EXTENSION == index)
            {
                nextColor = TRUE;
            }
#endif	/* (0 < VSCP_PS_SIZE_DM_EXTENSION) */
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
        
        platform_setTextColor(PLATFORM_COLOR_GREY);
        color       = PLATFORM_COLOR_RED;
        newColor    = PLATFORM_COLOR_RED;
        
        for(index = 0; index < eepromSize; ++index)
        {
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
#endif	/* (0 < VSCP_PS_SIZE_GUID) */
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
#endif	/* (0 < VSCP_PS_SIZE_MANUFACTURER_DEV_ID) */
#if (0 < VSCP_PS_SIZE_MANUFACTURER_SUB_DEV_ID)
            else if (VSCP_PS_ADDR_MANUFACTURER_SUB_DEV_ID == index)
            {
                nextColor = TRUE;
            }
#endif	/* (0 < VSCP_PS_SIZE_MANUFACTURER_SUB_DEV_ID) */
#if (0 < VSCP_PS_SIZE_MDF_URL)
            else if (VSCP_PS_ADDR_MDF_URL == index)
            {
                nextColor = TRUE;
            }
#endif	/* (0 < VSCP_PS_SIZE_MDF_URL) */
#if (0 < VSCP_PS_SIZE_STD_DEV_FAMILY_CODE)
            else if (VSCP_PS_ADDR_STD_DEV_FAMILY_CODE == index)
            {
                nextColor = TRUE;
            }
#endif	/* (0 < VSCP_PS_SIZE_STD_DEV_FAMILY_CODE) */
#if (0 < VSCP_PS_SIZE_STD_DEV_TYPE)
            else if (VSCP_PS_ADDR_STD_DEV_TYPE == index)
            {
                nextColor = TRUE;
            }
#endif	/* (0 < VSCP_PS_SIZE_STD_DEV_TYPE) */
#if (0 < VSCP_PS_SIZE_DM)
            else if (VSCP_PS_ADDR_DM == index)
            {
                nextColor = TRUE;
            }
#endif	/* (0 < VSCP_PS_SIZE_DM) */
#if (0 < VSCP_PS_SIZE_DM_EXTENSION)
            else if (VSCP_PS_ADDR_DM_EXTENSION == index)
            {
                nextColor = TRUE;
            }
#endif	/* (0 < VSCP_PS_SIZE_DM_EXTENSION) */
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
        
        platform_setTextColor(PLATFORM_COLOR_GREY);
    }
    
    return;
}

/**
 * This function contains the main loop where every user key press is
 * interpreted.
 */
static void main_loop(void)
{
    int     keyValue        = 0;
    BOOL    nodeHeartbeat   = TRUE; /* Heartbeat is on per default */
                
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
                (void)pthread_mutex_lock(&main_vscpMutex);
                main_showKeyTable();
                (void)pthread_mutex_unlock(&main_vscpMutex);
            }
            /* Dump EEPROM */
            else if ('e' == keyValue)
            {
                (void)pthread_mutex_lock(&main_vscpMutex);
                main_dumpEEPROM();
                (void)pthread_mutex_unlock(&main_vscpMutex);
            }               
            /* Enable/Disable node hearbeat */
            else if ('h' == keyValue)
            {
                (void)pthread_mutex_lock(&main_vscpMutex);
                if (TRUE == nodeHeartbeat)
                {
                    printf("Disable node heartbeat.\n");
                    nodeHeartbeat = FALSE;
                }
                else
                {
                    printf("Enable node heartbeat.\n");
                    nodeHeartbeat = TRUE;
                }
                vscp_core_enableHeartbeat(nodeHeartbeat);
                (void)pthread_mutex_unlock(&main_vscpMutex);
            }
            /* Start node segment initialization */
            else if ('i' == keyValue)
            {
                (void)pthread_mutex_lock(&main_vscpMutex);
                printf("Start node segment initialization.\n");
                vscp_core_startNodeSegmentInit();
                (void)pthread_mutex_unlock(&main_vscpMutex);
            }
            /* Quit program */
            else if ('q' == keyValue)
            {
                (void)pthread_mutex_lock(&main_vscpMutex);
                printf("Quit.\n");
                (void)pthread_mutex_unlock(&main_vscpMutex);
            }
            /* Send a button message to the VSCP thread */
            else if (('0' <= keyValue) && ('9' >= keyValue))
            {
                vscp_RxMessage  rxMsg;
                
                rxMsg.vscpClass = VSCP_CLASS_L1_INFORMATION;
                rxMsg.vscpType  = VSCP_TYPE_INFORMATION_BUTTON;
                rxMsg.priority  = VSCP_PRIORITY_3_NORMAL;
                rxMsg.oAddr     = (uint8_t)(keyValue - '0') + 2;
                rxMsg.dataNum   = 7;
                rxMsg.data[0]   = (uint8_t)(keyValue - '0');
                rxMsg.data[1]   = 0xff;
                rxMsg.data[2]   = 0xff;
                rxMsg.data[3]   = 0;
                rxMsg.data[4]   = 0;
                rxMsg.data[5]   = 0;
                rxMsg.data[6]   = 0;
                
                (void)pthread_mutex_lock(&main_vscpMutex);
                vscp_tp_adapter_simulateReceivedMessage(&rxMsg);
                (void)pthread_mutex_unlock(&main_vscpMutex);
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
