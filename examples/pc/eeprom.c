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
@brief  EEPROM simulation
@file   eeprom.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
@see eeprom.h

*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "eeprom.h"
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include "log.h"

/*******************************************************************************
    COMPILER SWITCHES
*******************************************************************************/

/*******************************************************************************
    CONSTANTS
*******************************************************************************/

#ifndef BOOL
/** Boolean type */
#define BOOL    unsigned char
#endif  /* Undefined BOOL */

#ifndef FALSE
/** Boolean false */
#define FALSE   0
#endif  /* Undefined FALSE */

#ifndef TRUE
/** Boolean true */
#define TRUE    1
#endif  /* Undefined TRUE */

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

/** EEPROM size in bytes */
static uint16_t eeprom_size     = 0;

/** EEPROM storage in the memory */
static uint8_t* eeprom_storage  = NULL;

/** Dirty flag of EEPROM */
static BOOL     eeprom_dirty    = FALSE;

/*******************************************************************************
    GLOBAL VARIABLES
*******************************************************************************/

/*******************************************************************************
    GLOBAL FUNCTIONS
*******************************************************************************/

/**
 * This function intializes the EEPROM simulation.
 *
 * @param[in] size  EEPROM size in bytes
 */
extern void eeprom_init(uint16_t size)
{
    if ((0 < size) &&
        (NULL == eeprom_storage))
    {
        /* EEPROM size must be a multiple of EEPROM_STORAGE_MULTIPLE */
        eeprom_size     = (size / EEPROM_STORAGE_MULTIPLE) * EEPROM_STORAGE_MULTIPLE;
        eeprom_size    += (0 < (size % EEPROM_STORAGE_MULTIPLE)) ? EEPROM_STORAGE_MULTIPLE : 0;
        eeprom_storage  = (uint8_t*) malloc(eeprom_size);

        if (NULL == eeprom_storage)
        {
            eeprom_size = 0;
        }
    }

    return;
}

/**
 * This function de-initializes the EEPROM simulation.
 */
extern void eeprom_deInit(void)
{
    if (NULL != eeprom_storage)
    {
        free(eeprom_storage);
        eeprom_storage = NULL;
    }

    return;
}

/**
 * Load file to EEPROM memory.
 *
 * @param[in] fileName  Filename of EEPROM file
 */
extern void eeprom_load(char* fileName)
{
    FILE    *fd     = NULL;
    BOOL    psReset = FALSE;

    if ((NULL == fileName) ||
        (NULL == eeprom_storage))
    {
        LOG_FATAL("Unexpected NULL pointer.");
        return;
    }

    /* If a EEPROM file exists, it will be loaded. */
    fd = fopen(fileName, "rb");

    if (NULL != fd)
    {
        long    fileSize    = 0;
        BOOL    error       = FALSE;

        /* EEPROM file size in bytes. It depends on the used format in ASCII. */
        const long  calcFileSize    = ((eeprom_size / EEPROM_STORAGE_MULTIPLE) * (3 * EEPROM_STORAGE_MULTIPLE + 7));

        LOG_INFO("EEPROM file found.");

        /* Determine file size */
        if (0 != fseek(fd, 0L, SEEK_END))
        {
            error = TRUE;
        }
        else
        {
            fileSize = ftell(fd);

            if (0 != fseek(fd, 0L, SEEK_SET))
            {
                error = TRUE;
            }
            else
            {
                fileSize -= ftell(fd);
            }
        }

        /* File will be loaded if no error happened and file size is correct. */
        if ((FALSE == error) &&
            (calcFileSize == fileSize))
        {
            uint16_t    index   = 0;

            for(index = 0; index < eeprom_size; ++index)
            {
                uint32_t    tmp = 0;

                if (0 == (index % EEPROM_STORAGE_MULTIPLE))
                {
                    /* Overstep address */
                    if (0 != fseek(fd, 6L, SEEK_CUR))
                    {
                        psReset = TRUE;
                    }
                }

                if (1 != fscanf(fd, "%02X", &tmp))
                {
                    psReset = TRUE;
                }

                if (255 >= tmp)
                {
                    eeprom_storage[index] = tmp;
                }
                else
                {
                    psReset = TRUE;
                }

                if (0 != ((index + 1) % EEPROM_STORAGE_MULTIPLE))
                {
                    /* Overstep space */
                    if (0 != fseek(fd, 1L, SEEK_CUR))
                    {
                        psReset = TRUE;
                    }
                }
                else
                {
                    /* Overstep CR LF */
                    if (0 != fseek(fd, 2L, SEEK_CUR))
                    {
                        psReset = TRUE;
                    }
                }

                if (TRUE == psReset)
                {
                    LOG_ERROR("Loading EEPROM file aborted.");
                    break;
                }
            }

            if (FALSE == psReset)
            {
                LOG_INFO("EEPROM file loaded.");
            }
        }
        else
        {
            LOG_WARNING("EEPROM file corrupt.");
            psReset = TRUE;
        }

        fclose(fd);
    }
    else
    {
        psReset = TRUE;
    }

    if (TRUE == psReset)
    {
        /* Reset EEPROM. */
        memset(eeprom_storage, 0xff, eeprom_size);

        eeprom_dirty = TRUE;
    }

    return;
}

