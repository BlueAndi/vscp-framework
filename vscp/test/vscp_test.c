/* The MIT License (MIT)
 *
 * Copyright (c) 2014 - 2018, Andreas Merkle
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
@brief  VSCP module tests
@file   vscp_test.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
@see vscp_test.h

*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "vscp_test.h"
#include <stdint.h>
#include <string.h>
#include "CUnit.h"
#include "vscp_core.h"
#include "vscp_ps.h"
#include "vscp_class_l1.h"
#include "vscp_type_protocol.h"
#include "vscp_type_information.h"
#include "vscp_stubs.h"
#include "vscp_dev_data.h"
#include "vscp_dm_ng.h"

/*******************************************************************************
    COMPILER SWITCHES
*******************************************************************************/

/*******************************************************************************
    CONSTANTS
*******************************************************************************/

/** Size of the persistent memory in byte */
#define VSCP_TEST_PS_SIZE                       VSCP_PS_ADDR_NEXT

/** Number of tx message buffers */
#define VSCP_TEST_TX_MSG_NUM                    10

/** Node nickname id for tests */
#define VSCP_TEST_NICKNAME                      (0x5A)

/** Remote node nickname id for tests */
#define VSCP_TEST_NICKNAME_REMOTE               (0x12)

/** Segment CRC */
#define VSCP_TEST_SEGMENT_CRC                   (0x34)

/** Firmware version: major */
#define VSCP_TEST_FIRMWARE_VERSION_MAJOR        1

/** Firmware version: minor */
#define VSCP_TEST_FIRMWARE_VERSION_MINOR        2

/** Firmware version: sub minor */
#define VSCP_TEST_FIRMWARE_VERSION_SUB_MINOR    3

/** Manufacturer device id */
#define VSCP_TEST_MANUFACTURER_DEV_ID           ((uint32_t)0x12345678)

/** Manufacturer sub device id */
#define VSCP_TEST_MANUFACTURER_SUB_DEV_ID       ((uint32_t)0x90123456)

/** Boot loader algorithm */
#define VSCP_TEST_BOOT_LOADER_ALGORITHM         (0x01)

/** Number of used pages */
#define VSCP_TEST_PAGES_USED                    1

/** Family code */
#define VSCP_TEST_STD_DEVICE_FAMILY_CODE        ((uint32_t)0x87654321)

/** Standard device type */
#define VSCP_TEST_STD_DEVICE_TYPE               ((uint32_t)0x19283af1)

/** GUID part 0 */
#define VSCP_TEST_GUID_0                        ((uint32_t)0x10293847)

/** GUID part 1 */
#define VSCP_TEST_GUID_1                        ((uint32_t)0xa1b2c3d4)

/** GUID part 2 */
#define VSCP_TEST_GUID_2                        ((uint32_t)0xf1f2f3f4)

/** GUID part 3 */
#define VSCP_TEST_GUID_3                        ((uint32_t)0x05040107)

/** MDF URL */
#define VSCP_TEST_MDF_URL                       "www.blue-andi.de/vscp/test.mdf"

/** Heartbeat zone */
#define VSCP_TEST_HEARTBEAT_ZONE                (0xff)

/** Heartbeat sub zone */
#define VSCP_TEST_HEARTBEAT_SUB_ZONE            (0xff)

/** Time since epoch: 03/31/2018 @ 3:45pm (UTC) */
#define VSCP_TEST_TIME_SINCE_EPOCH              (1522511138)

/*******************************************************************************
    MACROS
*******************************************************************************/

/** Get number of elements in an array. */
#define VSCP_TEST_ARRAY_NUM(__array)    (sizeof(__array)/sizeof((__array)[0]))

/*******************************************************************************
    TYPES AND STRUCTURES
*******************************************************************************/

/** Call counter indices for persistent memory. */
typedef enum
{
    VSCP_TEST_CALL_COUNTER_APP_RESTORE_FACTORY_DEFAULTS = 0,

    VSCP_TEST_CALL_COUNTER_APP_REG_INIT,

    VSCP_TEST_CALL_COUNTER_ACTION_INIT,
    VSCP_TEST_CALL_COUNTER_ACTION_EXECUTE,

    VSCP_TEST_CALL_COUNTER_TP_ADAPTER_INIT,
    VSCP_TEST_CALL_COUNTER_TP_ADAPTER_READ_MESSAGE,
    VSCP_TEST_CALL_COUNTER_TP_ADAPTER_WRITE_MESSAGE,

    VSCP_TEST_CALL_COUNTER_PORTABLE_INIT,
    VSCP_TEST_CALL_COUNTER_PORTABLE_CREATE_TIMER,
    VSCP_TEST_CALL_COUNTER_PORTABLE_START_TIMER,
    VSCP_TEST_CALL_COUNTER_PORTABLE_STOP_TIMER,
    VSCP_TEST_CALL_COUNTER_PORTABLE_RESET_REQUEST,
    VSCP_TEST_CALL_COUNTER_PORTABLE_IDLE_STATE_ENTERED,
    VSCP_TEST_CALL_COUNTER_PORTABLE_ERROR_STATE_ENTERED,
    VSCP_TEST_CALL_COUNTER_PORTABLE_BOOT_LOADER_REQUEST,
    VSCP_TEST_CALL_COUNTER_PORTABLE_SET_LAMP_STATE,
    VSCP_TEST_CALL_COUNTER_PORTABLE_PROVIDE_EVENT,

    VSCP_TEST_CALL_COUNTER_PS_ACCESS_INIT,

    VSCP_TEST_CALL_COUNTER_TIMER_INIT,

    VSCP_TEST_CALL_COUNTER_SIZE

} VSCP_TEST_CALL_COUNTER;

/** Timer value indices */
typedef enum
{
    VSCP_TEST_TIMER_0 = 0,
    VSCP_TEST_TIMER_1,
    VSCP_TEST_TIMER_2,
    VSCP_TEST_TIMER_3,

    VSCP_TEST_TIMER_SIZE

} VSCP_TEST_TIMER;

/** Register access types */
typedef enum
{
    VSCP_TEST_REG_ACCESS_READ_WRITE = 0,
    VSCP_TEST_REG_ACCESS_READ_ONLY,
    VSCP_TEST_REG_ACCESS_WRITE_ONLY

} VSCP_TEST_REG_ACCESS;

/** Register test table line */
typedef struct
{
    uint8_t                 addr;                               /**< Register address */
    VSCP_TEST_REG_ACCESS    access;                             /**< Register access */
    uint8_t                 (*getExpectedValue)(uint8_t par);   /**< Function to get the expected value */
    uint8_t                 value;                              /**< Parameter for function or expected value */
    char*                   description;                        /**< Description */

} vscp_test_RegTestLine;

/*******************************************************************************
    PROTOTYPES
*******************************************************************************/

static void vscp_test_initTestCase(void);
static void vscp_test_waitForTxMessage(uint8_t min, uint16_t max);
static void vscp_test_processTimers(void);
static uint8_t  vscp_test_readRegister(uint8_t addr);
static uint8_t  vscp_test_writeRegister(uint8_t addr, uint8_t value, BOOL readOnly);

static uint8_t  vscp_test_getAlarmStatus(uint8_t value);
static uint8_t  vscp_test_getControlFlags(uint8_t value);
static uint8_t  vscp_test_getNicknameId(uint8_t value);
static uint8_t  vscp_test_getZero(uint8_t value);

/*******************************************************************************
    LOCAL VARIABLES
*******************************************************************************/

/** Persistent memory storage */
static uint8_t          vscp_test_persistentMemory[VSCP_TEST_PS_SIZE];

/** Call counter */
static uint8_t          vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_SIZE];

/** Receive message storage */
static vscp_RxMessage   vscp_test_rxMessage;

/** Transmit message storage */
static vscp_TxMessage   vscp_test_txMessage[VSCP_TEST_TX_MSG_NUM];

/** Transmit message counter */
static uint8_t          vscp_test_txMessageCnt      = 0;

/** Timer values */
static uint16_t         vscp_test_timerValues[VSCP_TEST_TIMER_SIZE];

/** Number of timer instances. */
static uint8_t          vscp_test_timerInstances    = 0;

/** Register test table */
static vscp_test_RegTestLine    vscp_test_registerTestTable[]   =
{
    {   VSCP_REG_ALARM_STATUS,                  VSCP_TEST_REG_ACCESS_READ_ONLY,   vscp_test_getAlarmStatus,   0x89,                                               "alarm status"                  },
    {   VSCP_REG_VSCP_VERSION_MAJOR,            VSCP_TEST_REG_ACCESS_READ_ONLY,   NULL,                       VSCP_CORE_VERSION_MAJOR,                            "vscp major version"            },
    {   VSCP_REG_VSCP_VERSION_MINOR,            VSCP_TEST_REG_ACCESS_READ_ONLY,   NULL,                       VSCP_CORE_VERSION_MINOR,                            "vscp minor version"            },
    {   VSCP_REG_NODE_CONTROL_FLAGS,            VSCP_TEST_REG_ACCESS_READ_WRITE,  vscp_test_getControlFlags,  0,                                                  "node control flags"            },
    {   VSCP_REG_USER_ID_0,                     VSCP_TEST_REG_ACCESS_READ_WRITE,  NULL,                       0x11,                                               "user id 0"                     },
    {   VSCP_REG_USER_ID_1,                     VSCP_TEST_REG_ACCESS_READ_WRITE,  NULL,                       0x22,                                               "user id 1"                     },
    {   VSCP_REG_USER_ID_2,                     VSCP_TEST_REG_ACCESS_READ_WRITE,  NULL,                       0x33,                                               "user id 2"                     },
    {   VSCP_REG_USER_ID_3,                     VSCP_TEST_REG_ACCESS_READ_WRITE,  NULL,                       0x44,                                               "user id 3"                     },
    {   VSCP_REG_USER_ID_4,                     VSCP_TEST_REG_ACCESS_READ_WRITE,  NULL,                       0x55,                                               "user id 4"                     },
    {   VSCP_REG_MANUFACTURER_DEV_ID_0,         VSCP_TEST_REG_ACCESS_READ_ONLY,   NULL,                       (VSCP_TEST_MANUFACTURER_DEV_ID >>  0) & 0xff,       "manufacturer device id 0"      },
    {   VSCP_REG_MANUFACTURER_DEV_ID_1,         VSCP_TEST_REG_ACCESS_READ_ONLY,   NULL,                       (VSCP_TEST_MANUFACTURER_DEV_ID >>  8) & 0xff,       "manufacturer device id 1"      },
    {   VSCP_REG_MANUFACTURER_DEV_ID_2,         VSCP_TEST_REG_ACCESS_READ_ONLY,   NULL,                       (VSCP_TEST_MANUFACTURER_DEV_ID >> 16) & 0xff,       "manufacturer device id 2"      },
    {   VSCP_REG_MANUFACTURER_DEV_ID_3,         VSCP_TEST_REG_ACCESS_READ_ONLY,   NULL,                       (VSCP_TEST_MANUFACTURER_DEV_ID >> 24) & 0xff,       "manufacturer device id 3"      },
    {   VSCP_REG_MANUFACTURER_SUB_DEV_ID_0,     VSCP_TEST_REG_ACCESS_READ_ONLY,   NULL,                       (VSCP_TEST_MANUFACTURER_SUB_DEV_ID >>  0) & 0xff,   "manufacturer sub device id 0"  },
    {   VSCP_REG_MANUFACTURER_SUB_DEV_ID_1,     VSCP_TEST_REG_ACCESS_READ_ONLY,   NULL,                       (VSCP_TEST_MANUFACTURER_SUB_DEV_ID >>  8) & 0xff,   "manufacturer sub device id 1"  },
    {   VSCP_REG_MANUFACTURER_SUB_DEV_ID_2,     VSCP_TEST_REG_ACCESS_READ_ONLY,   NULL,                       (VSCP_TEST_MANUFACTURER_SUB_DEV_ID >> 16) & 0xff,   "manufacturer sub device id 2"  },
    {   VSCP_REG_MANUFACTURER_SUB_DEV_ID_3,     VSCP_TEST_REG_ACCESS_READ_ONLY,   NULL,                       (VSCP_TEST_MANUFACTURER_SUB_DEV_ID >> 24) & 0xff,   "manufacturer sub device id 3"  },
    {   VSCP_REG_NICKNAME_ID,                   VSCP_TEST_REG_ACCESS_READ_ONLY,   vscp_test_getNicknameId,    0,                                                  "nickname id"                   },
    {   VSCP_REG_PAGE_SELECT_MSB,               VSCP_TEST_REG_ACCESS_READ_WRITE,  NULL,                       0,                                                  "page select msb"               },
    {   VSCP_REG_PAGE_SELECT_LSB,               VSCP_TEST_REG_ACCESS_READ_WRITE,  NULL,                       0,                                                  "page select lsb"               },
    {   VSCP_REG_FIRMWARE_VERSION_MAJOR,        VSCP_TEST_REG_ACCESS_READ_ONLY,   NULL,                       VSCP_TEST_FIRMWARE_VERSION_MAJOR,                   "firmware version major"        },
    {   VSCP_REG_FIRMWARE_VERSION_MINOR,        VSCP_TEST_REG_ACCESS_READ_ONLY,   NULL,                       VSCP_TEST_FIRMWARE_VERSION_MINOR,                   "firmware version minor"        },
    {   VSCP_REG_FIRMWARE_VERSION_SUB_MINOR,    VSCP_TEST_REG_ACCESS_READ_ONLY,   NULL,                       VSCP_TEST_FIRMWARE_VERSION_SUB_MINOR,               "firmware version sub minor"    },
    {   VSCP_REG_BOOT_LOADER_ALGORITHM,         VSCP_TEST_REG_ACCESS_READ_ONLY,   NULL,                       VSCP_TEST_BOOT_LOADER_ALGORITHM,                    "boot loader algorithm"         },
    {   VSCP_REG_BUFFER_SIZE,                   VSCP_TEST_REG_ACCESS_READ_ONLY,   NULL,                       VSCP_L1_DATA_SIZE,                                  "buffer size"                   },
    {   VSCP_REG_PAGES_USED,                    VSCP_TEST_REG_ACCESS_READ_ONLY,   NULL,                       VSCP_TEST_PAGES_USED,                               "pages used"                    },
    {   VSCP_REG_STD_DEV_FAMILY_CODE_3,         VSCP_TEST_REG_ACCESS_READ_ONLY,   NULL,                       (VSCP_TEST_STD_DEVICE_FAMILY_CODE >> 24) & 0xff,    "standard device family code 3" },
    {   VSCP_REG_STD_DEV_FAMILY_CODE_2,         VSCP_TEST_REG_ACCESS_READ_ONLY,   NULL,                       (VSCP_TEST_STD_DEVICE_FAMILY_CODE >> 16) & 0xff,    "standard device family code 2" },
    {   VSCP_REG_STD_DEV_FAMILY_CODE_1,         VSCP_TEST_REG_ACCESS_READ_ONLY,   NULL,                       (VSCP_TEST_STD_DEVICE_FAMILY_CODE >>  8) & 0xff,    "standard device family code 1" },
    {   VSCP_REG_STD_DEV_FAMILY_CODE_0,         VSCP_TEST_REG_ACCESS_READ_ONLY,   NULL,                       (VSCP_TEST_STD_DEVICE_FAMILY_CODE >>  0) & 0xff,    "standard device family code 0" },
    {   VSCP_REG_STD_DEV_TYPE_3,                VSCP_TEST_REG_ACCESS_READ_ONLY,   NULL,                       (VSCP_TEST_STD_DEVICE_TYPE >> 24) & 0xff,           "standard device type 3"        },
    {   VSCP_REG_STD_DEV_TYPE_2,                VSCP_TEST_REG_ACCESS_READ_ONLY,   NULL,                       (VSCP_TEST_STD_DEVICE_TYPE >> 16) & 0xff,           "standard device type 2"        },
    {   VSCP_REG_STD_DEV_TYPE_1,                VSCP_TEST_REG_ACCESS_READ_ONLY,   NULL,                       (VSCP_TEST_STD_DEVICE_TYPE >>  8) & 0xff,           "standard device type 1"        },
    {   VSCP_REG_STD_DEV_TYPE_0,                VSCP_TEST_REG_ACCESS_READ_ONLY,   NULL,                       (VSCP_TEST_STD_DEVICE_TYPE >>  0) & 0xff,           "standard device type 0"        },
    {   VSCP_REG_RESTORE_STD_CFG,               VSCP_TEST_REG_ACCESS_WRITE_ONLY,  vscp_test_getZero,          0,                                                  "restore default settings"      },
    {   VSCP_REG_GUID_15,                       VSCP_TEST_REG_ACCESS_READ_ONLY,   NULL,                       (VSCP_TEST_GUID_3 >> 24) & 0xff,                    "GUID 15"                       }, /* MSB */
    {   VSCP_REG_GUID_14,                       VSCP_TEST_REG_ACCESS_READ_ONLY,   NULL,                       (VSCP_TEST_GUID_3 >> 16) & 0xff,                    "GUID 14"                       },
    {   VSCP_REG_GUID_13,                       VSCP_TEST_REG_ACCESS_READ_ONLY,   NULL,                       (VSCP_TEST_GUID_3 >>  8) & 0xff,                    "GUID 13"                       },
    {   VSCP_REG_GUID_12,                       VSCP_TEST_REG_ACCESS_READ_ONLY,   NULL,                       (VSCP_TEST_GUID_3 >>  0) & 0xff,                    "GUID 12"                       },
    {   VSCP_REG_GUID_11,                       VSCP_TEST_REG_ACCESS_READ_ONLY,   NULL,                       (VSCP_TEST_GUID_2 >> 24) & 0xff,                    "GUID 11"                       },
    {   VSCP_REG_GUID_10,                       VSCP_TEST_REG_ACCESS_READ_ONLY,   NULL,                       (VSCP_TEST_GUID_2 >> 16) & 0xff,                    "GUID 10"                       },
    {   VSCP_REG_GUID_9,                        VSCP_TEST_REG_ACCESS_READ_ONLY,   NULL,                       (VSCP_TEST_GUID_2 >>  8) & 0xff,                    "GUID 9"                        },
    {   VSCP_REG_GUID_8,                        VSCP_TEST_REG_ACCESS_READ_ONLY,   NULL,                       (VSCP_TEST_GUID_2 >>  0) & 0xff,                    "GUID 8"                        },
    {   VSCP_REG_GUID_7,                        VSCP_TEST_REG_ACCESS_READ_ONLY,   NULL,                       (VSCP_TEST_GUID_1 >> 24) & 0xff,                    "GUID 7"                        },
    {   VSCP_REG_GUID_6,                        VSCP_TEST_REG_ACCESS_READ_ONLY,   NULL,                       (VSCP_TEST_GUID_1 >> 16) & 0xff,                    "GUID 6"                        },
    {   VSCP_REG_GUID_5,                        VSCP_TEST_REG_ACCESS_READ_ONLY,   NULL,                       (VSCP_TEST_GUID_1 >>  8) & 0xff,                    "GUID 5"                        },
    {   VSCP_REG_GUID_4,                        VSCP_TEST_REG_ACCESS_READ_ONLY,   NULL,                       (VSCP_TEST_GUID_1 >>  0) & 0xff,                    "GUID 4"                        },
    {   VSCP_REG_GUID_3,                        VSCP_TEST_REG_ACCESS_READ_ONLY,   NULL,                       (VSCP_TEST_GUID_0 >> 24) & 0xff,                    "GUID 3"                        },
    {   VSCP_REG_GUID_2,                        VSCP_TEST_REG_ACCESS_READ_ONLY,   NULL,                       (VSCP_TEST_GUID_0 >> 16) & 0xff,                    "GUID 2"                        },
    {   VSCP_REG_GUID_1,                        VSCP_TEST_REG_ACCESS_READ_ONLY,   NULL,                       (VSCP_TEST_GUID_0 >>  8) & 0xff,                    "GUID 1"                        },
    {   VSCP_REG_GUID_0,                        VSCP_TEST_REG_ACCESS_READ_ONLY,   NULL,                       (VSCP_TEST_GUID_0 >>  0) & 0xff,                    "GUID 0"                        } /* LSB */
};

