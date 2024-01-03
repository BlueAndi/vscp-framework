/* The MIT License (MIT)
 *
 * Copyright (c) 2014 - 2024 Andreas Merkle
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
@brief  Error
@file   vscp_type_error.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
Error reporting functionality that follow the errors defined in the vscp.h header file.

This file is automatically generated. Don't change it manually.

*******************************************************************************/

#ifndef __VSCP_TYPE_ERROR_H__
#define __VSCP_TYPE_ERROR_H__

/** @defgroup vscp_type_error Error
 * Level 1 protocol class types
 * @{
 * @ingroup vscp_l1
 */

/*******************************************************************************
    INCLUDES
*******************************************************************************/

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
    COMPILER SWITCHES
*******************************************************************************/

/*******************************************************************************
    CONSTANTS
*******************************************************************************/

/**
 * This error message is used to report success, that is no error.
 */
#define VSCP_TYPE_ERROR_SUCCESS           0

/**
 * This error message is used to indicate an error condition.
 */
#define VSCP_TYPE_ERROR_ERROR             1

/**
 * This error message is used to indicate a (communication) channel problem.
 */
#define VSCP_TYPE_ERROR_CHANNEL           7

/**
 * This error message is used to indicate a fifo empty problem.
 */
#define VSCP_TYPE_ERROR_FIFO_EMPTY        8

/**
 * This error message is used to indicate a fifo full problem.
 */
#define VSCP_TYPE_ERROR_FIFO_FULL         9

/**
 * This error message is used to indicate a fifo size problem.
 */
#define VSCP_TYPE_ERROR_FIFO_SIZE         10

/**
 * This error message is used to indicate a fifo wait problem.
 */
#define VSCP_TYPE_ERROR_FIFO_WAIT         11

/**
 * This error message is used to indicate a generic error.
 */
#define VSCP_TYPE_ERROR_GENERIC           12

/**
 * This error message is used to indicate a hardware error.
 */
#define VSCP_TYPE_ERROR_HARDWARE          13

/**
 * This error message is used to indicate a initialization error.
 */
#define VSCP_TYPE_ERROR_INIT_FAIL         14

/**
 * This error message is used to indicate a missing initialization error.
 */
#define VSCP_TYPE_ERROR_INIT_MISSING      15

/**
 * This error message is used to indicate a initialization ready error.
 */
#define VSCP_TYPE_ERROR_INIT_READY        16

/**
 * This error message is used to indicate a not supported error.
 */
#define VSCP_TYPE_ERROR_NOT_SUPPORTED     17

/**
 * This error message is used to indicate an overrun error.
 */
#define VSCP_TYPE_ERROR_OVERRUN           18

/**
 * This error message is used to indicate a receiver empty error.
 */
#define VSCP_TYPE_ERROR_RCV_EMPTY         19

/**
 * This error message is used to indicate a register error.
 */
#define VSCP_TYPE_ERROR_REGISTER          20

/**
 * This error message is used to indicate a transmitter full error.
 */
#define VSCP_TYPE_ERROR_TRM_FULL          21

/**
 * This error message is used to indicate a library error.
 */
#define VSCP_TYPE_ERROR_LIBRARY           28

/**
 * This error message is used to indicate a procedural address error.
 */
#define VSCP_TYPE_ERROR_PROCADDRESS       29

/**
 * This error message is used to indicate an only one instance error.
 */
#define VSCP_TYPE_ERROR_ONLY_ONE_INSTANCE 30

/**
 * This error message is used to indicate a sub driver error.
 */
#define VSCP_TYPE_ERROR_SUB_DRIVER        31

/**
 * This error message is used to indicate a timeout error.
 */
#define VSCP_TYPE_ERROR_TIMEOUT           32

/**
 * This error message is used to indicate a not open error.
 */
#define VSCP_TYPE_ERROR_NOT_OPEN          33

/**
 * This error message is used to indicate a parameter error.
 */
#define VSCP_TYPE_ERROR_PARAMETER         34

/**
 * This error message is used to indicate a memory error.
 */
#define VSCP_TYPE_ERROR_MEMORY            35

/**
 * This error message is used to indicate a internal error.
 */
#define VSCP_TYPE_ERROR_INTERNAL          36

/**
 * This error message is used to indicate a communication error.
 */
#define VSCP_TYPE_ERROR_COMMUNICATION     37

/**
 * This error message is used to indicate a user error.
 */
#define VSCP_TYPE_ERROR_USER              38

/**
 * This error message is used to indicate a password error.
 */
#define VSCP_TYPE_ERROR_PASSWORD          39

/**
 * This error message is used to indicate a connection error.
 */
#define VSCP_TYPE_ERROR_CONNECTION        40

/**
 * This error message is used to indicate a invalid handle error.
 */
#define VSCP_TYPE_ERROR_INVALID_HANDLE    41

/**
 * This error message is used to indicate an operation failed error.
 */
#define VSCP_TYPE_ERROR_OPERATION_FAILED  42

/**
 * This error message is used to indicate that the supplied buffer is to small to fit content.
 */
#define VSCP_TYPE_ERROR_BUFFER_SMALL      43

/**
 * This error message is used to indicate that the requested item (remote variable) is unknown.
 */
#define VSCP_TYPE_ERROR_ITEM_UNKNOWN      44

/**
 * This error message is used to indicate that the name is already in use.
 */
#define VSCP_TYPE_ERROR_NAME_USED         45

/**
 * This error message is used to indicate an error when writing data.
 */
#define VSCP_TYPE_ERROR_DATA_WRITE        46

/**
 * This error message is used to indicate a stopped or an aborted operation.
 */
#define VSCP_TYPE_ERROR_ABORTED           47

/**
 * This error message is used to indicate a pointer with an invalid value.
 */
#define VSCP_TYPE_ERROR_INVALID_POINTER   48

/*******************************************************************************
    MACROS
*******************************************************************************/

/*******************************************************************************
    TYPES AND STRUCTURES
*******************************************************************************/

/*******************************************************************************
    VARIABLES
*******************************************************************************/

/*******************************************************************************
    FUNCTIONS
*******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif  /* __VSCP_TYPE_ERROR_H__ */

/** @} */