/**
 * Save the EEPROM to file.
 *
 * @param[in] fileName  Filename of EEPROM file.
 */
extern void eeprom_save(char* fileName)
{
    if ((NULL == fileName) ||
        (NULL == eeprom_storage))
    {
        LOG_FATAL("Unexpected NULL pointer.");
        return;
    }

    /* Save only, if something has changed in the persistent memory. */
    if (FALSE != eeprom_dirty)
    {
        FILE    *fd = NULL;

        LOG_INFO("EEPROM will be stored to file.");

        fd = fopen(fileName, "wb");

        if (NULL == fd)
        {
            LOG_ERROR("Couldn't create EEPROM file.");
        }
        else
        {
            uint16_t    index       = 0;

            for(index = 0; index < eeprom_size; ++index)
            {
                if (0 == (index % EEPROM_STORAGE_MULTIPLE))
                {
                    (void)fprintf(fd, "%04X: ", index);
                }

                (void)fprintf(fd, "%02X", eeprom_storage[index]);

                if (0 != ((index + 1) % EEPROM_STORAGE_MULTIPLE))
                {
                    (void)fprintf(fd, " ");
                }
                else
                {
                    (void)fprintf(fd, "\r\n");
                }
            }

            LOG_INFO("EEPROM successful stored to file.");

            fclose(fd);
        }
    }

    return;
}

/**
 * Read a single byte from the EEPROM.
 *
 * @param[in]   addr    Address in EEPROM
 * @return  Value
 */
extern uint8_t  eeprom_read8(uint16_t addr)
{
    uint8_t data    = 0;

    if (eeprom_size > addr)
    {
        data = eeprom_storage[addr];
    }
    else
    {
        char    logBuffer[255];

        snprintf(logBuffer, 255, "Address is out of bounds: %u", addr);
        LOG_ERROR(logBuffer);
    }

    return data;
}

/**
 * Write a single byte to the EEPROM.
 *
 * @param[in]   addr    Address in EEPROM
 * @param[in]   value   Value to write
 */
extern void eeprom_write8(uint16_t addr, uint8_t value)
{
    if (eeprom_size > addr)
    {
        eeprom_storage[addr] = value;

        eeprom_dirty = TRUE;
    }
    else
    {
        char    logBuffer[255];

        snprintf(logBuffer, 255, "Address is out of bounds: %u", addr);
        LOG_ERROR(logBuffer);
    }

    return;
}

/**
 * Get base address of EEPROM storage in memory.
 *
 * @param[out] size EEPROM memory size
 * @return EEPROM memory base address
 */
extern uint8_t* eeprom_getBase(uint16_t* size)
{
    if (NULL != size)
    {
        *size = eeprom_size;
    }

    return eeprom_storage;
}

/**
 * Dump the EEPROM to console.
 */
extern void eeprom_dump(void)
{
    if (NULL == eeprom_storage)
    {
        printf("No EEPROM available.\n");
    }
    else
    {
        uint16_t    index   = 0;

        for(index = 0; index < eeprom_size; ++index)
        {
            if (0 == (index % EEPROM_STORAGE_MULTIPLE))
            {
                printf("%04X: ", index);
            }

            printf("%02X", eeprom_storage[index]);

            if (0 != ((index + 1) % EEPROM_STORAGE_MULTIPLE))
            {
                printf(" ");
            }
            else
            {
                printf("\r\n");
            }
        }
    }

    return;
}

/*******************************************************************************
    LOCAL FUNCTIONS
*******************************************************************************/
