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
@file   intelHexParser.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
@see intelHexParser.h

*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "intelHexParser.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "log.h"

/*******************************************************************************
    COMPILER SWITCHES
*******************************************************************************/

/*******************************************************************************
    CONSTANTS
*******************************************************************************/

/** Record mark */
#define INTELHEXPARSER_RECORD_MARK  ':'

/*******************************************************************************
    MACROS
*******************************************************************************/

/*******************************************************************************
    TYPES AND STRUCTURES
*******************************************************************************/

/*******************************************************************************
    PROTOTYPES
*******************************************************************************/

static INTELHEXPARSER_RET intelHexParser_overstep(char const * const iHex, uint32_t num, uint32_t * const overstepped);
static INTELHEXPARSER_RET intelHexParser_parseHex8(char const * const iHex, uint8_t * const value);
static INTELHEXPARSER_RET intelHexParser_parseHex16(char const * const iHex, uint16_t * const value);
static INTELHEXPARSER_RET intelHexParser_verifyFormat(char const * const iHex, uint32_t * const recNum, size_t* const dataSize);

/*******************************************************************************
    LOCAL VARIABLES
*******************************************************************************/

/** Line number of the intel hex file, where the last error occurred. */
static uint32_t intelHexParser_line     = 0;

/** Column number of the intel hex file, where the last error occurred. */
static uint32_t intelHexParser_column   = 0;

/*******************************************************************************
    GLOBAL VARIABLES
*******************************************************************************/