/** Decision matrix */
static vscp_dm_MatrixRow*   vscp_test_dmStorage     = (vscp_dm_MatrixRow*)&vscp_test_persistentMemory[VSCP_PS_ADDR_DM];

/** Decision matrix page */
static uint16_t             vscp_test_dmPage        = VSCP_CONFIG_DM_PAGE;

/** Decision matrix offset */
static uint8_t              vscp_test_dmOffset      = VSCP_CONFIG_DM_OFFSET;

/** Decision matrix extension */
static vscp_dm_ExtRow*      vscp_test_extStorage    = (vscp_dm_ExtRow*)&vscp_test_persistentMemory[VSCP_PS_ADDR_DM_EXTENSION];

/** Decision matrix next generation */
static uint8_t*             vscp_test_dmNG          = &vscp_test_persistentMemory[VSCP_PS_ADDR_DM_NEXT_GENERATION];

/** Action id */
static uint8_t              vscp_test_action    = 0;

/** Action parameter */
static uint8_t              vscp_test_actionPar = 0;

/** VSCP event which triggered the action */
static vscp_RxMessage       vscp_test_actionTriggeredMsg;

/** Lamp state */
static VSCP_LAMP_STATE      vscp_test_lampState = VSCP_LAMP_STATE_OFF;

/** Simulate no more timers available. */
static BOOL                 vscp_test_noMoreTimers  = FALSE;

/*******************************************************************************
    GLOBAL VARIABLES
*******************************************************************************/

/*******************************************************************************
    GLOBAL FUNCTIONS
*******************************************************************************/

/**
 * Initializes the persistent memory with default values (0xFF).
 * Resets all call counters.
 * Clear rx and tx message buffers.
 * Resets all timers.
 * Reset the decision matrix.
 */
extern int  vscp_test_init(void)
{
    /* Set persistent memory to default value. */
    memset(vscp_test_persistentMemory, 0xFF, sizeof(vscp_test_persistentMemory));

    /* Reset call counter */
    memset(vscp_test_callCounter, 0x00, sizeof(vscp_test_callCounter));

    /* Clear receive and transmit buffers */
    memset(&vscp_test_rxMessage, 0, sizeof(vscp_test_rxMessage));
    vscp_test_txMessageCnt = 0;
    memset(vscp_test_txMessage, 0, sizeof(vscp_test_txMessage));

    /* Reset all timers */
    vscp_test_timerInstances = 0;
    memset(&vscp_test_timerValues, 0, sizeof(vscp_test_timerValues));

    /* Reset decision matrix */
    memset(vscp_test_dmStorage, 0, VSCP_PS_SIZE_DM);

    /* Reset decision matrix extension */
    memset(vscp_test_extStorage, 0, VSCP_PS_SIZE_DM_EXTENSION);

    /* Reset action triggered VSCP event */
    memset(&vscp_test_actionTriggeredMsg, 0, sizeof(vscp_test_actionTriggeredMsg));

    return 0;
}

/**
 * Precondition:
 *  - VSCP is not initialized yet.
 *
 * Action:
 *  - Initialize VSCP framework.
 *
 * Expectation:
 *  - All VSCP layers are initialized.
 *  - Persistent memory is initialized and contains default values.
 *  - Lamp blinks.
 */
extern void vscp_test_initNodeTheFirstTime(void)
{
    /* Initialize core */
    CU_ASSERT_EQUAL(VSCP_CORE_RET_OK, vscp_core_init());

    /* Persistent memory module shall be initialized. */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_PS_ACCESS_INIT], 1);

    /* Transport layer shall be initialized. */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_TP_ADAPTER_INIT], 1);

    /* Portable layer shall be initialized. */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_PORTABLE_INIT], 1);

    /* Timer layer shall be initialized. */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_TIMER_INIT], 1);

    /* Application register module shall be initialized. */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_APP_REG_INIT], 1);

    /* Action module shall be initialized. */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_ACTION_INIT], 1);

    /* 4 timers shall be created, general timer, 2 multi-frame timers and heartbeat timer */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_PORTABLE_CREATE_TIMER], 4);

    /* Persistent memory shall contain default values.  */
    CU_ASSERT_EQUAL(vscp_ps_readNodeControlFlags(), VSCP_NODE_CONTROL_FLAGS_DEFAULT);
    CU_ASSERT_EQUAL(vscp_ps_readNicknameId(), VSCP_NICKNAME_NOT_INIT);
    CU_ASSERT_EQUAL(vscp_ps_readSegmentControllerCRC(), 0xFF);

    /* Nickname id value shall be set for not initialized nodes. */
    CU_ASSERT_EQUAL(vscp_core_readNicknameId(), VSCP_NICKNAME_NOT_INIT);

    /* Lamp shall blink slow */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_PORTABLE_SET_LAMP_STATE], 1);
    CU_ASSERT_EQUAL(vscp_test_lampState, VSCP_LAMP_STATE_BLINK_SLOW);

    return;
}

/**
 * Precondition:
 *  - VSCP is initialized just one call before.
 *
 * Action:
 *  - Process VSCP the first time.
 *
 * Expectation:
 *  - Nickname discovery starts.
 *  - Timer for nickname discovery observation is started.
 *  - Lamp blinks.
 */
extern void vscp_test_processNodeTheFirstTime(void)
{
    vscp_test_initTestCase();

    /* Process core */
    vscp_test_processTimers();
    vscp_core_process();

    /* Initialization shall only start if explicit requested. */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_TP_ADAPTER_WRITE_MESSAGE], 0);

    /* Request initialization */
    vscp_core_startNodeSegmentInit();

    /* Process core */
    vscp_test_waitForTxMessage(1, 10);

    /* Probe event shall be sent to segment master */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_TP_ADAPTER_WRITE_MESSAGE], 1);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpClass, VSCP_CLASS_L1_PROTOCOL);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpType, VSCP_TYPE_PROTOCOL_NEW_NODE_ONLINE);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].oAddr, VSCP_NICKNAME_NOT_INIT);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].hardCoded, FALSE);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].priority, VSCP_PRIORITY_7_LOW);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].dataNum, 1);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[0], VSCP_NICKNAME_SEGMENT_MASTER);

    /* Timer shall be started to observe the nickname discovery */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_PORTABLE_START_TIMER], 1);
    CU_ASSERT_EQUAL(vscp_test_timerValues[VSCP_TEST_TIMER_0], VSCP_CONFIG_NODE_SEGMENT_INIT_TIMEOUT);

    /* Lamp shall blink fast */
    CU_ASSERT_EQUAL(vscp_test_lampState, VSCP_LAMP_STATE_BLINK_FAST);

    return;
}

/**
 * Precondition:
 *  - Nickname discovery started and nickname is requested from segment master.
 *
 * Action:
 *  - Send nickname id back.
 *
 * Expectation:
 *  - Nickname is stored in persistent memory.
 *  - Node stops nickname discovery.
 *  - Lamp is on.
 */
extern void vscp_test_finisheNicknameDiscovery(void)
{
    vscp_test_initTestCase();

    /* Send probe acknowledge */
    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_PROTOCOL;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_PROTOCOL_PROBE_ACK;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = VSCP_NICKNAME_SEGMENT_MASTER;
    vscp_test_rxMessage.hardCoded   = FALSE;
    vscp_test_rxMessage.dataNum     = 0;

    /* Process core */
    vscp_test_waitForTxMessage(1, 10);

    /* No event shall be sent. */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_TP_ADAPTER_WRITE_MESSAGE], 0);

    /* Timer is not stopped! */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_PORTABLE_STOP_TIMER], 0);

    /* Set new nickname */
    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_PROTOCOL;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_PROTOCOL_SET_NICKNAME_ID;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_3_NORMAL;
    vscp_test_rxMessage.oAddr       = VSCP_NICKNAME_SEGMENT_MASTER;
    vscp_test_rxMessage.hardCoded   = FALSE;
    vscp_test_rxMessage.dataNum     = 2;
    vscp_test_rxMessage.data[0]     = VSCP_NICKNAME_NOT_INIT;
    vscp_test_rxMessage.data[1]     = VSCP_TEST_NICKNAME;

    /* Process core */
    vscp_test_waitForTxMessage(2, 2);

    /* Nickname is stored */
    CU_ASSERT_EQUAL(vscp_ps_readNicknameId(), VSCP_TEST_NICKNAME);

    /* Timer is stopped */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_PORTABLE_STOP_TIMER], 1);

    /* Nickname accepted shall be sent. */
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpClass, VSCP_CLASS_L1_PROTOCOL);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpType, VSCP_TYPE_PROTOCOL_NICKNAME_ID_ACCEPTED);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].oAddr, VSCP_TEST_NICKNAME);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].hardCoded, FALSE);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].dataNum, 0);

    /* New node online shall be sent. */
    CU_ASSERT_EQUAL(vscp_test_txMessage[1].vscpClass, VSCP_CLASS_L1_PROTOCOL);
    CU_ASSERT_EQUAL(vscp_test_txMessage[1].vscpType, VSCP_TYPE_PROTOCOL_NEW_NODE_ONLINE);
    CU_ASSERT_EQUAL(vscp_test_txMessage[1].oAddr, VSCP_TEST_NICKNAME);
    CU_ASSERT_EQUAL(vscp_test_txMessage[1].hardCoded, FALSE);
    CU_ASSERT_EQUAL(vscp_test_txMessage[1].dataNum, 1);
    CU_ASSERT_EQUAL(vscp_test_txMessage[1].data[0], VSCP_TEST_NICKNAME);

    /* Node is in active state */
    CU_ASSERT_EQUAL(vscp_core_isActive(), TRUE);

    /* Lamp shall be on */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_PORTABLE_SET_LAMP_STATE], 1);
    CU_ASSERT_EQUAL(vscp_test_lampState, VSCP_LAMP_STATE_ON);

    return;
}

/**
 * Precondition:
 *  - Nickname discovery finished and nickname is set.
 *  - Node sent a new node online event.
 *
 * Action:
 *  - Send segment controller heartbeat event.
 *
 * Expectation:
 *  - Node stores the segment CRC.
 *  - Node doesn't initialize again.
 */
extern void vscp_test_firstSegCtrlHeartBeat(void)
{
    vscp_test_initTestCase();

    /* Send probe acknowledge */
    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_PROTOCOL;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_PROTOCOL_SEGMENT_CONTROLLER_HEARTBEAT;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = VSCP_NICKNAME_SEGMENT_MASTER;
    vscp_test_rxMessage.hardCoded   = FALSE;
    vscp_test_rxMessage.dataNum     = 5;
    vscp_test_rxMessage.data[0]     = VSCP_TEST_SEGMENT_CRC;
    vscp_test_rxMessage.data[1]     = 0x00; /* Time since epoch */
    vscp_test_rxMessage.data[2]     = 0x00; /* Time since epoch */
    vscp_test_rxMessage.data[3]     = 0x00; /* Time since epoch */
    vscp_test_rxMessage.data[4]     = 0x00; /* Time since epoch */

    vscp_test_waitForTxMessage(1, 10);

    /* Node shall send nothing. */
    CU_ASSERT_EQUAL(vscp_test_txMessageCnt, 0);

    /* Node shall store the segment CRC. */
    CU_ASSERT_EQUAL(vscp_ps_readSegmentControllerCRC(), VSCP_TEST_SEGMENT_CRC);

    /* Lamp shall be on */
    CU_ASSERT_EQUAL(vscp_test_lampState, VSCP_LAMP_STATE_ON);

    return;
}

/**
 * Precondition:
 *  - Nickname discovery finished and nickname is set.
 *  - Node sent a new node online event.
 *
 * Action:
 *  - Send a probe acknowledge with the same nickname.
 *
 * Expectation:
 *  - Nickname is cleared in persistent memory.
 *  - Node sends a probe event to segment master.
 */
extern void vscp_test_sendProbeAckInActiveState(void)
{
    vscp_test_initTestCase();

    /* Send probe acknowledge */
    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_PROTOCOL;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_PROTOCOL_PROBE_ACK;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = VSCP_TEST_NICKNAME;
    vscp_test_rxMessage.hardCoded   = FALSE;
    vscp_test_rxMessage.dataNum     = 0;

    /* Wait for at least 1 event */
    vscp_test_waitForTxMessage(1, 10);

    /* Nickname shall be cleared. */
    CU_ASSERT_EQUAL(vscp_ps_readNicknameId(), VSCP_NICKNAME_NOT_INIT);

    /* Segment controller CRC shall not be cleared. */
    CU_ASSERT_EQUAL(vscp_ps_readSegmentControllerCRC(),VSCP_TEST_SEGMENT_CRC);

    /* Probe event shall be sent to segment master */
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpClass, VSCP_CLASS_L1_PROTOCOL);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpType, VSCP_TYPE_PROTOCOL_NEW_NODE_ONLINE);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].oAddr, VSCP_NICKNAME_NOT_INIT);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].hardCoded, FALSE);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].priority, VSCP_PRIORITY_7_LOW);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].dataNum, 1);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[0], VSCP_NICKNAME_SEGMENT_MASTER);

    /* Lamp shall blink fast */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_PORTABLE_SET_LAMP_STATE], 1);
    CU_ASSERT_EQUAL(vscp_test_lampState, VSCP_LAMP_STATE_BLINK_FAST);

    return;
}

/**
 * Precondition:
 *  - Node sent a probe event to segment master.
 *
 * Action:
 *  - No response (no segment master).
 *
 * Expectation:
 *  - After a specific time, the node starts probing id for id.
 *  - But because every probe gets an answer, the node surrenders at the end.
 */
extern void vscp_test_noSegmentMaster(void)
{
    uint8_t id  = 1;

    vscp_test_initTestCase();

    /* Wait for at least 1 event */
    vscp_test_waitForTxMessage(1, VSCP_CONFIG_NODE_SEGMENT_INIT_TIMEOUT + 1);

    while(255 > id)
    {
        /* Probe event shall be sent with nickname id 1 */
        CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpClass, VSCP_CLASS_L1_PROTOCOL);
        CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpType, VSCP_TYPE_PROTOCOL_NEW_NODE_ONLINE);
        CU_ASSERT_EQUAL(vscp_test_txMessage[0].oAddr, VSCP_NICKNAME_NOT_INIT);
        CU_ASSERT_EQUAL(vscp_test_txMessage[0].hardCoded, FALSE);
        CU_ASSERT_EQUAL(vscp_test_txMessage[0].priority, VSCP_PRIORITY_7_LOW);
        CU_ASSERT_EQUAL(vscp_test_txMessage[0].dataNum, 1);
        CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[0], id);

        /* Send probe acknowledge */
        vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_PROTOCOL;
        vscp_test_rxMessage.vscpType    = VSCP_TYPE_PROTOCOL_PROBE_ACK;
        vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
        vscp_test_rxMessage.oAddr       = id;
        vscp_test_rxMessage.hardCoded   = FALSE;
        vscp_test_rxMessage.dataNum     = 0;

        /* Next id */
        ++id;

        vscp_test_waitForTxMessage(1, 2);
    }

    /* Node stops and sends 0xFF as nickaname to notify it surrenders */
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpClass, VSCP_CLASS_L1_PROTOCOL);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpType, VSCP_TYPE_PROTOCOL_NEW_NODE_ONLINE);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].oAddr, VSCP_NICKNAME_NOT_INIT);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].hardCoded, FALSE);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].priority, VSCP_PRIORITY_7_LOW);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].dataNum, 1);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[0], id);

    /* Lamp shall be off */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_PORTABLE_SET_LAMP_STATE], 1);
    CU_ASSERT_EQUAL(vscp_test_lampState, VSCP_LAMP_STATE_OFF);

    return;
}

