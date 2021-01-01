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
@brief  Intel hex string parser
@file   intelHexParser.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
This module contains functionality to parse a intel hex format string.

*******************************************************************************/
/** @defgroup intelHexParser The intel hex format string parser
 * This module contains functionality to parse a intel hex format string.
 *
 *
 * @{
 */

/*
 * Don't forget to set JAVADOC_AUTOBRIEF to YES in the doxygen file to generate
 * a correct module description.
 */

#ifndef __INTEL_HEX_PARSER_H__
#define __INTEL_HEX_PARSER_H__

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include <stdint.h>

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

/*******************************************************************************
    MACROS
*******************************************************************************/

/*******************************************************************************
    TYPES AND STRUCTURES
*******************************************************************************/

/** This type defines the function return values. */
typedef enum
{
    INTELHEXPARSER_RET_OK = 0,                  /**< Successful */
    INTELHEXPARSER_RET_ERROR,                   /**< Failed */
    INTELHEXPARSER_RET_NULL,                    /**< Unexpected NULL pointer */
    INTELHEXPARSER_RET_FILE_NOT_FOUND,          /**< File not found */
    INTELHEXPARSER_RET_INVALID_FORMAT,          /**< Invalid format */
    INTELHEXPARSER_RET_MISSING_REC_MARK,        /**< Missing record mark */
    INTELHEXPARSER_RET_INVALID_REC_LEN,         /**< Invalid record length */
    INTELHEXPARSER_RET_INVALID_REC_LOAD_OFFSET, /**< Invalid record load offset */
    INTELHEXPARSER_RET_INVALID_REC_TYPE,        /**< Invalid record type */
    INTELHEXPARSER_RET_INVALID_REC_DATA,        /**< Invalid record data */
    INTELHEXPARSER_RET_INVALID_REC_CHECKSUM,    /**< Invalid record checksum */
    INTELHEXPARSER_RET_INVALID_REC_END          /**< Invalid record end */

} INTELHEXPARSER_RET;

/** This type defines the record types. */
typedef enum
{
    INTELHEXPARSER_REC_TYPE_DATA = 0,       /**< Data */
    INTELHEXPARSER_REC_TYPE_EOF,            /**< End of file */
    INTELHEXPARSER_REC_TYPE_EXT_SEG_ADDR,   /**< Extended segment address record */
    INTELHEXPARSER_REC_TYPE_START_SEG_ADDR, /**< Start segment address record */
    INTELHEXPARSER_REC_TYPE_EXT_LIN_ADDR,   /**< Extended linear address record */
    INTELHEXPARSER_REC_TYPE_START_LIN_ADDR, /**< Start linear address record */

    INTELHEXPARSER_REC_TYPE_MAX

} INTELHEXPARSER_REC_TYPE;

/** This type defines a single intel hex record. */
typedef struct _intelHexParser_Record
{
    uint16_t                loadOffset; /**< Record load offset */
    INTELHEXPARSER_REC_TYPE type;       /**< Record type */
    uint8_t*                data;       /**< Record data */
    uint8_t                 dataSize;   /**< Record data size in byte */
    uint8_t                 checksum;   /**< Record checksum */

} intelHexParser_Record;

/*******************************************************************************
    VARIABLES
*******************************************************************************/

/*******************************************************************************
    FUNCTIONS
*******************************************************************************/

/**
 * Load a intel hex format file and returns the record set.
 * Don't forget to free the record set later, because it will be created on the heap.
 *
 * @param[in]   fileName    Name of a intel hex format file
 * @param[out]  recSet      Record set, which will be created
 * @param[out]  recNum      Number of records in the record set
 *
 * @return Status of the parse operation.
 */
extern INTELHEXPARSER_RET intelHexParser_load(char const * const fileName, intelHexParser_Record ** const recSet, uint32_t * const recNum);

/**
 * Parse a intel hex format string and returns the record set.
 * Don't forget to free the record set later, because it will be created on the heap.
 *
 * @param[in]   iHex    Intel hex format string
 * @param[out]  recSet  Record set, which will be created
 * @param[out]  recNum  Number of records in the record set
 *
 * @return Status of the parse operation.
 */
extern INTELHEXPARSER_RET intelHexParser_parse(char const * const iHex, intelHexParser_Record ** const recSet, uint32_t * const recNum);

/**
 * Free the record set.
 *
 * @param[in,out]   recSet  Record set
 */
extern void intelHexParser_freeRecSet(intelHexParser_Record** const recSet);

/**
 * Get the location in the intel hex format string of the last error.
 *
 * @param[out]  line    Line number in the intel hex format string
 * @param[out]  column  Column number in the intel hex format string
 */
extern void intelHexParser_getLastError(uint32_t * const line, uint32_t * const column);

#ifdef __cplusplus
}
#endif

#endif  /* __INTEL_HEX_PARSER_H__ */

/** @} */