/*******************************************************************************
    GLOBAL FUNCTIONS
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
extern INTELHEXPARSER_RET intelHexParser_load(char const * const fileName, intelHexParser_Record ** const recSet, uint32_t * const recNum)
{
    INTELHEXPARSER_RET  ret         = INTELHEXPARSER_RET_OK;
    FILE*               fd          = NULL;
    char*               buffer      = NULL;
    long                fileSize    = 0;
    size_t              read        = 0;

    if ((NULL == fileName) ||
        (NULL == recSet) ||
        (NULL == recNum))
    {
        return INTELHEXPARSER_RET_NULL;
    }

    /* Open file */
    fd = fopen(fileName, "rb");

    if (NULL == fd)
    {
        ret = INTELHEXPARSER_RET_FILE_NOT_FOUND;
    }
    else
    {
        /* Determine file size */
        (void)fseek(fd, 0L, SEEK_END);
        fileSize = ftell(fd);
        (void)fseek(fd, 0L, SEEK_SET);
        fileSize -= ftell(fd);

        /* Allocate memory for the whole file */
        buffer = (char*)malloc(fileSize);

        if (NULL == buffer)
        {
            ret = INTELHEXPARSER_RET_ERROR;
        }
        else
        {
            /* Load the whole file to memory */
            read = fread(buffer, sizeof(char), fileSize, fd);

            if (read != fileSize)
            {
                ret = INTELHEXPARSER_RET_ERROR;
            }
            else
            {
                /* Parse it */
                ret = intelHexParser_parse(buffer, recSet, recNum);
            }

            /* File buffer not needed anymore */
            free(buffer);
            buffer = NULL;
        }

        /* Close file */
        fclose(fd);
    }

    return ret;
}

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
extern INTELHEXPARSER_RET intelHexParser_parse(char const * const iHex, intelHexParser_Record ** const recSet, uint32_t * const recNum)
{
	char const *            walker      = iHex;
    uint32_t                line        = 0;
    uint32_t                column      = 0;
    INTELHEXPARSER_RET      ret         = INTELHEXPARSER_RET_OK;
    intelHexParser_Record   rec;
    size_t                  dataSize    = 0;
    uint8_t*                dataPtr     = NULL;
    uint8_t                 checksum    = 0;
    uint32_t                tmpRecNum   = 0;
    uint32_t                recIndex    = 0;
    uint32_t                index       = 0;

    if ((NULL == iHex) ||
        (NULL == recSet) ||
        (NULL == recNum))
    {
        return INTELHEXPARSER_RET_NULL;
    }

    *recSet = NULL;
    *recNum = 0;

    ret = intelHexParser_verifyFormat(iHex, &tmpRecNum, &dataSize);

    if (INTELHEXPARSER_RET_OK == ret)
    {
        /* Allocate memory for the whole record set, including the data. */
        dataPtr = (uint8_t*)malloc(sizeof(intelHexParser_Record) * tmpRecNum + dataSize);
        if (NULL == dataPtr)
        {
            ret = INTELHEXPARSER_RET_ERROR;
        }
        else
        {
            *recSet = (intelHexParser_Record*)dataPtr;
            *recNum = tmpRecNum;
            dataPtr = &dataPtr[sizeof(intelHexParser_Record) * tmpRecNum];

            for(recIndex = 0; recIndex < tmpRecNum; ++recIndex)
            {
                /* Clear record */
                memset(&rec, 0, sizeof(intelHexParser_Record));

                /* Clear checksum */
                checksum = 0;

                /* Overstep the record mark */
                ++column;

                /* Get data size */
                ret = intelHexParser_parseHex8(&walker[column], &rec.dataSize);
                if (INTELHEXPARSER_RET_OK != ret)
                {
                    /* Abort the record parser */
                    ret = INTELHEXPARSER_RET_INVALID_REC_LEN;
                    break;
                }
                column += 2;

                /* Get load offset */
                ret = intelHexParser_parseHex16(&walker[column], &rec.loadOffset);
                if (INTELHEXPARSER_RET_OK != ret)
                {
                    /* Abort the record parser */
                    ret = INTELHEXPARSER_RET_INVALID_REC_LOAD_OFFSET;
                    break;
                }
                column += 4;

                /* Get record type */
                ret = intelHexParser_parseHex8(&walker[column], &rec.type);
                if (INTELHEXPARSER_RET_OK != ret)
                {
                    /* Abort the record parser */
                    ret = INTELHEXPARSER_RET_INVALID_REC_TYPE;
                    break;
                }
                column += 2;

                /* Assign data memory */
                rec.data = dataPtr;

                /* Move data pointer to the next available memory. */
                dataPtr = &dataPtr[rec.dataSize];
                
                /* Calculate first part of checksum */
                checksum += rec.type;
                checksum += ((uint8_t*)&rec.loadOffset)[0];
                checksum += ((uint8_t*)&rec.loadOffset)[1];
                checksum += rec.dataSize;
                
                /* Get the data */
                for(index = 0; index < rec.dataSize; ++index)
                {
                    ret = intelHexParser_parseHex8(&walker[column], &rec.data[index]);
                    if (INTELHEXPARSER_RET_OK != ret)
                    {
                        LOG_DEBUG("Data error");
                        
                        /* Abort the for loop */
                        ret = INTELHEXPARSER_RET_INVALID_REC_DATA;
                        break;
                    }
                    column += 2;

                    /* Calculate second part of checksum */
                    checksum += rec.data[index];
                }

                if (INTELHEXPARSER_RET_OK != ret)
                {
                    /* Abort the record parser */
                    break;
                }

                /* Get checksum */
                ret = intelHexParser_parseHex8(&walker[column], &rec.checksum);
                if (INTELHEXPARSER_RET_OK != ret)
                {
                    /* Abort the record parser */
                    ret = INTELHEXPARSER_RET_INVALID_REC_CHECKSUM;
                    break;
                }

                /* Calculate third part of checksum */
                checksum  = (~checksum) + 1;    /* 2-complement */

                /* If checksum of the record is invalid, abort now. */
                if (checksum != rec.checksum)
                {
                    LOG_WARNING_UINT32("Record: ", recIndex + 1);
                    LOG_WARNING_UINT32("Invalid checksum: ", rec.checksum);
                    LOG_WARNING_UINT32("Expected checksum: ", checksum);
                    
                    /* Abort the record parser */
                    ret = INTELHEXPARSER_RET_INVALID_REC_CHECKSUM;
                    break;
                }

                column += 2;

                /* Check for line end */
                if (('\r' == walker[column]) &&
                    ('\n' == walker[column + 1]))
                {
                    column += 2;
                }
                else
                if ('\n' == walker[column])
                {
                    ++column;
                }
                else
                {
                    /* Abort */
                    ret = INTELHEXPARSER_RET_INVALID_REC_END;
                    break;
                }

                /* Copy record to record set */
                (*recSet)[recIndex] = rec;

                /* Next record */
                walker = &walker[column];
                column = 0;
                ++line;
            }
        }

        /* Free the record set and the data in case of an error. */
        if (INTELHEXPARSER_RET_OK != ret)
        {
            intelHexParser_line     = line + 1;
            intelHexParser_column   = column + 1;

            /* Destroy the record set and the data */
            if (NULL != *recSet)
            {
                free(*recSet);
                *recSet = NULL;
            }
        }
    }

    return ret;
}