/**
 * Precondition:
 *  - VSCP is not called yet.
 *
 * Action:
 *  - Limit number of available timers.
 *
 * Expectation:
 *  - VSCP core is in error state
 */
extern void vscp_test_init07(void)
{
    vscp_test_noMoreTimers = TRUE;

    /* Initialize core */
    CU_ASSERT_EQUAL(VSCP_CORE_RET_ERROR, vscp_core_init());

    vscp_test_noMoreTimers = FALSE;

    /* ProcessVSCP core */
    vscp_core_process();

    /* VSCP shall be in error state */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_PORTABLE_ERROR_STATE_ENTERED], 1);

    /* Nickname id value shall be set for not initialized nodes. */
    CU_ASSERT_EQUAL(vscp_core_readNicknameId(), VSCP_NICKNAME_NOT_INIT);

    /* Lamp shall be off */
    CU_ASSERT_EQUAL(vscp_test_lampState, VSCP_LAMP_STATE_OFF);

    return;
}

/**
 * Initializes the persistent memory with default values (0xFF).
 * Resets all call counters.
 * Clear rx and tx message buffers.
 * Resets all timers.
 * Set a valid node nickname.
 * Set a valid start up control.
 * Set a valid segment crc.
 */
extern int  vscp_test_initActive(void)
{
    /* Set persistent memory to default value. */
    memset(vscp_test_persistentMemory, 0xFF, sizeof(vscp_test_persistentMemory));

    /* Reset call counter */
    memset(vscp_test_callCounter, 0x00, sizeof(vscp_test_callCounter));

    /* Clear receive and transmit buffers */
    memset(&vscp_test_rxMessage, 0, sizeof(vscp_test_rxMessage));
    vscp_test_txMessageCnt = 0;
    memset(vscp_test_txMessage, 0, sizeof(vscp_test_txMessage));

    /* Reset all timers */
    vscp_test_timerInstances = 0;
    memset(&vscp_test_timerValues, 0, sizeof(vscp_test_timerValues));

    vscp_core_restoreFactoryDefaultSettings();

    /* Set nickname */
    vscp_ps_writeNicknameId(VSCP_TEST_NICKNAME);

    /* Set start up control */
    vscp_ps_writeNodeControlFlags(0x40);

    /* Set segment crc */
    vscp_ps_writeSegmentControllerCRC(VSCP_TEST_SEGMENT_CRC);

    return 0;
}

/**
 * Precondition:
 *  - VSCP is not initialized yet.
 *  - Nickname is set.
 *  - Node control flags are valid.
 *
 * Action:
 *  - Initialize and process VSCP.
 *
 * Expectation:
 *  - Persistent memory is initialized.
 *  - All VSCP layers are initialized.
 *  - New node online event is sent.
 */
extern void vscp_test_active01(void)
{
    /* Initialize core */
    CU_ASSERT_EQUAL(VSCP_CORE_RET_OK, vscp_core_init());

    /* Persistent memory module shall be initialized. */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_PS_ACCESS_INIT], 1);

    /* Transport layer shall be initialized. */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_TP_ADAPTER_INIT], 1);

    /* Portable layer shall be initialized. */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_PORTABLE_INIT], 1);

    /* Timer layer shall be initialized. */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_TIMER_INIT], 1);

    /* Application register module shall be initialized. */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_APP_REG_INIT], 1);

    /* Action module shall be initialized. */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_ACTION_INIT], 1);

    /* 4 timers shall be created, general timer, multi-frame timers and heartbeat timer */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_PORTABLE_CREATE_TIMER], 4);

    /* Nickname shall not be overwritten.  */
    CU_ASSERT_EQUAL(vscp_ps_readNicknameId(), VSCP_TEST_NICKNAME);

    vscp_test_waitForTxMessage(1, 10);

    CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpClass, VSCP_CLASS_L1_PROTOCOL);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpType, VSCP_TYPE_PROTOCOL_NEW_NODE_ONLINE);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].oAddr, VSCP_TEST_NICKNAME);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].hardCoded, FALSE);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].dataNum, 1);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[0], VSCP_TEST_NICKNAME);

    /* Lamp shall be on */
    CU_ASSERT_EQUAL(vscp_test_lampState, VSCP_LAMP_STATE_ON);

    /* Node shall be in active state */
    CU_ASSERT_EQUAL(vscp_core_isActive(), TRUE);

    return;
}

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Send segment controller heartbeat event.
 *
 * Expectation:
 *  - Node stays in active mode.
 */
extern void vscp_test_active02(void)
{
    vscp_test_initTestCase();

    /* Send probe acknowledge */
    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_PROTOCOL;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_PROTOCOL_SEGMENT_CONTROLLER_HEARTBEAT;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = VSCP_NICKNAME_SEGMENT_MASTER;
    vscp_test_rxMessage.hardCoded   = FALSE;
    vscp_test_rxMessage.dataNum     = 5;
    vscp_test_rxMessage.data[0]     = VSCP_TEST_SEGMENT_CRC;
    vscp_test_rxMessage.data[1]     = (VSCP_TEST_TIME_SINCE_EPOCH >> 24) & 0xff;    /* Time since epoch */
    vscp_test_rxMessage.data[2]     = (VSCP_TEST_TIME_SINCE_EPOCH >> 16) & 0xff;    /* Time since epoch */
    vscp_test_rxMessage.data[3]     = (VSCP_TEST_TIME_SINCE_EPOCH >>  8) & 0xff;    /* Time since epoch */
    vscp_test_rxMessage.data[4]     = (VSCP_TEST_TIME_SINCE_EPOCH >>  0) & 0xff;    /* Time since epoch */

    vscp_test_waitForTxMessage(1, 10);

    /* Node shall send nothing. */
    CU_ASSERT_EQUAL(vscp_test_txMessageCnt, 0);

    /* Nickname shall not be overwritten.  */
    CU_ASSERT_EQUAL(vscp_ps_readNicknameId(), VSCP_TEST_NICKNAME);

    /* Node shall store the segment CRC. */
    CU_ASSERT_EQUAL(vscp_ps_readSegmentControllerCRC(), VSCP_TEST_SEGMENT_CRC);

    /* Node shall store the time since epoch */
    CU_ASSERT_EQUAL(vscp_core_getTimeSinceEpoch(), VSCP_TEST_TIME_SINCE_EPOCH);

    return;
}

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Send segment controller heartbeat event with new time since epoch.
 *
 * Expectation:
 *  - Node stays in active mode.
 *  - Node updates time since epoch.
 */
extern void vscp_test_active02_1(void)
{
    uint32_t    timeSinceEpoch = VSCP_TEST_TIME_SINCE_EPOCH + 10;

    vscp_test_initTestCase();

    /* Send probe acknowledge */
    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_PROTOCOL;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_PROTOCOL_SEGMENT_CONTROLLER_HEARTBEAT;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = VSCP_NICKNAME_SEGMENT_MASTER;
    vscp_test_rxMessage.hardCoded   = FALSE;
    vscp_test_rxMessage.dataNum     = 5;
    vscp_test_rxMessage.data[0]     = VSCP_TEST_SEGMENT_CRC;
    vscp_test_rxMessage.data[1]     = (timeSinceEpoch >> 24) & 0xff;    /* Time since epoch */
    vscp_test_rxMessage.data[2]     = (timeSinceEpoch >> 16) & 0xff;    /* Time since epoch */
    vscp_test_rxMessage.data[3]     = (timeSinceEpoch >>  8) & 0xff;    /* Time since epoch */
    vscp_test_rxMessage.data[4]     = (timeSinceEpoch >>  0) & 0xff;    /* Time since epoch */

    vscp_test_waitForTxMessage(1, 10);

    /* Node shall send nothing. */
    CU_ASSERT_EQUAL(vscp_test_txMessageCnt, 0);

    /* Nickname shall not be overwritten.  */
    CU_ASSERT_EQUAL(vscp_ps_readNicknameId(), VSCP_TEST_NICKNAME);

    /* Node shall store the segment CRC. */
    CU_ASSERT_EQUAL(vscp_ps_readSegmentControllerCRC(), VSCP_TEST_SEGMENT_CRC);

    /* Node shall store the time since epoch */
    CU_ASSERT_EQUAL(vscp_core_getTimeSinceEpoch(), timeSinceEpoch);

    return;
}

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Send segment controller heartbeat event with different CRC.
 *
 * Expectation:
 *  - Node changes to initialization mode.
 */
extern void vscp_test_active03(void)
{
    vscp_test_initTestCase();

    /* Send probe acknowledge */
    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_PROTOCOL;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_PROTOCOL_SEGMENT_CONTROLLER_HEARTBEAT;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = VSCP_NICKNAME_SEGMENT_MASTER;
    vscp_test_rxMessage.hardCoded   = FALSE;
    vscp_test_rxMessage.dataNum     = 5;
    vscp_test_rxMessage.data[0]     = 0x00; /* Segment CRC */
    vscp_test_rxMessage.data[1]     = 0x00; /* Time since epoch */
    vscp_test_rxMessage.data[2]     = 0x00; /* Time since epoch */
    vscp_test_rxMessage.data[3]     = 0x00; /* Time since epoch */
    vscp_test_rxMessage.data[4]     = 0x00; /* Time since epoch */

    vscp_test_waitForTxMessage(1, 10);

    /* Node shall send probe event. */
    CU_ASSERT_EQUAL(vscp_test_txMessageCnt, 1);

    /* Probe event shall be sent to segment master */
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpClass, VSCP_CLASS_L1_PROTOCOL);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpType, VSCP_TYPE_PROTOCOL_NEW_NODE_ONLINE);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].oAddr, VSCP_NICKNAME_NOT_INIT);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].hardCoded, FALSE);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].priority, VSCP_PRIORITY_7_LOW);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].dataNum, 1);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[0], VSCP_NICKNAME_SEGMENT_MASTER);

    /* Nickname shall be cleared.  */
    CU_ASSERT_EQUAL(vscp_ps_readNicknameId(), VSCP_NICKNAME_NOT_INIT);

    /* Node shall store the segment CRC. */
    CU_ASSERT_EQUAL(vscp_ps_readSegmentControllerCRC(), 0x00);

    /* Node shall store the time since epoch */
    CU_ASSERT_EQUAL(vscp_core_getTimeSinceEpoch(), 0);

    return;
}

/**
 * Precondition:
 *  - Probe event is sent to the segment master.
 *
 * Action:
 *  - Proceed nickname disocvery.
 *
 * Expectation:
 *  - Node changes to active state.
 */
extern void vscp_test_active04(void)
{
    vscp_test_initTestCase();

    /* Send probe acknowledge */
    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_PROTOCOL;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_PROTOCOL_PROBE_ACK;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = VSCP_NICKNAME_SEGMENT_MASTER;
    vscp_test_rxMessage.hardCoded   = FALSE;
    vscp_test_rxMessage.dataNum     = 0;

    /* Process core */
    vscp_test_waitForTxMessage(1, 10);

    /* No event shall be sent. */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_TP_ADAPTER_WRITE_MESSAGE], 0);

    /* Timer is not stopped! */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_PORTABLE_STOP_TIMER], 0);

    /* Set new nickname */
    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_PROTOCOL;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_PROTOCOL_SET_NICKNAME_ID;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_3_NORMAL;
    vscp_test_rxMessage.oAddr       = VSCP_NICKNAME_SEGMENT_MASTER;
    vscp_test_rxMessage.hardCoded   = FALSE;
    vscp_test_rxMessage.dataNum     = 2;
    vscp_test_rxMessage.data[0]     = VSCP_NICKNAME_NOT_INIT;
    vscp_test_rxMessage.data[1]     = VSCP_TEST_NICKNAME;

    /* Process core */
    vscp_test_waitForTxMessage(2, 2);

    /* Nickname is stored */
    CU_ASSERT_EQUAL(vscp_ps_readNicknameId(), VSCP_TEST_NICKNAME);

    /* Timer is stopped */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_PORTABLE_STOP_TIMER], 1);

    /* Nickname accepted shall be sent. */
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpClass, VSCP_CLASS_L1_PROTOCOL);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpType, VSCP_TYPE_PROTOCOL_NICKNAME_ID_ACCEPTED);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].oAddr, VSCP_TEST_NICKNAME);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].hardCoded, FALSE);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].dataNum, 0);

    /* New node online shall be sent. */
    CU_ASSERT_EQUAL(vscp_test_txMessage[1].vscpClass, VSCP_CLASS_L1_PROTOCOL);
    CU_ASSERT_EQUAL(vscp_test_txMessage[1].vscpType, VSCP_TYPE_PROTOCOL_NEW_NODE_ONLINE);
    CU_ASSERT_EQUAL(vscp_test_txMessage[1].oAddr, VSCP_TEST_NICKNAME);
    CU_ASSERT_EQUAL(vscp_test_txMessage[1].hardCoded, FALSE);
    CU_ASSERT_EQUAL(vscp_test_txMessage[1].dataNum, 1);
    CU_ASSERT_EQUAL(vscp_test_txMessage[1].data[0], VSCP_TEST_NICKNAME);

    return;
}

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Send new node online / probe event (equal nickname id )
 *
 * Expectation:
 *  - Node sends probe acknowledge.
 */
extern void vscp_test_active05(void)
{
    vscp_test_initTestCase();

    /* Send probe acknowledge */
    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_PROTOCOL;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_PROTOCOL_NEW_NODE_ONLINE;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = VSCP_TEST_NICKNAME;
    vscp_test_rxMessage.hardCoded   = FALSE;
    vscp_test_rxMessage.dataNum     = 1;
    vscp_test_rxMessage.data[0]     = VSCP_TEST_NICKNAME;

    vscp_test_waitForTxMessage(1, 10);

    /* Node shall send probe acknowledge. */
    CU_ASSERT_EQUAL(vscp_test_txMessageCnt, 1);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpClass, VSCP_CLASS_L1_PROTOCOL);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpType, VSCP_TYPE_PROTOCOL_PROBE_ACK);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].oAddr, VSCP_TEST_NICKNAME);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].hardCoded, FALSE);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].dataNum, 0);

    return;
}

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Send new node online / probe event (different nickname id )
 *
 * Expectation:
 *  - Node do nothing.
 */
extern void vscp_test_active06(void)
{
    vscp_test_initTestCase();

    /* Send probe acknowledge */
    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_PROTOCOL;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_PROTOCOL_NEW_NODE_ONLINE;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = VSCP_TEST_NICKNAME_REMOTE;
    vscp_test_rxMessage.hardCoded   = FALSE;
    vscp_test_rxMessage.dataNum     = 1;
    vscp_test_rxMessage.data[0]     = VSCP_TEST_NICKNAME_REMOTE;

    vscp_test_waitForTxMessage(1, 10);

    /* Node shall send nothing. */
    CU_ASSERT_EQUAL(vscp_test_txMessageCnt, 0);

    return;
}

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Send probe ACK.
 *
 * Expectation:
 *  - Node do nothing.
 */
extern void vscp_test_active07(void)
{
    vscp_test_initTestCase();

    /* Send probe acknowledge */
    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_PROTOCOL;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_PROTOCOL_PROBE_ACK;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = VSCP_TEST_NICKNAME_REMOTE;
    vscp_test_rxMessage.hardCoded   = FALSE;
    vscp_test_rxMessage.dataNum     = 0;

    vscp_test_waitForTxMessage(1, 10);

    /* Node shall send nothing. */
    CU_ASSERT_EQUAL(vscp_test_txMessageCnt, 0);

    return;
}

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Set nickname id for node
 *
 * Expectation:
 *  - Node accepts new nickname id
 */
extern void vscp_test_active08(void)
{
    vscp_test_initTestCase();

    /* Send probe acknowledge */
    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_PROTOCOL;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_PROTOCOL_SET_NICKNAME_ID;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = VSCP_NICKNAME_SEGMENT_MASTER;
    vscp_test_rxMessage.hardCoded   = FALSE;
    vscp_test_rxMessage.dataNum     = 2;
    vscp_test_rxMessage.data[0]     = VSCP_TEST_NICKNAME;
    vscp_test_rxMessage.data[1]     = VSCP_TEST_NICKNAME_REMOTE;

    vscp_test_waitForTxMessage(1, 10);

    /* Node shall send probe acknowledge. */
    CU_ASSERT_EQUAL(vscp_test_txMessageCnt, 1);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpClass, VSCP_CLASS_L1_PROTOCOL);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpType, VSCP_TYPE_PROTOCOL_NICKNAME_ID_ACCEPTED);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].oAddr, VSCP_TEST_NICKNAME_REMOTE);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].hardCoded, FALSE);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].dataNum, 0);

    /* New nickname shall be set */
    CU_ASSERT_EQUAL(vscp_ps_readNicknameId(), VSCP_TEST_NICKNAME_REMOTE);

    return;
}

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Set illegal nickname id for node
 *
 * Expectation:
 *  - Node doesn't accept nickname (sends nothing)
 */