/**
 * Free the record set.
 *
 * @param[in,out]   recSet  Record set
 */
extern void intelHexParser_freeRecSet(intelHexParser_Record** const recSet)
{
    if (NULL != recSet)
    {
        if (NULL != *recSet)
        {
            free(*recSet);
            *recSet = NULL;
        }
    }

    return;
}

/**
 * Get the location in the intel hex format string of the last error.
 *
 * @param[out]  line    Line number in the intel hex format string
 * @param[out]  column  Column number in the intel hex format string
 */
extern void intelHexParser_getLastError(uint32_t * const line, uint32_t * const column)
{
    if (NULL != line)
    {
        *line = intelHexParser_line;
    }

    if (NULL != column)
    {
        *column = intelHexParser_column;
    }

    return;
}

/*******************************************************************************
    LOCAL FUNCTIONS
*******************************************************************************/

/**
 * Overstep some characters in a single line of a intel hex format string.
 * It additional verify the character range.
 *
 * @param[in]  iHex         Intel hex format string
 * @param[in]  num          Number of characters to overstep
 * @param[out] overstepped  Overstepped characters
 *
 * @return Status of the operation
 */
static INTELHEXPARSER_RET intelHexParser_overstep(char const * const iHex, uint32_t num, uint32_t * const overstepped)
{
    INTELHEXPARSER_RET  ret     = INTELHEXPARSER_RET_OK;
    uint32_t            index   = 0;

    if ((NULL == iHex) ||
        (NULL == overstepped))
    {
        return INTELHEXPARSER_RET_NULL;
    }

    *overstepped = 0;
    while(('\0' != iHex[index]) && (0 < num))
    {
        if (('0' <= iHex[index]) &&
            ('9' >= iHex[index]))
        {
            /* Ok */
            ;
        }
        else
        if (('a' <= iHex[index]) &&
            ('f' >= iHex[index]))
        {
            /* Ok */
            ;
        }
        else
        if (('A' <= iHex[index]) &&
            ('F' >= iHex[index]))
        {
            /* Ok */
            ;
        }
        else
        /* Abort */
        {
            ret = INTELHEXPARSER_RET_INVALID_FORMAT;
            break;
        }

        *overstepped += 1;
        --num;
    }

    return ret;
}

/**
 * Parse a 8-bit hex value.
 *
 * @param[in]  iHex     Intel hex format string
 * @param[out] value    Unsigned 8-bit value
 *
 * @return Status of the operation
 */
static INTELHEXPARSER_RET intelHexParser_parseHex8(char const * const iHex, uint8_t * const value)
{
    uint8_t             index   = 0;
    INTELHEXPARSER_RET  ret     = INTELHEXPARSER_RET_OK;

    if ((NULL == iHex) ||
        (NULL == value))
    {
        return INTELHEXPARSER_RET_NULL;
    }

    *value = 0;

    for(index = 0; index < 2; ++index)
    {
        *value <<= 4;
        
        if (('0' <= iHex[index]) &&
            ('9' >= iHex[index]))
        {
            *value += iHex[index] - '0';
        }
        else
        if (('a' <= iHex[index]) &&
            ('f' >= iHex[index]))
        {
            *value += iHex[index] - 'a' + 10;
        }
        else
        if (('A' <= iHex[index]) &&
            ('F' >= iHex[index]))
        {
            *value += iHex[index] - 'A' + 10;
        }
        else
        {
            ret = INTELHEXPARSER_RET_INVALID_FORMAT;
            break;
        }
    }

    return ret;
}

/**
 * Parse a 16-bit hex value.
 *
 * @param[in]  iHex     Intel hex format string
 * @param[out] value    Unsigned 16-bit value
 *
 * @return Status of the operation
 */
static INTELHEXPARSER_RET intelHexParser_parseHex16(char const * const iHex, uint16_t * const value)
{
    INTELHEXPARSER_RET  ret     = INTELHEXPARSER_RET_OK;
    uint8_t             value8  = 0;

    if ((NULL == iHex) ||
        (NULL == value))
    {
        return INTELHEXPARSER_RET_NULL;
    }

    *value = 0;

    ret = intelHexParser_parseHex8(iHex, &value8);
    if (INTELHEXPARSER_RET_OK == ret)
    {
        *value = value8;
        *value <<= 8;

        ret = intelHexParser_parseHex8(&iHex[2], &value8);
        if (INTELHEXPARSER_RET_OK == ret)
        {
            *value |= value8;
        }
    }

    return ret;
}