extern void vscp_test_active09(void)
{
    vscp_test_initTestCase();

    /* Send probe acknowledge */
    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_PROTOCOL;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_PROTOCOL_SET_NICKNAME_ID;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = VSCP_NICKNAME_SEGMENT_MASTER;
    vscp_test_rxMessage.hardCoded   = FALSE;
    vscp_test_rxMessage.dataNum     = 2;
    vscp_test_rxMessage.data[0]     = VSCP_TEST_NICKNAME_REMOTE;
    vscp_test_rxMessage.data[1]     = VSCP_NICKNAME_SEGMENT_MASTER;

    vscp_test_waitForTxMessage(1, 10);

    /* Node shall send nothing. */
    CU_ASSERT_EQUAL(vscp_test_txMessageCnt, 0);

    /* New nickname shall be set */
    CU_ASSERT_EQUAL(vscp_ps_readNicknameId(), VSCP_TEST_NICKNAME_REMOTE);

    return;
}

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Drop nickname
 *
 * Expectation:
 *  - Node restarts in initialization state
 */
extern void vscp_test_active10(void)
{
    vscp_test_initTestCase();

    /* Send probe acknowledge */
    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_PROTOCOL;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_PROTOCOL_DROP_NICKNAME_ID;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = VSCP_NICKNAME_SEGMENT_MASTER;
    vscp_test_rxMessage.hardCoded   = FALSE;
    vscp_test_rxMessage.dataNum     = 1;
    vscp_test_rxMessage.data[0]     = VSCP_TEST_NICKNAME_REMOTE;

    vscp_test_waitForTxMessage(1, 10);

    /* Node shall send nothing. */
    CU_ASSERT_EQUAL(vscp_test_txMessageCnt, 0);

    /* New nickname shall be cleared */
    CU_ASSERT_EQUAL(vscp_ps_readNicknameId(), VSCP_NICKNAME_NOT_INIT);

    /* Node shall request reset */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_PORTABLE_RESET_REQUEST], 1);

    return;
}

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Reset device, keep nickname
 *
 * Expectation:
 *  - Node restarts normal
 */
extern void vscp_test_active11(void)
{
    vscp_test_initTestCase();

    /* Send probe acknowledge */
    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_PROTOCOL;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_PROTOCOL_DROP_NICKNAME_ID;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = VSCP_NICKNAME_SEGMENT_MASTER;
    vscp_test_rxMessage.hardCoded   = FALSE;
    vscp_test_rxMessage.dataNum     = 2;
    vscp_test_rxMessage.data[0]     = VSCP_TEST_NICKNAME;
    vscp_test_rxMessage.data[1]     = 0x20;

    vscp_test_waitForTxMessage(1, 10);

    /* Node shall send nothing. */
    CU_ASSERT_EQUAL(vscp_test_txMessageCnt, 0);

    /* New nickname shall not be cleared */
    CU_ASSERT_EQUAL(vscp_ps_readNicknameId(), VSCP_TEST_NICKNAME);

    /* Node shall request reset */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_PORTABLE_RESET_REQUEST], 1);

    return;
}

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Go idle, do not start up again
 *
 * Expectation:
 *  - Node is in idle state
 */
extern void vscp_test_active12(void)
{
    vscp_test_initTestCase();

    /* Send probe acknowledge */
    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_PROTOCOL;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_PROTOCOL_DROP_NICKNAME_ID;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = VSCP_NICKNAME_SEGMENT_MASTER;
    vscp_test_rxMessage.hardCoded   = FALSE;
    vscp_test_rxMessage.dataNum     = 2;
    vscp_test_rxMessage.data[0]     = VSCP_TEST_NICKNAME;
    vscp_test_rxMessage.data[1]     = 0x80;

    vscp_test_waitForTxMessage(1, 10);

    /* Node shall send nothing. */
    CU_ASSERT_EQUAL(vscp_test_txMessageCnt, 0);

    /* New nickname shall be cleared */
    CU_ASSERT_EQUAL(vscp_ps_readNicknameId(), VSCP_NICKNAME_NOT_INIT);

    /* Node shall not request reset */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_PORTABLE_RESET_REQUEST], 0);

    /* Node shall be entered idle state */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_PORTABLE_IDLE_STATE_ENTERED], 1);

    return;
}

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Set persistent memory defaults
 *
 * Expectation:
 *  - Node set persistent memory defaults.
 *  - Node resets
 */
extern void vscp_test_active13(void)
{
    vscp_test_initTestCase();

    /* Send probe acknowledge */
    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_PROTOCOL;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_PROTOCOL_DROP_NICKNAME_ID;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = VSCP_NICKNAME_SEGMENT_MASTER;
    vscp_test_rxMessage.hardCoded   = FALSE;
    vscp_test_rxMessage.dataNum     = 2;
    vscp_test_rxMessage.data[0]     = VSCP_TEST_NICKNAME;
    vscp_test_rxMessage.data[1]     = 0x40;

    vscp_test_waitForTxMessage(1, 10);

    /* Node shall send nothing. */
    CU_ASSERT_EQUAL(vscp_test_txMessageCnt, 0);

    /* New nickname shall be cleared */
    CU_ASSERT_EQUAL(vscp_ps_readNicknameId(), VSCP_NICKNAME_NOT_INIT);

    /* Node shall set persistent memory defaults */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_APP_RESTORE_FACTORY_DEFAULTS], 1);

    /* Node shall request reset */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_PORTABLE_RESET_REQUEST], 1);

    /* Node shall no be entered idle state */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_PORTABLE_IDLE_STATE_ENTERED], 0);

    return;
}

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Reset after specific time and keep nickname
 *
 * Expectation:
 *  - Node keeps nickname
 *  - Node resets after specific time
 */
extern void vscp_test_active14(void)
{
    vscp_test_initTestCase();

    /* Send probe acknowledge */
    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_PROTOCOL;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_PROTOCOL_DROP_NICKNAME_ID;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = VSCP_NICKNAME_SEGMENT_MASTER;
    vscp_test_rxMessage.hardCoded   = FALSE;
    vscp_test_rxMessage.dataNum     = 3;
    vscp_test_rxMessage.data[0]     = VSCP_TEST_NICKNAME;
    vscp_test_rxMessage.data[1]     = 0x20;
    vscp_test_rxMessage.data[2]     = 1;

    vscp_test_waitForTxMessage(1, 1000);

    /* Node shall not request reset */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_PORTABLE_RESET_REQUEST], 0);

    vscp_test_waitForTxMessage(1, 1);

    /* Node shall send nothing. */
    CU_ASSERT_EQUAL(vscp_test_txMessageCnt, 0);

    /* New nickname shall not be cleared */
    CU_ASSERT_EQUAL(vscp_ps_readNicknameId(), VSCP_TEST_NICKNAME);

    /* Node shall not set persistent memory defaults */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_APP_RESTORE_FACTORY_DEFAULTS], 0);

    /* Node shall request reset */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_PORTABLE_RESET_REQUEST], 1);

    /* Node shall no be entered idle state */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_PORTABLE_IDLE_STATE_ENTERED], 0);

    return;
}

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Read all registers.
 *  - Write to all read/write registers.
 *  - Check read only registers.
 *  - Check write only registers.
 *
 * Expectation:
 *  - All registers can be read and return the correct value.
 *  - All read/write registers can be written.
 *  - All read only registers can not be written.
 */
extern void vscp_test_active15(void)
{
    uint8_t index   = 0;
    uint8_t value   = 0;

    unsigned int    numFailures = CU_get_number_of_failures();
    unsigned int    tmp         = numFailures;

    for(index = 0; index < VSCP_TEST_ARRAY_NUM(vscp_test_registerTestTable); ++index)
    {
        /* Read/Write? */
        if (VSCP_TEST_REG_ACCESS_READ_WRITE == vscp_test_registerTestTable[index].access)
        {
            numFailures = CU_get_number_of_failures();

            /* First write the value */
            value = vscp_test_writeRegister(vscp_test_registerTestTable[index].addr, vscp_test_registerTestTable[index].value, FALSE);

            CU_ASSERT_EQUAL(vscp_test_registerTestTable[index].value, value);

            tmp = CU_get_number_of_failures();
            if (tmp != numFailures)
            {
                printf("Write 0x%02X to register %s failed.\n", value, vscp_test_registerTestTable[index].description);
                numFailures = tmp;
            }
        }
        /* Read only? */
        else if (VSCP_TEST_REG_ACCESS_READ_ONLY == vscp_test_registerTestTable[index].access)
        {
            numFailures = CU_get_number_of_failures();

            /* Check if it is really read only. */
            (void)vscp_test_writeRegister(vscp_test_registerTestTable[index].addr, vscp_test_registerTestTable[index].value, TRUE);

            tmp = CU_get_number_of_failures();
            if (tmp != numFailures)
            {
                printf("Check register %s for read only failed.\n", vscp_test_registerTestTable[index].description);
                numFailures = tmp;
            }
        }
        else
        /* Write only */
        {
            numFailures = CU_get_number_of_failures();

            /* First write the value */
            value = vscp_test_writeRegister(vscp_test_registerTestTable[index].addr, vscp_test_registerTestTable[index].value, FALSE);

            CU_ASSERT_EQUAL(vscp_test_registerTestTable[index].value, value);

            tmp = CU_get_number_of_failures();
            if (tmp != numFailures)
            {
                printf("Write 0x%02X to register %s failed.\n", value, vscp_test_registerTestTable[index].description);
                numFailures = tmp;
            }
        }

        value = vscp_test_registerTestTable[index].value;

        if (NULL != vscp_test_registerTestTable[index].getExpectedValue)
        {
            value = vscp_test_registerTestTable[index].getExpectedValue(value);
        }

        numFailures = CU_get_number_of_failures();

        if (vscp_test_readRegister(vscp_test_registerTestTable[index].addr) != value)
        {
            printf("\nFailed in register test table index %u.\n", index);
            CU_ASSERT_EQUAL(vscp_test_readRegister(vscp_test_registerTestTable[index].addr), value);
        }

        tmp = CU_get_number_of_failures();
        if (tmp != numFailures)
        {
            printf("Read from register %s failed.\n", vscp_test_registerTestTable[index].description);
            numFailures = tmp;
        }
    }

    return;
}

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Enter boot loader (wrong data)
 *
 * Expectation:
 *  - Node send a NACK and doesn't enter boot loader
 */
extern void vscp_test_active16(void)
{
    vscp_test_initTestCase();

    /* Send probe acknowledge */
    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_PROTOCOL;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_PROTOCOL_ENTER_BOOT_LOADER_MODE;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = VSCP_NICKNAME_SEGMENT_MASTER;
    vscp_test_rxMessage.hardCoded   = FALSE;
    vscp_test_rxMessage.dataNum     = 8;
    vscp_test_rxMessage.data[0]     = VSCP_TEST_NICKNAME;
    vscp_test_rxMessage.data[1]     = VSCP_TEST_BOOT_LOADER_ALGORITHM + 1;  /* Wrong algorithm */
    vscp_test_rxMessage.data[2]     = (VSCP_TEST_GUID_3 >> 24) & 0xff;  /* Byte 0 */
    vscp_test_rxMessage.data[3]     = (VSCP_TEST_GUID_3 >>  0) & 0xff;  /* Byte 3 */
    vscp_test_rxMessage.data[4]     = (VSCP_TEST_GUID_2 >> 16) & 0xff;  /* Byte 5 */
    vscp_test_rxMessage.data[5]     = (VSCP_TEST_GUID_2 >>  0) & 0xff;  /* Byte 7 */
    vscp_test_rxMessage.data[6]     = 0;
    vscp_test_rxMessage.data[7]     = 0;

    vscp_test_waitForTxMessage(1, 10);

    /* Node shall send NACK */
    CU_ASSERT_EQUAL(vscp_test_txMessageCnt, 1);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpClass, VSCP_CLASS_L1_PROTOCOL);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpType, VSCP_TYPE_PROTOCOL_ENTER_BOOT_LOADER_MODE_NACK);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].oAddr, VSCP_TEST_NICKNAME);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].hardCoded, FALSE);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].dataNum, 1);

    /* Jump to boot loader shall not be requested. */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_PORTABLE_BOOT_LOADER_REQUEST], 0);

    return;
}

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Enter boot loader
 *
 * Expectation:
 *  - Boot loader shall be requested
 */
extern void vscp_test_active17(void)
{
    vscp_test_initTestCase();

    /* Send probe acknowledge */
    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_PROTOCOL;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_PROTOCOL_ENTER_BOOT_LOADER_MODE;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = VSCP_NICKNAME_SEGMENT_MASTER;
    vscp_test_rxMessage.hardCoded   = FALSE;
    vscp_test_rxMessage.dataNum     = 8;
    vscp_test_rxMessage.data[0]     = VSCP_TEST_NICKNAME;
    vscp_test_rxMessage.data[1]     = VSCP_TEST_BOOT_LOADER_ALGORITHM;
    vscp_test_rxMessage.data[2]     = (VSCP_TEST_GUID_3 >> 24) & 0xff;  /* Byte 0 */
    vscp_test_rxMessage.data[3]     = (VSCP_TEST_GUID_3 >>  0) & 0xff;  /* Byte 3 */
    vscp_test_rxMessage.data[4]     = (VSCP_TEST_GUID_2 >> 16) & 0xff;  /* Byte 5 */
    vscp_test_rxMessage.data[5]     = (VSCP_TEST_GUID_2 >>  0) & 0xff;  /* Byte 7 */
    vscp_test_rxMessage.data[6]     = 0;
    vscp_test_rxMessage.data[7]     = 0;

    vscp_test_waitForTxMessage(1, 10);

    /* Node shall send nothing, because in the real system the response will be
     * sent by the boot loader.
     */
    CU_ASSERT_EQUAL(vscp_test_txMessageCnt, 0);

    /* Jump to boot loader shall be requested. */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_PORTABLE_BOOT_LOADER_REQUEST], 1);

    return;
}

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - GUID drop nickname
 *
 * Expectation:
 *  - Node is in initialization state
 */
extern void vscp_test_active18(void)
{
    vscp_test_initTestCase();

    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_PROTOCOL;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_PROTOCOL_GUID_DROP_NICKNAME_ID;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = VSCP_NICKNAME_SEGMENT_MASTER;
    vscp_test_rxMessage.hardCoded   = FALSE;
    vscp_test_rxMessage.dataNum     = 5;
    vscp_test_rxMessage.data[0]     = 0;
    vscp_test_rxMessage.data[1]     = (VSCP_TEST_GUID_3 >> 24) & 0xff;
    vscp_test_rxMessage.data[2]     = (VSCP_TEST_GUID_3 >> 16) & 0xff;
    vscp_test_rxMessage.data[3]     = (VSCP_TEST_GUID_3 >>  8) & 0xff;
    vscp_test_rxMessage.data[4]     = (VSCP_TEST_GUID_3 >>  0) & 0xff;

    vscp_test_waitForTxMessage(1, 10);

    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_PROTOCOL;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_PROTOCOL_GUID_DROP_NICKNAME_ID;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = VSCP_NICKNAME_SEGMENT_MASTER;
    vscp_test_rxMessage.hardCoded   = FALSE;
    vscp_test_rxMessage.dataNum     = 5;
    vscp_test_rxMessage.data[0]     = 1;
    vscp_test_rxMessage.data[1]     = (VSCP_TEST_GUID_2 >> 24) & 0xff;
    vscp_test_rxMessage.data[2]     = (VSCP_TEST_GUID_2 >> 16) & 0xff;
    vscp_test_rxMessage.data[3]     = (VSCP_TEST_GUID_2 >>  8) & 0xff;
    vscp_test_rxMessage.data[4]     = (VSCP_TEST_GUID_2 >>  0) & 0xff;

    vscp_test_waitForTxMessage(1, 10);

    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_PROTOCOL;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_PROTOCOL_GUID_DROP_NICKNAME_ID;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = VSCP_NICKNAME_SEGMENT_MASTER;
    vscp_test_rxMessage.hardCoded   = FALSE;
    vscp_test_rxMessage.dataNum     = 5;
    vscp_test_rxMessage.data[0]     = 2;
    vscp_test_rxMessage.data[1]     = (VSCP_TEST_GUID_1 >> 24) & 0xff;
    vscp_test_rxMessage.data[2]     = (VSCP_TEST_GUID_1 >> 16) & 0xff;
    vscp_test_rxMessage.data[3]     = (VSCP_TEST_GUID_1 >>  8) & 0xff;
    vscp_test_rxMessage.data[4]     = (VSCP_TEST_GUID_1 >>  0) & 0xff;

    vscp_test_waitForTxMessage(1, 10);

    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_PROTOCOL;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_PROTOCOL_GUID_DROP_NICKNAME_ID;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = VSCP_NICKNAME_SEGMENT_MASTER;
    vscp_test_rxMessage.hardCoded   = FALSE;
    vscp_test_rxMessage.dataNum     = 5;
    vscp_test_rxMessage.data[0]     = 3;
    vscp_test_rxMessage.data[1]     = (VSCP_TEST_GUID_0 >> 24) & 0xff;
    vscp_test_rxMessage.data[2]     = (VSCP_TEST_GUID_0 >> 16) & 0xff;
    vscp_test_rxMessage.data[3]     = (VSCP_TEST_GUID_0 >>  8) & 0xff;
    vscp_test_rxMessage.data[4]     = (VSCP_TEST_GUID_0 >>  0) & 0xff;

    vscp_test_waitForTxMessage(1, 10);

    /* Node shall send nothing. */
    CU_ASSERT_EQUAL(vscp_test_txMessageCnt, 0);

    /* New nickname shall be cleared */
    CU_ASSERT_EQUAL(vscp_ps_readNicknameId(), VSCP_NICKNAME_NOT_INIT);

    /* Node shall request reset */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_PORTABLE_RESET_REQUEST], 1);

    return;
}

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Read page (4 byte)
 *
 * Expectation:
 *  - Page shall be read
 */
extern void vscp_test_active19(void)
{
    vscp_test_initTestCase();

    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_PROTOCOL;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_PROTOCOL_PAGE_READ;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = VSCP_NICKNAME_SEGMENT_MASTER;
    vscp_test_rxMessage.hardCoded   = FALSE;
    vscp_test_rxMessage.dataNum     = 3;
    vscp_test_rxMessage.data[0]     = VSCP_TEST_NICKNAME;
    vscp_test_rxMessage.data[1]     = VSCP_REG_MANUFACTURER_DEV_ID_0;
    vscp_test_rxMessage.data[2]     = 4;

    vscp_test_waitForTxMessage(1, 10);

    /* Node shall send a responses */
    CU_ASSERT_EQUAL(vscp_test_txMessageCnt, 1);

    CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpClass, VSCP_CLASS_L1_PROTOCOL);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpType, VSCP_TYPE_PROTOCOL_PAGE_READ_WRITE_RESPONSE);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].oAddr, VSCP_TEST_NICKNAME);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].hardCoded, FALSE);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].dataNum, 5);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[0], 0);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[1], (VSCP_TEST_MANUFACTURER_DEV_ID >>  0) & 0xff);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[2], (VSCP_TEST_MANUFACTURER_DEV_ID >>  8) & 0xff);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[3], (VSCP_TEST_MANUFACTURER_DEV_ID >> 16) & 0xff);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[4], (VSCP_TEST_MANUFACTURER_DEV_ID >> 24) & 0xff);

    return;
}

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Read page (7 byte)
 *
 * Expectation:
 *  - Page shall be read
 */
extern void vscp_test_active20(void)
{
    vscp_test_initTestCase();

    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_PROTOCOL;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_PROTOCOL_PAGE_READ;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = VSCP_NICKNAME_SEGMENT_MASTER;
    vscp_test_rxMessage.hardCoded   = FALSE;
    vscp_test_rxMessage.dataNum     = 3;
    vscp_test_rxMessage.data[0]     = VSCP_TEST_NICKNAME;
    vscp_test_rxMessage.data[1]     = VSCP_REG_MANUFACTURER_DEV_ID_0;
    vscp_test_rxMessage.data[2]     = 7;

    vscp_test_waitForTxMessage(1, 10);

    /* Node shall send a responses */
    CU_ASSERT_EQUAL(vscp_test_txMessageCnt, 1);

    CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpClass, VSCP_CLASS_L1_PROTOCOL);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpType, VSCP_TYPE_PROTOCOL_PAGE_READ_WRITE_RESPONSE);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].oAddr, VSCP_TEST_NICKNAME);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].hardCoded, FALSE);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].dataNum, 8);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[0], 0);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[1], (VSCP_TEST_MANUFACTURER_DEV_ID >>  0) & 0xff);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[2], (VSCP_TEST_MANUFACTURER_DEV_ID >>  8) & 0xff);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[3], (VSCP_TEST_MANUFACTURER_DEV_ID >> 16) & 0xff);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[4], (VSCP_TEST_MANUFACTURER_DEV_ID >> 24) & 0xff);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[5], (VSCP_TEST_MANUFACTURER_SUB_DEV_ID >>  0) & 0xff);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[6], (VSCP_TEST_MANUFACTURER_SUB_DEV_ID >>  8) & 0xff);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[7], (VSCP_TEST_MANUFACTURER_SUB_DEV_ID >> 16) & 0xff);

    return;
}

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Read page (8 byte)
 *
 * Expectation:
 *  - Page shall be read
 */
extern void vscp_test_active21(void)
{
    vscp_test_initTestCase();

    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_PROTOCOL;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_PROTOCOL_PAGE_READ;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = VSCP_NICKNAME_SEGMENT_MASTER;
    vscp_test_rxMessage.hardCoded   = FALSE;
    vscp_test_rxMessage.dataNum     = 3;
    vscp_test_rxMessage.data[0]     = VSCP_TEST_NICKNAME;
    vscp_test_rxMessage.data[1]     = VSCP_REG_MANUFACTURER_DEV_ID_0;
    vscp_test_rxMessage.data[2]     = 8;

    vscp_test_waitForTxMessage(2, 10);

    /* Node shall send a responses */
    CU_ASSERT_EQUAL(vscp_test_txMessageCnt, 2);

    CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpClass, VSCP_CLASS_L1_PROTOCOL);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpType, VSCP_TYPE_PROTOCOL_PAGE_READ_WRITE_RESPONSE);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].oAddr, VSCP_TEST_NICKNAME);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].hardCoded, FALSE);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].dataNum, 8);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[0], 0);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[1], (VSCP_TEST_MANUFACTURER_DEV_ID >>  0) & 0xff);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[2], (VSCP_TEST_MANUFACTURER_DEV_ID >>  8) & 0xff);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[3], (VSCP_TEST_MANUFACTURER_DEV_ID >> 16) & 0xff);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[4], (VSCP_TEST_MANUFACTURER_DEV_ID >> 24) & 0xff);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[5], (VSCP_TEST_MANUFACTURER_SUB_DEV_ID >>  0) & 0xff);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[6], (VSCP_TEST_MANUFACTURER_SUB_DEV_ID >>  8) & 0xff);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[7], (VSCP_TEST_MANUFACTURER_SUB_DEV_ID >> 16) & 0xff);

    CU_ASSERT_EQUAL(vscp_test_txMessage[1].vscpClass, VSCP_CLASS_L1_PROTOCOL);
    CU_ASSERT_EQUAL(vscp_test_txMessage[1].vscpType, VSCP_TYPE_PROTOCOL_PAGE_READ_WRITE_RESPONSE);
    CU_ASSERT_EQUAL(vscp_test_txMessage[1].oAddr, VSCP_TEST_NICKNAME);
    CU_ASSERT_EQUAL(vscp_test_txMessage[1].hardCoded, FALSE);
    CU_ASSERT_EQUAL(vscp_test_txMessage[1].dataNum, 2);
    CU_ASSERT_EQUAL(vscp_test_txMessage[1].data[0], 1);
    CU_ASSERT_EQUAL(vscp_test_txMessage[1].data[1], (VSCP_TEST_MANUFACTURER_SUB_DEV_ID >> 24) & 0xff);

    return;
}

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Write page (4 byte)
 *
 * Expectation:
 *  - Page shall be read
 */
extern void vscp_test_active22(void)
{
    vscp_test_initTestCase();

    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_PROTOCOL;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_PROTOCOL_PAGE_WRITE;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = VSCP_NICKNAME_SEGMENT_MASTER;
    vscp_test_rxMessage.hardCoded   = FALSE;
    vscp_test_rxMessage.dataNum     = 6;
    vscp_test_rxMessage.data[0]     = VSCP_TEST_NICKNAME;
    vscp_test_rxMessage.data[1]     = VSCP_REG_USER_ID_0;
    vscp_test_rxMessage.data[2]     = 0x21;
    vscp_test_rxMessage.data[3]     = 0x32;
    vscp_test_rxMessage.data[4]     = 0x43;
    vscp_test_rxMessage.data[5]     = 0x54;

    vscp_test_waitForTxMessage(1, 10);

    /* Node shall send a response */
    CU_ASSERT_EQUAL(vscp_test_txMessageCnt, 1);

    CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpClass, VSCP_CLASS_L1_PROTOCOL);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpType, VSCP_TYPE_PROTOCOL_PAGE_READ_WRITE_RESPONSE);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].oAddr, VSCP_TEST_NICKNAME);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].hardCoded, FALSE);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].dataNum, 5);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[0], 0);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[1], 0x21);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[2], 0x32);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[3], 0x43);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[4], 0x54);

    return;
}

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Increment register
 *
 * Expectation:
 *  - Register is incremented
 */
extern void vscp_test_active23(void)
{
    uint8_t value   = vscp_ps_readUserId(0);

    vscp_test_initTestCase();

    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_PROTOCOL;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_PROTOCOL_INCREMENT_REGISTER;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = VSCP_NICKNAME_SEGMENT_MASTER;
    vscp_test_rxMessage.hardCoded   = FALSE;
    vscp_test_rxMessage.dataNum     = 2;
    vscp_test_rxMessage.data[0]     = VSCP_TEST_NICKNAME;
    vscp_test_rxMessage.data[1]     = VSCP_REG_USER_ID_0;

    vscp_test_waitForTxMessage(1, 10);

    /* Node shall send a response */
    CU_ASSERT_EQUAL(vscp_test_txMessageCnt, 1);

    CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpClass, VSCP_CLASS_L1_PROTOCOL);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpType, VSCP_TYPE_PROTOCOL_READ_WRITE_RESPONSE);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].oAddr, VSCP_TEST_NICKNAME);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].hardCoded, FALSE);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].dataNum, 2);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[0], VSCP_REG_USER_ID_0);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[1], value + 1);

    return;
}

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Decrement register
 *
 * Expectation:
 *  - Register is decremented
 */
extern void vscp_test_active24(void)
{
    uint8_t value   = vscp_ps_readUserId(0);

    vscp_test_initTestCase();

    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_PROTOCOL;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_PROTOCOL_DECREMENT_REGISTER;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = VSCP_NICKNAME_SEGMENT_MASTER;
    vscp_test_rxMessage.hardCoded   = FALSE;
    vscp_test_rxMessage.dataNum     = 2;
    vscp_test_rxMessage.data[0]     = VSCP_TEST_NICKNAME;
    vscp_test_rxMessage.data[1]     = VSCP_REG_USER_ID_0;

    vscp_test_waitForTxMessage(1, 10);

    /* Node shall send a response */
    CU_ASSERT_EQUAL(vscp_test_txMessageCnt, 1);

    CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpClass, VSCP_CLASS_L1_PROTOCOL);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpType, VSCP_TYPE_PROTOCOL_READ_WRITE_RESPONSE);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].oAddr, VSCP_TEST_NICKNAME);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].hardCoded, FALSE);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].dataNum, 2);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[0], VSCP_REG_USER_ID_0);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[1], value - 1);

    return;
}

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Who is there? (all)
 *
 * Expectation:
 *  - Response ...
 */
extern void vscp_test_active25(void)
{
    uint8_t index    = 0;

    vscp_test_initTestCase();

    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_PROTOCOL;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_PROTOCOL_WHO_IS_THERE;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = VSCP_NICKNAME_SEGMENT_MASTER;
    vscp_test_rxMessage.hardCoded   = FALSE;
    vscp_test_rxMessage.dataNum     = 1;
    vscp_test_rxMessage.data[0]     = 0xFF;

    vscp_test_waitForTxMessage(7, 10);

    /* Node shall send a response */
    CU_ASSERT_EQUAL(vscp_test_txMessageCnt, 7);

    for(index = 0; index < 7; ++index)
    {
        CU_ASSERT_EQUAL(vscp_test_txMessage[index].vscpClass, VSCP_CLASS_L1_PROTOCOL);
        CU_ASSERT_EQUAL(vscp_test_txMessage[index].vscpType, VSCP_TYPE_PROTOCOL_WHO_IS_THERE_RESPONSE);
        CU_ASSERT_EQUAL(vscp_test_txMessage[index].oAddr, VSCP_TEST_NICKNAME);
        CU_ASSERT_EQUAL(vscp_test_txMessage[index].hardCoded, FALSE);
        CU_ASSERT_EQUAL(vscp_test_txMessage[index].dataNum, 8);
        CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[0], index);

        switch(index)
        {
        case 0:
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[1], vscp_dev_data_getGUID(15));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[2], vscp_dev_data_getGUID(14));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[3], vscp_dev_data_getGUID(13));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[4], vscp_dev_data_getGUID(12));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[5], vscp_dev_data_getGUID(11));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[6], vscp_dev_data_getGUID(10));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[7], vscp_dev_data_getGUID(9));
            break;

        case 1:
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[1], vscp_dev_data_getGUID(8));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[2], vscp_dev_data_getGUID(7));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[3], vscp_dev_data_getGUID(6));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[4], vscp_dev_data_getGUID(5));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[5], vscp_dev_data_getGUID(4));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[6], vscp_dev_data_getGUID(3));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[7], vscp_dev_data_getGUID(2));
            break;

        case 2:
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[1], vscp_dev_data_getGUID(1));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[2], vscp_dev_data_getGUID(0));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[3], vscp_test_portableGetMdfUrl(0));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[4], vscp_test_portableGetMdfUrl(1));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[5], vscp_test_portableGetMdfUrl(2));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[6], vscp_test_portableGetMdfUrl(3));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[7], vscp_test_portableGetMdfUrl(4));
            break;

        case 3:
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[1], vscp_test_portableGetMdfUrl(5));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[2], vscp_test_portableGetMdfUrl(6));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[3], vscp_test_portableGetMdfUrl(7));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[4], vscp_test_portableGetMdfUrl(8));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[5], vscp_test_portableGetMdfUrl(9));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[6], vscp_test_portableGetMdfUrl(10));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[7], vscp_test_portableGetMdfUrl(11));
            break;

        case 4:
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[1], vscp_test_portableGetMdfUrl(12));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[2], vscp_test_portableGetMdfUrl(13));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[3], vscp_test_portableGetMdfUrl(14));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[4], vscp_test_portableGetMdfUrl(15));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[5], vscp_test_portableGetMdfUrl(16));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[6], vscp_test_portableGetMdfUrl(17));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[7], vscp_test_portableGetMdfUrl(18));
            break;

        case 5:
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[1], vscp_test_portableGetMdfUrl(19));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[2], vscp_test_portableGetMdfUrl(20));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[3], vscp_test_portableGetMdfUrl(21));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[4], vscp_test_portableGetMdfUrl(22));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[5], vscp_test_portableGetMdfUrl(23));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[6], vscp_test_portableGetMdfUrl(24));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[7], vscp_test_portableGetMdfUrl(25));
            break;

        case 6:
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[1], vscp_test_portableGetMdfUrl(26));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[2], vscp_test_portableGetMdfUrl(27));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[3], vscp_test_portableGetMdfUrl(28));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[4], vscp_test_portableGetMdfUrl(29));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[5], vscp_test_portableGetMdfUrl(30));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[6], vscp_test_portableGetMdfUrl(31));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[7], 0);
            break;

        default:
            break;
        }
    }

    return;
}

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Who is there? (node addressed)
 *
 * Expectation:
 *  - Response ...
 */
extern void vscp_test_active26(void)
{
    uint8_t index    = 0;

    vscp_test_initTestCase();

    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_PROTOCOL;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_PROTOCOL_WHO_IS_THERE;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = VSCP_NICKNAME_SEGMENT_MASTER;
    vscp_test_rxMessage.hardCoded   = FALSE;
    vscp_test_rxMessage.dataNum     = 1;
    vscp_test_rxMessage.data[0]     = VSCP_TEST_NICKNAME;

    vscp_test_waitForTxMessage(7, 10);

    /* Node shall send a response */
    CU_ASSERT_EQUAL(vscp_test_txMessageCnt, 7);

    for(index = 0; index < 7; ++index)
    {
        CU_ASSERT_EQUAL(vscp_test_txMessage[index].vscpClass, VSCP_CLASS_L1_PROTOCOL);
        CU_ASSERT_EQUAL(vscp_test_txMessage[index].vscpType, VSCP_TYPE_PROTOCOL_WHO_IS_THERE_RESPONSE);
        CU_ASSERT_EQUAL(vscp_test_txMessage[index].oAddr, VSCP_TEST_NICKNAME);
        CU_ASSERT_EQUAL(vscp_test_txMessage[index].hardCoded, FALSE);
        CU_ASSERT_EQUAL(vscp_test_txMessage[index].dataNum, 8);
        CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[0], index);

        switch(index)
        {
        case 0:
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[1], vscp_dev_data_getGUID(15));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[2], vscp_dev_data_getGUID(14));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[3], vscp_dev_data_getGUID(13));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[4], vscp_dev_data_getGUID(12));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[5], vscp_dev_data_getGUID(11));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[6], vscp_dev_data_getGUID(10));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[7], vscp_dev_data_getGUID(9));
            break;

        case 1:
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[1], vscp_dev_data_getGUID(8));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[2], vscp_dev_data_getGUID(7));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[3], vscp_dev_data_getGUID(6));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[4], vscp_dev_data_getGUID(5));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[5], vscp_dev_data_getGUID(4));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[6], vscp_dev_data_getGUID(3));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[7], vscp_dev_data_getGUID(2));
            break;

        case 2:
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[1], vscp_dev_data_getGUID(1));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[2], vscp_dev_data_getGUID(0));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[3], vscp_test_portableGetMdfUrl(0));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[4], vscp_test_portableGetMdfUrl(1));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[5], vscp_test_portableGetMdfUrl(2));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[6], vscp_test_portableGetMdfUrl(3));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[7], vscp_test_portableGetMdfUrl(4));
            break;

        case 3:
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[1], vscp_test_portableGetMdfUrl(5));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[2], vscp_test_portableGetMdfUrl(6));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[3], vscp_test_portableGetMdfUrl(7));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[4], vscp_test_portableGetMdfUrl(8));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[5], vscp_test_portableGetMdfUrl(9));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[6], vscp_test_portableGetMdfUrl(10));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[7], vscp_test_portableGetMdfUrl(11));
            break;

        case 4:
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[1], vscp_test_portableGetMdfUrl(12));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[2], vscp_test_portableGetMdfUrl(13));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[3], vscp_test_portableGetMdfUrl(14));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[4], vscp_test_portableGetMdfUrl(15));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[5], vscp_test_portableGetMdfUrl(16));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[6], vscp_test_portableGetMdfUrl(17));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[7], vscp_test_portableGetMdfUrl(18));
            break;

        case 5:
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[1], vscp_test_portableGetMdfUrl(19));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[2], vscp_test_portableGetMdfUrl(20));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[3], vscp_test_portableGetMdfUrl(21));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[4], vscp_test_portableGetMdfUrl(22));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[5], vscp_test_portableGetMdfUrl(23));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[6], vscp_test_portableGetMdfUrl(24));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[7], vscp_test_portableGetMdfUrl(25));
            break;

        case 6:
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[1], vscp_test_portableGetMdfUrl(26));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[2], vscp_test_portableGetMdfUrl(27));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[3], vscp_test_portableGetMdfUrl(28));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[4], vscp_test_portableGetMdfUrl(29));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[5], vscp_test_portableGetMdfUrl(30));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[6], vscp_test_portableGetMdfUrl(31));
            CU_ASSERT_EQUAL(vscp_test_txMessage[index].data[7], 0);
            break;

        default:
            break;
        }
    }

    return;
}