/**
 * Verify a intel hex format string.
 *
 * @param[in]  iHex     Intel hex format string
 * @param[out] recNum   Number of round records
 * @param[out] dataSize Size of memory, necessary for data
 *
 * @return Status of the operation
 */
static INTELHEXPARSER_RET intelHexParser_verifyFormat(char const * const iHex, uint32_t * const recNum, size_t* const dataSize)
{
    char const *            walker      = iHex;
    uint32_t                line        = 0;
    uint32_t                column      = 0;
    INTELHEXPARSER_RET      ret         = INTELHEXPARSER_RET_OK;
    uint8_t                 recLen      = 0;
    INTELHEXPARSER_REC_TYPE recType     = INTELHEXPARSER_REC_TYPE_MAX;
    uint32_t                overstepped = 0;
    uint8_t                 value       = 0;

    if ((NULL == iHex) ||
        (NULL == recNum) ||
        (NULL == dataSize))
    {
        return INTELHEXPARSER_RET_NULL;
    }

    *recNum     = 0;
    *dataSize   = 0;

    /* Walk through the intel hex string, verify the intel hex format and
     * count the number of records and
     * sum all record data sizes.
     */
    while(('\0' != walker[column]) && (INTELHEXPARSER_REC_TYPE_EOF != recType))
    {
        /* Every record starts with a record mark */
        if (INTELHEXPARSER_RECORD_MARK != walker[column])
        {
            /* Abort the record parser */
            ret = INTELHEXPARSER_RET_MISSING_REC_MARK;
            break;
        }
        ++column;
        
        /* Get record length */
        ret = intelHexParser_parseHex8(&walker[column], &recLen);
        if (INTELHEXPARSER_RET_OK != ret)
        {
            /* Abort the record parser */
            ret = INTELHEXPARSER_RET_INVALID_REC_LEN;
            break;
        }
        column += 2;

        /* Overstep load offset */
        ret = intelHexParser_overstep(&walker[column], 4, &overstepped);
        column += overstepped;
        if (INTELHEXPARSER_RET_OK != ret)
        {
            /* Abort the record parser */
            ret = INTELHEXPARSER_RET_INVALID_REC_LOAD_OFFSET;
            break;
        }

        /* Get record type */
        ret = intelHexParser_parseHex8(&walker[column], &value);
        if (INTELHEXPARSER_RET_OK != ret)
        {
            /* Abort the record parser */
            ret = INTELHEXPARSER_RET_INVALID_REC_TYPE;
            break;
        }
        else
        if (INTELHEXPARSER_REC_TYPE_MAX <= value)
        {
            /* Abort the record parser */
            ret = INTELHEXPARSER_RET_INVALID_REC_TYPE;
            break;
        }
        column += 2;
        recType = value;

        /* Overstep data */
        ret = intelHexParser_overstep(&walker[column], 2 * recLen, &overstepped);
        column += overstepped;
        if (INTELHEXPARSER_RET_OK != ret)
        {
            /* Abort the record parser */
            ret = INTELHEXPARSER_RET_INVALID_REC_DATA;
            break;
        }

        /* Overstep checksum */
        ret = intelHexParser_overstep(&walker[column], 2, &overstepped);
        column += overstepped;
        if (INTELHEXPARSER_RET_OK != ret)
        {
            /* Abort the record parser */
            ret = INTELHEXPARSER_RET_INVALID_REC_CHECKSUM;
            break;
        }
        
        /* Check for line end */
        if (('\r' == walker[column]) &&
            ('\n' == walker[column + 1]))
        {
            column += 2;
        }
        else
        if ('\n' == walker[column])
        {
            ++column;
        }
        else if (INTELHEXPARSER_REC_TYPE_EOF != recType)
        {
            /* Abort */
            ret = INTELHEXPARSER_RET_INVALID_REC_END;
            break;
        }

        /* Update number of records and needed memory size */
        *recNum  += 1;
        *dataSize += recLen;

        /* Next record */
        walker = &walker[column];
        column = 0;
        ++line;
    }

    if (INTELHEXPARSER_RET_OK != ret)
    {
        intelHexParser_line     = line + 1;
        intelHexParser_column   = column + 1;
    }

    return ret;
}