/**
 * Precondition:
 *  - VSCP is active.
 *  - Decision matrix exists
 *
 * Action:
 *  - Get decision matrix info
 *
 * Expectation:
 *  - Response ...
 */
extern void vscp_test_active28(void)
{
    vscp_test_initTestCase();

    /* Reset decision matrix */
    memset(vscp_test_dmStorage, 0, VSCP_PS_SIZE_DM);

    /* Reset decision matrix extension */
    memset(vscp_test_extStorage, 0, VSCP_PS_SIZE_DM_EXTENSION);

    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_PROTOCOL;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_PROTOCOL_GET_DECISION_MATRIX_INFO;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = VSCP_NICKNAME_SEGMENT_MASTER;
    vscp_test_rxMessage.hardCoded   = FALSE;
    vscp_test_rxMessage.dataNum     = 1;
    vscp_test_rxMessage.data[0]     = VSCP_TEST_NICKNAME;

    vscp_test_waitForTxMessage(1, 10);

    /* Node shall send a response */
    CU_ASSERT_EQUAL(vscp_test_txMessageCnt, 1);

    CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpClass, VSCP_CLASS_L1_PROTOCOL);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpType, VSCP_TYPE_PROTOCOL_GET_DECISION_MATRIX_INFO_RESPONSE);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].oAddr, VSCP_TEST_NICKNAME);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].hardCoded, FALSE);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].dataNum, 4);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[0], VSCP_CONFIG_DM_ROWS);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[1], vscp_test_dmOffset);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[2], (vscp_test_dmPage >> 8) & 0xff);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[3], (vscp_test_dmPage >> 0) & 0xff);

    return;
}

/**
 * Precondition:
 *  - VSCP is active.
 *  - Decision matrix exists
 *
 * Action:
 *  - Write data to decision matrix
 *
 * Expectation:
 *  - Response ...
 */
extern void vscp_test_active29(void)
{
    uint8_t index   = 0;

    vscp_test_initTestCase();

    /* Remove write protection */
    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_PROTOCOL;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_PROTOCOL_WRITE_REGISTER;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = VSCP_NICKNAME_SEGMENT_MASTER;
    vscp_test_rxMessage.hardCoded   = FALSE;
    vscp_test_rxMessage.dataNum     = 3;
    vscp_test_rxMessage.data[0]     = VSCP_TEST_NICKNAME;
    vscp_test_rxMessage.data[1]     = VSCP_REG_NODE_CONTROL_FLAGS;
    vscp_test_rxMessage.data[2]     = VSCP_NODE_CONTROL_FLAGS_DEFAULT | 0x20;

    vscp_test_waitForTxMessage(1, 10);
    CU_ASSERT_EQUAL(vscp_test_txMessageCnt, 1);

    for(index = 0; index < 8; ++index)
    {
        if (0 == vscp_test_dmPage)
        {
            vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_PROTOCOL;
            vscp_test_rxMessage.vscpType    = VSCP_TYPE_PROTOCOL_WRITE_REGISTER;
            vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
            vscp_test_rxMessage.oAddr       = VSCP_NICKNAME_SEGMENT_MASTER;
            vscp_test_rxMessage.hardCoded   = FALSE;
            vscp_test_rxMessage.dataNum     = 3;
            vscp_test_rxMessage.data[0]     = VSCP_TEST_NICKNAME;
            vscp_test_rxMessage.data[1]     = vscp_test_dmOffset + index;
            vscp_test_rxMessage.data[2]     = index + 1;
        }
        else
        {
            uint16_t    page    = vscp_test_dmPage;
            uint16_t    offset  = (uint16_t)vscp_test_dmOffset + (uint16_t)index;

            page += offset / 256;
            offset %= 256;

            vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_PROTOCOL;
            vscp_test_rxMessage.vscpType    = VSCP_TYPE_PROTOCOL_EXTENDED_PAGE_WRITE_REGISTER;
            vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
            vscp_test_rxMessage.oAddr       = VSCP_NICKNAME_SEGMENT_MASTER;
            vscp_test_rxMessage.hardCoded   = FALSE;
            vscp_test_rxMessage.dataNum     = 5;
            vscp_test_rxMessage.data[0]     = VSCP_TEST_NICKNAME;
            vscp_test_rxMessage.data[1]     = (page >> 8) & 0xff;
            vscp_test_rxMessage.data[2]     = (page >> 0) & 0xff;
            vscp_test_rxMessage.data[3]     = offset & 0xff;
            vscp_test_rxMessage.data[4]     = index + 1;
        }

        vscp_test_waitForTxMessage(1, 10);

        /* Node shall send a response */
        CU_ASSERT_EQUAL(vscp_test_txMessageCnt, 1);

        if (0 == vscp_test_dmPage)
        {
            CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpClass, VSCP_CLASS_L1_PROTOCOL);
            CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpType, VSCP_TYPE_PROTOCOL_READ_WRITE_RESPONSE);
            CU_ASSERT_EQUAL(vscp_test_txMessage[0].oAddr, VSCP_TEST_NICKNAME);
            CU_ASSERT_EQUAL(vscp_test_txMessage[0].hardCoded, FALSE);
            CU_ASSERT_EQUAL(vscp_test_txMessage[0].dataNum, 2);
            CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[0], vscp_test_dmOffset + index);
            CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[1], index + 1);
        }
        else
        {
            uint16_t    page    = vscp_test_dmPage;
            uint16_t    offset  = (uint16_t)vscp_test_dmOffset + (uint16_t)index;

            page += offset / 256;
            offset %= 256;

            CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpClass, VSCP_CLASS_L1_PROTOCOL);
            CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpType, VSCP_TYPE_PROTOCOL_EXTENDED_PAGE_READ_WRITE_RESPONSE);
            CU_ASSERT_EQUAL(vscp_test_txMessage[0].oAddr, VSCP_TEST_NICKNAME);
            CU_ASSERT_EQUAL(vscp_test_txMessage[0].hardCoded, FALSE);
            CU_ASSERT_EQUAL(vscp_test_txMessage[0].dataNum, 5);
            CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[0], 0);
            CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[1], (page >> 8) & 0xff);
            CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[2], (page >> 0) & 0xff);
            CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[3], offset & 0xff);
            CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[4], index + 1);
        }
    }

    return;
}


/**
 * Precondition:
 *  - VSCP is active.
 *  - Decision matrix exists
 *
 * Action:
 *  - Read decision matrix data
 *
 * Expectation:
 *  - Response ...
 */
extern void vscp_test_active30(void)
{
    uint8_t index   = 0;

    vscp_test_initTestCase();

    for(index = 0; index < 8; ++index)
    {
        if (0 == vscp_test_dmPage)
        {
            vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_PROTOCOL;
            vscp_test_rxMessage.vscpType    = VSCP_TYPE_PROTOCOL_READ_REGISTER;
            vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
            vscp_test_rxMessage.oAddr       = VSCP_NICKNAME_SEGMENT_MASTER;
            vscp_test_rxMessage.hardCoded   = FALSE;
            vscp_test_rxMessage.dataNum     = 2;
            vscp_test_rxMessage.data[0]     = VSCP_TEST_NICKNAME;
            vscp_test_rxMessage.data[1]     = vscp_test_dmOffset + index;
        }
        else
        {
            uint16_t    page    = vscp_test_dmPage;
            uint16_t    offset  = (uint16_t)vscp_test_dmOffset + (uint16_t)index;

            page += offset / 256;
            offset %= 256;

            vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_PROTOCOL;
            vscp_test_rxMessage.vscpType    = VSCP_TYPE_PROTOCOL_EXTENDED_PAGE_READ_REGISTER;
            vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
            vscp_test_rxMessage.oAddr       = VSCP_NICKNAME_SEGMENT_MASTER;
            vscp_test_rxMessage.hardCoded   = FALSE;
            vscp_test_rxMessage.dataNum     = 4;
            vscp_test_rxMessage.data[0]     = VSCP_TEST_NICKNAME;
            vscp_test_rxMessage.data[1]     = (page >> 8) & 0xff;
            vscp_test_rxMessage.data[2]     = (page >> 0) & 0xff;
            vscp_test_rxMessage.data[3]     = offset & 0xff;
        }

        vscp_test_waitForTxMessage(1, 10);

        /* Node shall send a response */
        CU_ASSERT_EQUAL(vscp_test_txMessageCnt, 1);

        if (0 == vscp_test_dmPage)
        {
            CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpClass, VSCP_CLASS_L1_PROTOCOL);
            CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpType, VSCP_TYPE_PROTOCOL_READ_WRITE_RESPONSE);
            CU_ASSERT_EQUAL(vscp_test_txMessage[0].oAddr, VSCP_TEST_NICKNAME);
            CU_ASSERT_EQUAL(vscp_test_txMessage[0].hardCoded, FALSE);
            CU_ASSERT_EQUAL(vscp_test_txMessage[0].dataNum, 2);
            CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[0], vscp_test_dmOffset + index);
            CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[1], 1 + index);
        }
        else
        {
            uint16_t    page    = vscp_test_dmPage;
            uint16_t    offset  = (uint16_t)vscp_test_dmOffset + (uint16_t)index;

            page += offset / 256;
            offset %= 256;

            CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpClass, VSCP_CLASS_L1_PROTOCOL);
            CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpType, VSCP_TYPE_PROTOCOL_EXTENDED_PAGE_READ_WRITE_RESPONSE);
            CU_ASSERT_EQUAL(vscp_test_txMessage[0].oAddr, VSCP_TEST_NICKNAME);
            CU_ASSERT_EQUAL(vscp_test_txMessage[0].hardCoded, FALSE);
            CU_ASSERT_EQUAL(vscp_test_txMessage[0].dataNum, 5);
            CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[0], 0);
            CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[1], (page >> 8) & 0xff);
            CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[2], (page >> 0) & 0xff);
            CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[3], offset & 0xff);
            CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[4], index + 1);
        }
    }

    return;
}

/**
 * Initialize decision matrix and the decision matrix extension.
 */
extern int vscp_test_initDM(void)
{
    uint8_t index   = 0;

    vscp_test_initTestCase();

    /* Reset decision matrix */
    memset(vscp_test_dmStorage, 0, VSCP_PS_SIZE_DM);

    /* Reset decision matrix extension */
    memset(vscp_test_extStorage, 0, VSCP_PS_SIZE_DM_EXTENSION);

    /* Any event from node 0xaa triggers. */
    vscp_test_dmStorage[index].action       = index + 1;
    vscp_test_dmStorage[index].actionPar    = index;
    vscp_test_dmStorage[index].flags        = VSCP_DM_FLAG_ENABLE | VSCP_DM_FLAG_CHECK_OADDR;
    vscp_test_dmStorage[index].oaddr        = 0xaa;
    ++index;

    /* Any event from a hard coded node triggers. */
    vscp_test_dmStorage[index].action       = index + 1;
    vscp_test_dmStorage[index].actionPar    = index;
    vscp_test_dmStorage[index].flags        = VSCP_DM_FLAG_ENABLE | VSCP_DM_FLAG_HARDCODED;
    ++index;

    /* Any event with class 0x1f5 triggers. */
    vscp_test_dmStorage[index].action       = index + 1;
    vscp_test_dmStorage[index].actionPar    = index;
    vscp_test_dmStorage[index].flags        = VSCP_DM_FLAG_ENABLE | VSCP_DM_FLAG_CLASS_MASK_BIT8 | VSCP_DM_FLAG_CLASS_FILTER_BIT8;
    vscp_test_dmStorage[index].classMask    = 0xff;
    vscp_test_dmStorage[index].classFilter  = 0xf5;
    ++index;

    /* Any event with type 0x12 triggers. */
    vscp_test_dmStorage[index].action       = index + 1;
    vscp_test_dmStorage[index].actionPar    = index;
    vscp_test_dmStorage[index].flags        = VSCP_DM_FLAG_ENABLE;
    vscp_test_dmStorage[index].typeMask     = 0xff;
    vscp_test_dmStorage[index].typeFilter   = 0x12;
    ++index;

    /* Any event for the node zone and sub-zone triggers. */
    vscp_test_dmStorage[index].action       = index + 1;
    vscp_test_dmStorage[index].actionPar    = index;
    vscp_test_dmStorage[index].flags        = VSCP_DM_FLAG_ENABLE | VSCP_DM_FLAG_MATCH_ZONE | VSCP_DM_FLAG_MATCH_SUB_ZONE;
    ++index;

    /* Any class 0x14, type 0x01 event from node 0xbb triggers. */
    vscp_test_dmStorage[index].action       = index + 1;
    vscp_test_dmStorage[index].actionPar    = index;
    vscp_test_dmStorage[index].flags        = VSCP_DM_FLAG_ENABLE | VSCP_DM_FLAG_CHECK_OADDR | VSCP_DM_FLAG_CLASS_MASK_BIT8;
    vscp_test_dmStorage[index].oaddr        = 0xbb;
    vscp_test_dmStorage[index].classMask    = 0xff;
    vscp_test_dmStorage[index].classFilter  = 0x14;
    vscp_test_dmStorage[index].typeMask     = 0xff;
    vscp_test_dmStorage[index].typeFilter   = 0x01;
    ++index;

    /* Trigger for class information, type button event from node 0xdd, zone 0x02, sub-zone 0x03 */
    vscp_test_dmStorage[index].action       = VSCP_DM_ACTION_EXTENDED_DM;
    vscp_test_dmStorage[index].actionPar    = 0;
    vscp_test_dmStorage[index].flags        = VSCP_DM_FLAG_ENABLE | VSCP_DM_FLAG_CHECK_OADDR | VSCP_DM_FLAG_MATCH_ZONE | VSCP_DM_FLAG_MATCH_SUB_ZONE | VSCP_DM_FLAG_CLASS_MASK_BIT8;
    vscp_test_dmStorage[index].oaddr        = 0xdd;
    vscp_test_dmStorage[index].classMask    = 0xff;
    vscp_test_dmStorage[index].classFilter  = 0x14;
    vscp_test_dmStorage[index].typeMask     = 0xff;
    vscp_test_dmStorage[index].typeFilter   = 0x01;

    vscp_test_extStorage[index].zone        = 0x02;
    vscp_test_extStorage[index].subZone     = 0x03;
    vscp_test_extStorage[index].action      = index + 1;
    vscp_test_extStorage[index].actionPar   = index;
    ++index;

    /* Trigger for class information, type button event from node 0xdd, par0 = 0x01, par3 = 0x04, par4 = 0x05, par5 = 0x06 */
    vscp_test_dmStorage[index].action       = VSCP_DM_ACTION_EXTENDED_DM;
    vscp_test_dmStorage[index].actionPar    = VSCP_DM_EXTFLAG_MATCH_PAR_0 | VSCP_DM_EXTFLAG_MATCH_PAR_3 | VSCP_DM_EXTFLAG_MATCH_PAR_4 | VSCP_DM_EXTFLAG_MATCH_PAR_5;
    vscp_test_dmStorage[index].flags        = VSCP_DM_FLAG_ENABLE | VSCP_DM_FLAG_CHECK_OADDR | VSCP_DM_FLAG_CLASS_MASK_BIT8;
    vscp_test_dmStorage[index].oaddr        = 0xdd;
    vscp_test_dmStorage[index].classMask    = 0xff;
    vscp_test_dmStorage[index].classFilter  = 0x14;
    vscp_test_dmStorage[index].typeMask     = 0xff;
    vscp_test_dmStorage[index].typeFilter   = 0x01;

    vscp_test_extStorage[index].zone        = 0x03;
    vscp_test_extStorage[index].subZone     = 0x04;
    vscp_test_extStorage[index].par0        = 0x01;
    vscp_test_extStorage[index].par3        = 0x04;
    vscp_test_extStorage[index].par4        = 0x05;
    vscp_test_extStorage[index].par5        = 0x06;
    vscp_test_extStorage[index].action      = index + 1;
    vscp_test_extStorage[index].actionPar   = index;
    ++index;

    return 0;
}

/**
 * Precondition:
 *  - VSCP is active.
 *  - Decision matrix configured
 *
 * Action:
 *  - Node 0xaa sends a event
 *
 * Expectation:
 *  - Action 1 is executed.
 */
extern void vscp_test_dm01(void)
{
    vscp_test_initTestCase();

    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_INFORMATION;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_INFORMATION_BUTTON;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = 0xaa;
    vscp_test_rxMessage.hardCoded   = FALSE;
    vscp_test_rxMessage.dataNum     = 7;
    vscp_test_rxMessage.data[0]     = 0;
    vscp_test_rxMessage.data[1]     = 1;
    vscp_test_rxMessage.data[2]     = 2;
    vscp_test_rxMessage.data[3]     = 0;
    vscp_test_rxMessage.data[4]     = 0;
    vscp_test_rxMessage.data[5]     = 0;
    vscp_test_rxMessage.data[6]     = 0;

    /* Process core */
    vscp_test_processTimers();
    vscp_core_process();

    /* Only one action shall be executed */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_ACTION_EXECUTE], 1);

    /* Check action */
    CU_ASSERT_EQUAL(vscp_test_action, 1);
    CU_ASSERT_EQUAL(vscp_test_actionPar, 0);

    return;
}

/**
 * Precondition:
 *  - VSCP is active.
 *  - Decision matrix configured
 *
 * Action:
 *  - Send a event from a hard coded node.
 *
 * Expectation:
 *  - Action 2 is executed.
 */
extern void vscp_test_dm02(void)
{
    vscp_test_initTestCase();

    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_INFORMATION;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_INFORMATION_BUTTON;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = 0xaa;
    vscp_test_rxMessage.hardCoded   = TRUE;
    vscp_test_rxMessage.dataNum     = 7;
    vscp_test_rxMessage.data[0]     = 0;
    vscp_test_rxMessage.data[1]     = 1;
    vscp_test_rxMessage.data[2]     = 2;
    vscp_test_rxMessage.data[3]     = 0;
    vscp_test_rxMessage.data[4]     = 0;
    vscp_test_rxMessage.data[5]     = 0;
    vscp_test_rxMessage.data[6]     = 0;

    /* Process core */
    vscp_test_processTimers();
    vscp_core_process();

    /* Two actions shall be executed */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_ACTION_EXECUTE], 2);

    /* Check last action */
    CU_ASSERT_EQUAL(vscp_test_action, 2);
    CU_ASSERT_EQUAL(vscp_test_actionPar, 1);

    return;
}

/**
 * Precondition:
 *  - VSCP is active.
 *  - Decision matrix configured
 *
 * Action:
 *  - Send a event with class 0x1f5.
 *
 * Expectation:
 *  - Action 3 is executed.
 */
extern void vscp_test_dm03(void)
{
    vscp_test_initTestCase();

    vscp_test_rxMessage.vscpClass   = 0x1f5;
    vscp_test_rxMessage.vscpType    = 0x01;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = 0xaa;
    vscp_test_rxMessage.hardCoded   = TRUE;
    vscp_test_rxMessage.dataNum     = 7;
    vscp_test_rxMessage.data[0]     = 0;
    vscp_test_rxMessage.data[1]     = 1;
    vscp_test_rxMessage.data[2]     = 2;
    vscp_test_rxMessage.data[3]     = 0;
    vscp_test_rxMessage.data[4]     = 0;
    vscp_test_rxMessage.data[5]     = 0;
    vscp_test_rxMessage.data[6]     = 0;

    /* Process core */
    vscp_test_processTimers();
    vscp_core_process();

    /* Three actions shall be executed */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_ACTION_EXECUTE], 3);

    /* Check last action */
    CU_ASSERT_EQUAL(vscp_test_action, 3);
    CU_ASSERT_EQUAL(vscp_test_actionPar, 2);

    return;
}

/**
 * Precondition:
 *  - VSCP is active.
 *  - Decision matrix configured
 *
 * Action:
 *  - Send a event with type 0x12.
 *
 * Expectation:
 *  - Action 4 is executed.
 */
extern void vscp_test_dm04(void)
{
    vscp_test_initTestCase();

    vscp_test_rxMessage.vscpClass   = 0x1f5;
    vscp_test_rxMessage.vscpType    = 0x012;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = 0xaa;
    vscp_test_rxMessage.hardCoded   = TRUE;
    vscp_test_rxMessage.dataNum     = 7;
    vscp_test_rxMessage.data[0]     = 0;
    vscp_test_rxMessage.data[1]     = 1;
    vscp_test_rxMessage.data[2]     = 2;
    vscp_test_rxMessage.data[3]     = 0;
    vscp_test_rxMessage.data[4]     = 0;
    vscp_test_rxMessage.data[5]     = 0;
    vscp_test_rxMessage.data[6]     = 0;

    /* Process core */
    vscp_test_processTimers();
    vscp_core_process();

    /* Three actions shall be executed */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_ACTION_EXECUTE], 4);

    /* Check last action */
    CU_ASSERT_EQUAL(vscp_test_action, 4);
    CU_ASSERT_EQUAL(vscp_test_actionPar, 3);

    return;
}

/**
 * Precondition:
 *  - VSCP is active.
 *  - Decision matrix configured
 *
 * Action:
 *  - Send a event with the nodes zone and sub-zone.
 *
 * Expectation:
 *  - Action 5 is executed.
 */
extern void vscp_test_dm05(void)
{
    vscp_test_initTestCase();

    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_INFORMATION;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_INFORMATION_BUTTON;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = 0xaa;
    vscp_test_rxMessage.hardCoded   = TRUE;
    vscp_test_rxMessage.dataNum     = 7;
    vscp_test_rxMessage.data[0]     = 0;
    vscp_test_rxMessage.data[1]     = VSCP_TEST_HEARTBEAT_ZONE;
    vscp_test_rxMessage.data[2]     = VSCP_TEST_HEARTBEAT_SUB_ZONE;
    vscp_test_rxMessage.data[3]     = 0;
    vscp_test_rxMessage.data[4]     = 0;
    vscp_test_rxMessage.data[5]     = 0;
    vscp_test_rxMessage.data[6]     = 0;

    /* Process core */
    vscp_test_processTimers();
    vscp_core_process();

    /* Three actions shall be executed */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_ACTION_EXECUTE], 3);

    /* Check last action */
    CU_ASSERT_EQUAL(vscp_test_action, 5);
    CU_ASSERT_EQUAL(vscp_test_actionPar, 4);

    return;
}

/**
 * Precondition:
 *  - VSCP is active.
 *  - Decision matrix configured
 *
 * Action:
 *  - Send a class 0x14, type 0x01 event from node 0xbb.
 *
 * Expectation:
 *  - Action 6 is executed.
 */
extern void vscp_test_dm06(void)
{
    vscp_test_initTestCase();

    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_INFORMATION;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_INFORMATION_BUTTON;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = 0xbb;
    vscp_test_rxMessage.hardCoded   = FALSE;
    vscp_test_rxMessage.dataNum     = 7;
    vscp_test_rxMessage.data[0]     = 0;
    vscp_test_rxMessage.data[1]     = 1;
    vscp_test_rxMessage.data[2]     = 2;
    vscp_test_rxMessage.data[3]     = 0;
    vscp_test_rxMessage.data[4]     = 0;
    vscp_test_rxMessage.data[5]     = 0;
    vscp_test_rxMessage.data[6]     = 0;

    /* Process core */
    vscp_test_processTimers();
    vscp_core_process();

    /* Three actions shall be executed */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_ACTION_EXECUTE], 1);

    /* Check last action */
    CU_ASSERT_EQUAL(vscp_test_action, 6);
    CU_ASSERT_EQUAL(vscp_test_actionPar, 5);

    return;
}

/**
 * Precondition:
 *  - VSCP is active.
 *  - Decision matrix configured
 *
 * Action:
 *  - Send a class information, type button event from node 0xdd with zone 0x02, sub-zone 0x03
 *
 * Expectation:
 *  - Action 7 is executed.
 */
extern void vscp_test_dm07(void)
{
    vscp_test_initTestCase();

    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_INFORMATION;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_INFORMATION_BUTTON;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = 0xdd;
    vscp_test_rxMessage.hardCoded   = FALSE;
    vscp_test_rxMessage.dataNum     = 7;
    vscp_test_rxMessage.data[0]     = 0;
    vscp_test_rxMessage.data[1]     = 2;
    vscp_test_rxMessage.data[2]     = 3;
    vscp_test_rxMessage.data[3]     = 0;
    vscp_test_rxMessage.data[4]     = 0;
    vscp_test_rxMessage.data[5]     = 0;
    vscp_test_rxMessage.data[6]     = 0;

    /* Process core */
    vscp_test_processTimers();
    vscp_core_process();

    /* Three actions shall be executed */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_ACTION_EXECUTE], 1);

    /* Check last action */
    CU_ASSERT_EQUAL(vscp_test_action, 7);
    CU_ASSERT_EQUAL(vscp_test_actionPar, 6);

    return;
}

/**
 * Precondition:
 *  - VSCP is active.
 *  - Decision matrix configured
 *
 * Action:
 *  - Send class information, type button event from node 0xdd, par0 = 0x01, par3 = 0x04, par4 = 0x05, par5 = 0x06
 *
 * Expectation:
 *  - Action 8 is executed.
 */
extern void vscp_test_dm08(void)
{
    vscp_test_initTestCase();

    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_INFORMATION;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_INFORMATION_BUTTON;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = 0xdd;
    vscp_test_rxMessage.hardCoded   = FALSE;
    vscp_test_rxMessage.dataNum     = 7;
    vscp_test_rxMessage.data[0]     = 1;
    vscp_test_rxMessage.data[1]     = 10;
    vscp_test_rxMessage.data[2]     = 10;
    vscp_test_rxMessage.data[3]     = 4;
    vscp_test_rxMessage.data[4]     = 5;
    vscp_test_rxMessage.data[5]     = 6;
    vscp_test_rxMessage.data[6]     = 0;

    /* Process core */
    vscp_test_processTimers();
    vscp_core_process();

    /* Three actions shall be executed */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_ACTION_EXECUTE], 1);

    /* Check last action */
    CU_ASSERT_EQUAL(vscp_test_action, 8);
    CU_ASSERT_EQUAL(vscp_test_actionPar, 7);

    return;
}

/**
 * Initialize decision matrix next generation.
 */
extern int vscp_test_initDMNG(void)
{
    uint8_t index       = 0;
    uint8_t ruleSet[]   =
    {
        /* Number of rules */
        0x02,
        /* Rule 1 */
        0x16, 0x01, 0x01,
            /* Decision  */
            VSCP_DM_NG_LOGIC_OP_AND | VSCP_DM_NG_BASIC_OP_EQUAL, VSCP_DM_NG_EVENT_PAR_ID_CLASS, 0x00, 0x14,
            VSCP_DM_NG_LOGIC_OP_AND | VSCP_DM_NG_BASIC_OP_EQUAL, VSCP_DM_NG_EVENT_PAR_ID_TYPE, 0x01,
            VSCP_DM_NG_LOGIC_OP_AND | VSCP_DM_NG_BASIC_OP_EQUAL, VSCP_DM_NG_EVENT_PAR_ID_OADDR, VSCP_TEST_NICKNAME,
            VSCP_DM_NG_LOGIC_OP_AND | VSCP_DM_NG_BASIC_OP_GREATER, VSCP_DM_NG_EVENT_PAR_ID_DATA0, 0x00,
            VSCP_DM_NG_LOGIC_OP_AND | VSCP_DM_NG_BASIC_OP_LOWER, VSCP_DM_NG_EVENT_PAR_ID_DATA0, 0x03,
            VSCP_DM_NG_LOGIC_OP_LAST | VSCP_DM_NG_BASIC_OP_EQUAL, VSCP_DM_NG_EVENT_PAR_ID_DATA_NUM, 0x07,
        /* Rule 2 */
        0x10, 0x02, 0x02,
            /* Decision  */
            VSCP_DM_NG_LOGIC_OP_AND | VSCP_DM_NG_BASIC_OP_EQUAL, VSCP_DM_NG_EVENT_PAR_ID_CLASS, 0x00, 0x14,
            VSCP_DM_NG_LOGIC_OP_AND | VSCP_DM_NG_BASIC_OP_EQUAL, VSCP_DM_NG_EVENT_PAR_ID_TYPE, 0x01,
            VSCP_DM_NG_LOGIC_OP_AND | VSCP_DM_NG_BASIC_OP_EQUAL, VSCP_DM_NG_EVENT_PAR_ID_OADDR, 0xaa,
            VSCP_DM_NG_LOGIC_OP_LAST | VSCP_DM_NG_BASIC_OP_EQUAL, VSCP_DM_NG_EVENT_PAR_ID_HARD_CODED, 0x01
    };

    vscp_test_initTestCase();

    /* Reset decision matrix */
    memset(vscp_test_dmStorage, 0, VSCP_PS_SIZE_DM);

    /* Reset decision matrix extension */
    memset(vscp_test_extStorage, 0, VSCP_PS_SIZE_DM_EXTENSION);

    /* Set rule set */
    for(index = 0; index < sizeof(ruleSet); ++index)
    {
        vscp_test_dmNG[index] = ruleSet[index];
    }

    return 0;
}

/**
 * Precondition:
 *  - VSCP is active.
 *  - Decision matrix next generation configured
 *
 * Action:
 *  - Send class information, type button event with parameter "all released".
 *
 * Expectation:
 *  - No action is executed.
 */
extern void vscp_test_dmNG01(void)
{
    uint8_t rule    = 0;
    uint8_t pos     = 0;

    vscp_test_initTestCase();

    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_INFORMATION;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_INFORMATION_BUTTON;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = VSCP_TEST_NICKNAME;
    vscp_test_rxMessage.hardCoded   = FALSE;
    vscp_test_rxMessage.dataNum     = 7;
    vscp_test_rxMessage.data[0]     = 0;
    vscp_test_rxMessage.data[1]     = 1;
    vscp_test_rxMessage.data[2]     = 2;
    vscp_test_rxMessage.data[3]     = 3;
    vscp_test_rxMessage.data[4]     = 4;
    vscp_test_rxMessage.data[5]     = 5;
    vscp_test_rxMessage.data[6]     = 6;

    /* Process core */
    vscp_test_processTimers();
    vscp_core_process();

    /* No action shall be executed */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_ACTION_EXECUTE], 0);

    if (FALSE != vscp_dm_ng_getError(&rule, &pos))
    {
        printf("\nError in rule %u, at position %u.\n", rule, pos);
    }

    return;
}

/**
 * Precondition:
 *  - VSCP is active.
 *  - Decision matrix next generation configured
 *
 * Action:
 *  - Send class information, type button event with parameter "pressed".
 *
 * Expectation:
 *  - Action 1 is executed.
 */
extern void vscp_test_dmNG02(void)
{
    uint8_t rule    = 0;
    uint8_t pos     = 0;

    vscp_test_initTestCase();

    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_INFORMATION;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_INFORMATION_BUTTON;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = VSCP_TEST_NICKNAME;
    vscp_test_rxMessage.hardCoded   = FALSE;
    vscp_test_rxMessage.dataNum     = 7;
    vscp_test_rxMessage.data[0]     = 1;
    vscp_test_rxMessage.data[1]     = 1;
    vscp_test_rxMessage.data[2]     = 2;
    vscp_test_rxMessage.data[3]     = 3;
    vscp_test_rxMessage.data[4]     = 4;
    vscp_test_rxMessage.data[5]     = 5;
    vscp_test_rxMessage.data[6]     = 6;

    /* Process core */
    vscp_test_processTimers();
    vscp_core_process();

    /* No action shall be executed */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_ACTION_EXECUTE], 1);

    /* Check last action */
    CU_ASSERT_EQUAL(vscp_test_action, 1);
    CU_ASSERT_EQUAL(vscp_test_actionPar, 1);

    if (FALSE != vscp_dm_ng_getError(&rule, &pos))
    {
        printf("\nError in rule %u, at position %u.\n", rule, pos);
    }

    return;
}

/**
 * Precondition:
 *  - VSCP is active.
 *  - Decision matrix exists
 *
 * Action:
 *  - Extended page read
 *
 * Expectation:
 *  - Response ...
 */
extern void vscp_test_active31(void)
{
    vscp_test_initTestCase();

    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_PROTOCOL;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_PROTOCOL_EXTENDED_PAGE_READ_REGISTER;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = VSCP_NICKNAME_SEGMENT_MASTER;
    vscp_test_rxMessage.hardCoded   = FALSE;
    vscp_test_rxMessage.dataNum     = 5;
    vscp_test_rxMessage.data[0]     = VSCP_TEST_NICKNAME;
    vscp_test_rxMessage.data[1]     = 0;
    vscp_test_rxMessage.data[2]     = 0;
    vscp_test_rxMessage.data[3]     = VSCP_REG_VSCP_VERSION_MAJOR;
    vscp_test_rxMessage.data[4]     = 1;

    vscp_test_waitForTxMessage(1, 10);

    /* Node shall send a response */
    CU_ASSERT_EQUAL(vscp_test_txMessageCnt, 1);

    CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpClass, VSCP_CLASS_L1_PROTOCOL);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpType, VSCP_TYPE_PROTOCOL_EXTENDED_PAGE_READ_WRITE_RESPONSE);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].oAddr, VSCP_TEST_NICKNAME);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].hardCoded, FALSE);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].dataNum, 5);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[0], 0);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[1], 0);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[2], 0);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[3], VSCP_REG_VSCP_VERSION_MAJOR);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[4], VSCP_CORE_VERSION_MAJOR);

    return;
}

/**
 * Precondition:
 *  - VSCP is active.
 *  - Decision matrix exists
 *
 * Action:
 *  - Extended page write
 *
 * Expectation:
 *  - Response ...
 */
extern void vscp_test_active32(void)
{
    vscp_test_initTestCase();

    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_PROTOCOL;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_PROTOCOL_EXTENDED_PAGE_WRITE_REGISTER;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = VSCP_NICKNAME_SEGMENT_MASTER;
    vscp_test_rxMessage.hardCoded   = FALSE;
    vscp_test_rxMessage.dataNum     = 5;
    vscp_test_rxMessage.data[0]     = VSCP_TEST_NICKNAME;
    vscp_test_rxMessage.data[1]     = 0;
    vscp_test_rxMessage.data[2]     = 0;
    vscp_test_rxMessage.data[3]     = VSCP_REG_USER_ID_0;
    vscp_test_rxMessage.data[4]     = 0x18;

    vscp_test_waitForTxMessage(1, 10);

    /* Node shall send a response */
    CU_ASSERT_EQUAL(vscp_test_txMessageCnt, 1);

    CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpClass, VSCP_CLASS_L1_PROTOCOL);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpType, VSCP_TYPE_PROTOCOL_EXTENDED_PAGE_READ_WRITE_RESPONSE);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].oAddr, VSCP_TEST_NICKNAME);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].hardCoded, FALSE);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].dataNum, 5);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[0], 0);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[1], 0);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[2], 0);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[3], VSCP_REG_USER_ID_0);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[4], 0x18);

    return;
}

/**
 * Precondition:
 *  - VSCP is active.
 *  - Decision matrix exists
 *
 * Action:
 *  - Wait for node heartbeat
 *
 * Expectation:
 *  - Response ...
 */
extern void vscp_test_active33(void)
{
    vscp_test_initTestCase();

    vscp_test_waitForTxMessage(1, VSCP_CONFIG_HEARTBEAT_NODE_PERIOD + 1);

    /* Node shall send a response */
    CU_ASSERT_EQUAL(vscp_test_txMessageCnt, 1);

    CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpClass, VSCP_CLASS_L1_INFORMATION);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpType, VSCP_TYPE_INFORMATION_NODE_HEARTBEAT);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].oAddr, VSCP_TEST_NICKNAME);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].hardCoded, FALSE);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].dataNum, 3);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[0], 0);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[1], VSCP_TEST_HEARTBEAT_ZONE);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[2], VSCP_TEST_HEARTBEAT_SUB_ZONE);

    return;
}

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Prepare tx message
 *  - Send message
 *
 * Expectation:
 *  - Message is sent to the transport layer.
 */
extern void vscp_test_active34(void)
{
    vscp_TxMessage  txMessage;

    vscp_core_prepareTxMessage(&txMessage, VSCP_CLASS_L1_PROTOCOL, VSCP_TYPE_PROTOCOL_UNDEFINED, VSCP_PRIORITY_3_NORMAL);

    CU_ASSERT_EQUAL(txMessage.vscpClass, VSCP_CLASS_L1_PROTOCOL);
    CU_ASSERT_EQUAL(txMessage.vscpType, VSCP_TYPE_PROTOCOL_UNDEFINED);
    CU_ASSERT_EQUAL(txMessage.priority, VSCP_PRIORITY_3_NORMAL);
    CU_ASSERT_EQUAL(txMessage.oAddr, VSCP_TEST_NICKNAME);
    CU_ASSERT_EQUAL(txMessage.hardCoded, FALSE);
    CU_ASSERT_EQUAL(txMessage.dataNum, 0);

    txMessage.dataNum = 2;
    txMessage.data[0]   = 0x12;
    txMessage.data[1]   = 0x21;

    /* Clear transmit message buffers */
    memset(vscp_test_txMessage, 0, sizeof(vscp_test_txMessage));
    vscp_test_txMessageCnt = 0;

    vscp_core_sendEvent(&txMessage);

    /* Node shall send a response */
    CU_ASSERT_EQUAL(vscp_test_txMessageCnt, 1);

    CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpClass, VSCP_CLASS_L1_PROTOCOL);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpType, VSCP_TYPE_PROTOCOL_UNDEFINED);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].priority, VSCP_PRIORITY_3_NORMAL);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].oAddr, VSCP_TEST_NICKNAME);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].hardCoded, FALSE);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].dataNum, 2);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[0], 0x12);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[1], 0x21);

    return;
}

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Send a event to the core, which is not from the protocol class.
 *
 * Expectation:
 *  - Event is provided to the application.
 */
extern void vscp_test_active35(void)
{
    vscp_test_initTestCase();

    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_INFORMATION;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_INFORMATION_ALIVE;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_3_NORMAL;
    vscp_test_rxMessage.oAddr       = VSCP_TEST_NICKNAME_REMOTE;
    vscp_test_rxMessage.hardCoded   = FALSE;
    vscp_test_rxMessage.dataNum     = 0;

    vscp_test_waitForTxMessage(1, 1);

    /* Node shall not send a response */
    CU_ASSERT_EQUAL(vscp_test_txMessageCnt, 0);

    /* Node shall provide event to the application */
    CU_ASSERT_EQUAL(vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_PORTABLE_PROVIDE_EVENT], 1);

    return;
}



/* -------------------------------------------------------------------------- */
/*      Stubs                                                                 */
/* -------------------------------------------------------------------------- */

extern void vscp_test_psAccessInit(void)
{
    vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_PS_ACCESS_INIT]++;
    return;
}

extern uint8_t  vscp_test_psAccessRead8(uint16_t addr)
{
    uint8_t value   = 0;

    /* Check address out of bounce. */
    if (VSCP_TEST_ARRAY_NUM(vscp_test_persistentMemory) <= addr)
    {
        CU_ASSERT_FATAL(VSCP_TEST_ARRAY_NUM(vscp_test_persistentMemory) > addr);
    }
    else
    {
        value = vscp_test_persistentMemory[addr];
    }

    return value;
}

extern void vscp_test_psAccessWrite8(uint16_t addr, uint8_t value)
{
    /* Check address out of bounce. */
    if (VSCP_TEST_ARRAY_NUM(vscp_test_persistentMemory) <= addr)
    {
        CU_ASSERT_FATAL(VSCP_TEST_ARRAY_NUM(vscp_test_persistentMemory) > addr);
    }
    else
    {
        vscp_test_persistentMemory[addr] = value;
    }

    return;
}

extern void vscp_test_appRegInit(void)
{
    ++vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_APP_REG_INIT];
    return;
}

extern void vscp_test_tpAdatperInit(void)
{
    ++vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_TP_ADAPTER_INIT];
    return;
}

extern BOOL vscp_test_tpAdatperReadMessage(vscp_RxMessage * const msg)
{
    BOOL    status  = TRUE;

    CU_ASSERT_PTR_NOT_EQUAL_FATAL(msg, NULL);

    ++vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_TP_ADAPTER_READ_MESSAGE];

    *msg = vscp_test_rxMessage;

    return status;
}

extern BOOL vscp_test_tpAdatperWriteMessage(vscp_TxMessage const * const msg)
{
    BOOL    status  = TRUE;

    CU_ASSERT_PTR_NOT_EQUAL_FATAL(msg, NULL);
    CU_ASSERT_FATAL(VSCP_TEST_ARRAY_NUM(vscp_test_txMessage) > vscp_test_txMessageCnt);

    ++vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_TP_ADAPTER_WRITE_MESSAGE];

    vscp_test_txMessage[vscp_test_txMessageCnt] = *msg;
    ++vscp_test_txMessageCnt;

    return status;
}

extern void vscp_test_portableInit(void)
{
    ++vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_PORTABLE_INIT];
    return;
}

extern void vscp_test_portableRestoreFactoryDefaultSettings(void)
{
    uint8_t index   = 0;
    uint8_t part    = 0;

    vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_APP_RESTORE_FACTORY_DEFAULTS]++;

    /* Restore GUID */
    for(part = 0; part < 4; ++part)
    {
        uint32_t    guidPart    = 0;

        if (0 == part)
        {
            guidPart = VSCP_TEST_GUID_0;
        }
        else if (1 == part)
        {
            guidPart = VSCP_TEST_GUID_1;
        }
        else if (2 == part)
        {
            guidPart = VSCP_TEST_GUID_2;
        }
        else
        {
            guidPart = VSCP_TEST_GUID_3;
        }

        for(index = 0; index < 4; ++index)
        {
            uint8_t     value   = ((uint8_t*)&guidPart)[index];

            vscp_ps_writeGUID((part * 4) + index, value);
        }
    }

    /* Restore manufacturer device id */
    for(index = 0; index < sizeof(VSCP_TEST_MANUFACTURER_DEV_ID); ++index)
    {
        uint32_t    manufacturerDevId   = VSCP_TEST_MANUFACTURER_DEV_ID;
        uint8_t     value               = ((uint8_t*)&manufacturerDevId)[index];

        vscp_ps_writeManufacturerDevId(index, value);
    }

    /* Restore manufacturer sub device id */
    for(index = 0; index < sizeof(VSCP_TEST_MANUFACTURER_SUB_DEV_ID); ++index)
    {
        uint32_t    manufacturerSubDevId    = VSCP_TEST_MANUFACTURER_SUB_DEV_ID;
        uint8_t     value                   = ((uint8_t*)&manufacturerSubDevId)[index];

        vscp_ps_writeManufacturerSubDevId(index, value);
    }

    /* Restore standard device family code */
    for(index = 0; index < 4; ++index)
    {
        uint32_t    stdDevFamilyCode    = VSCP_TEST_STD_DEVICE_FAMILY_CODE;
        uint8_t     value               = ((uint8_t*)&stdDevFamilyCode)[index];

        vscp_ps_writeStdDevFamilyCode(index, value);
    }

    /* Restore standard device type */
    for(index = 0; index < 4; ++index)
    {
        uint32_t    stdDevType  = VSCP_TEST_STD_DEVICE_TYPE;
        uint8_t     value       = ((uint8_t*)&stdDevType)[index];

        vscp_ps_writeStdDevType(index, value);
    }

    return;
}

extern void vscp_test_timerInit(void)
{
    ++vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_TIMER_INIT];
    return;
}

extern uint8_t  vscp_test_timerCreate(void)
{
    uint8_t id  = vscp_test_timerInstances;

    ++vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_PORTABLE_CREATE_TIMER];

    if (TRUE == vscp_test_noMoreTimers)
    {
        id = 0xff;
    }
    else if (VSCP_TEST_ARRAY_NUM(vscp_test_timerValues) <= vscp_test_timerInstances)
    {
        CU_ASSERT_FATAL(VSCP_TEST_ARRAY_NUM(vscp_test_timerValues) > vscp_test_timerInstances);

        id = 0xff;
    }
    else
    {
        ++vscp_test_timerInstances;
    }

    return id;
}

extern void vscp_test_timerStart(uint8_t id, uint16_t value)
{
    CU_ASSERT_FATAL(vscp_test_timerInstances > id);

    ++vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_PORTABLE_START_TIMER];

    if (vscp_test_timerInstances > id)
    {
        vscp_test_timerValues[id] = value;
    }

    return;
}

extern void vscp_test_timerStop(uint8_t id)
{
    CU_ASSERT_FATAL(vscp_test_timerInstances > id);

    ++vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_PORTABLE_STOP_TIMER];

    if (vscp_test_timerInstances > id)
    {
        vscp_test_timerValues[id] = 0;
    }

    return;
}

extern BOOL vscp_test_timerGetStatus(uint8_t id)
{
    BOOL    status  = FALSE;

    CU_ASSERT_FATAL(vscp_test_timerInstances > id);

    if (vscp_test_timerInstances <= id)
    {
        status = FALSE;
    }
    else if (0 == vscp_test_timerValues[id])
    {
        status = FALSE;
    }
    else
    {
        status = TRUE;
    }

    return status;
}

extern void vscp_test_portableResetRequest(void)
{
    ++vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_PORTABLE_RESET_REQUEST];

    return;
}

extern void vscp_test_portableIdleStateEntered(void)
{
    ++vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_PORTABLE_IDLE_STATE_ENTERED];

    return;
}

extern void vscp_test_portableErrorStateEntered(void)
{
    ++vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_PORTABLE_ERROR_STATE_ENTERED];

    return;
}

extern uint8_t  vscp_test_portableGetBootLoaderAlgorithm(void)
{
    return VSCP_TEST_BOOT_LOADER_ALGORITHM;
}

extern uint8_t  vscp_test_portableGetPagesUsed(void)
{
    return VSCP_TEST_PAGES_USED;
}

extern void vscp_test_portableBootLoaderRequest(void)
{
    ++vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_PORTABLE_BOOT_LOADER_REQUEST];

    return;
}

extern uint8_t  vscp_test_portableGetMdfUrl(uint8_t index)
{
    const char  mdfUrl[]    = VSCP_TEST_MDF_URL;
    uint8_t     len         = strlen(mdfUrl);
    uint8_t     value       = 0;

    if (len > index)
    {
        value = (uint8_t)mdfUrl[index];
    }

    return value;
}

extern void vscp_test_portableProvideEvent(vscp_RxMessage const * const msg)
{
    ++vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_PORTABLE_PROVIDE_EVENT];

    return;
}

extern void vscp_test_portableSetLampState(VSCP_LAMP_STATE state)
{
    ++vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_PORTABLE_SET_LAMP_STATE];

    vscp_test_lampState = state;

    return;
}

extern void vscp_test_actionInit(void)
{
    ++vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_ACTION_INIT];
    return;
}

extern void vscp_test_actionExecute(uint8_t action, uint8_t par, vscp_RxMessage const * const msg)
{
    ++vscp_test_callCounter[VSCP_TEST_CALL_COUNTER_ACTION_EXECUTE];

    vscp_test_action                = action;
    vscp_test_actionPar             = par;
    vscp_test_actionTriggeredMsg    = *msg;

    return;
}

/*******************************************************************************
    LOCAL FUNCTIONS
*******************************************************************************/

static void vscp_test_initTestCase(void)
{
    /* Reset call counters */
    memset(vscp_test_callCounter, 0x00, sizeof(vscp_test_callCounter));

    /* Reset tx message buffer */
    vscp_test_txMessageCnt = 0;

    return;
}

static void vscp_test_waitForTxMessage(uint8_t min, uint16_t max)
{
    /* Clear transmit message buffers */
    memset(vscp_test_txMessage, 0, sizeof(vscp_test_txMessage));
    vscp_test_txMessageCnt = 0;

    /* Wait for a minimum number of messages */
    while((min > vscp_test_txMessageCnt) && (0 < max))
    {
        /* Process core */
        vscp_test_processTimers();
        vscp_core_process();

        --max;
    }

    return;
}

static void vscp_test_processTimers(void)
{
    uint8_t index   = 0;

    for(index = 0; index < VSCP_TEST_ARRAY_NUM(vscp_test_timerValues); ++index)
    {
        if (0 < vscp_test_timerValues[index])
        {
            --vscp_test_timerValues[index];
        }
    }

    return;
}

static uint8_t  vscp_test_readRegister(uint8_t addr)
{
    /* Read register */
    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_PROTOCOL;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_PROTOCOL_READ_REGISTER;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = VSCP_NICKNAME_SEGMENT_MASTER;
    vscp_test_rxMessage.hardCoded   = FALSE;
    vscp_test_rxMessage.dataNum     = 2;
    vscp_test_rxMessage.data[0]     = VSCP_TEST_NICKNAME;
    vscp_test_rxMessage.data[1]     = addr;

    vscp_test_waitForTxMessage(1, 10);

    /* Node shall send a response. */
    CU_ASSERT_EQUAL(vscp_test_txMessageCnt, 1);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpClass, VSCP_CLASS_L1_PROTOCOL);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpType, VSCP_TYPE_PROTOCOL_READ_WRITE_RESPONSE);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].oAddr, VSCP_TEST_NICKNAME);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].hardCoded, FALSE);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].dataNum, 2);
    CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[0], addr);

    return vscp_test_txMessage[0].data[1];
}

static uint8_t  vscp_test_writeRegister(uint8_t addr, uint8_t value, BOOL readOnly)
{
    /* Read register */
    vscp_test_rxMessage.vscpClass   = VSCP_CLASS_L1_PROTOCOL;
    vscp_test_rxMessage.vscpType    = VSCP_TYPE_PROTOCOL_WRITE_REGISTER;
    vscp_test_rxMessage.priority    = VSCP_PRIORITY_7_LOW;
    vscp_test_rxMessage.oAddr       = VSCP_NICKNAME_SEGMENT_MASTER;
    vscp_test_rxMessage.hardCoded   = FALSE;
    vscp_test_rxMessage.dataNum     = 3;
    vscp_test_rxMessage.data[0]     = VSCP_TEST_NICKNAME;
    vscp_test_rxMessage.data[1]     = addr;
    vscp_test_rxMessage.data[2]     = value;

    vscp_test_waitForTxMessage(1, 10);

    if (FALSE == readOnly)
    {
        /* Node shall send a response. */
        CU_ASSERT_EQUAL(vscp_test_txMessageCnt, 1);
        CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpClass, VSCP_CLASS_L1_PROTOCOL);
        CU_ASSERT_EQUAL(vscp_test_txMessage[0].vscpType, VSCP_TYPE_PROTOCOL_READ_WRITE_RESPONSE);
        CU_ASSERT_EQUAL(vscp_test_txMessage[0].oAddr, VSCP_TEST_NICKNAME);
        CU_ASSERT_EQUAL(vscp_test_txMessage[0].hardCoded, FALSE);
        CU_ASSERT_EQUAL(vscp_test_txMessage[0].dataNum, 2);
        CU_ASSERT_EQUAL(vscp_test_txMessage[0].data[0], addr);

        value = vscp_test_txMessage[0].data[1];
    }
    else
    {
        /* Node shall not send a response. */
        CU_ASSERT_EQUAL(vscp_test_txMessageCnt, 0);
    }

    return value;
}

static uint8_t  vscp_test_getAlarmStatus(uint8_t value)
{
    /* Prepare alarm status */
    vscp_core_setAlarm(value);

    return value;
}

static uint8_t  vscp_test_getControlFlags(uint8_t value)
{
    return vscp_ps_readNodeControlFlags();
}

static uint8_t  vscp_test_getNicknameId(uint8_t value)
{
    return vscp_ps_readNicknameId();
}

static uint8_t  vscp_test_getZero(uint8_t value)
{
    return 0;
}

