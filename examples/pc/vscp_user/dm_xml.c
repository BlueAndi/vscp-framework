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
@brief  Decision matrix XML loader
@file   dm_xml.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
@see dm_xml.h

@section svn Subversion
$Author: amerkle $
$Rev: 449 $
$Date: 2015-01-05 20:23:52 +0100 (Mo, 05 Jan 2015) $
*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "dm_xml.h"
#include "log.h"
#include <stdio.h>
#include <string.h>
#include "expat.h"

/*******************************************************************************
    COMPILER SWITCHES
*******************************************************************************/

/*******************************************************************************
    CONSTANTS
*******************************************************************************/

/** Current standard decision matrix xml version */
#define DM_STD_XML_VERSION  "1.0"

/** Current decision matrix extension xml version */
#define DM_EXT_XML_VERSION  "1.0"

/** Current decision matrix next generation xml version */
#define DM_NG_XML_VERSION   "1.0"

/** Decision matrix level (node level) */
#define DM_LEVEL            "1"

/*******************************************************************************
    MACROS
*******************************************************************************/

/* Validate pointer */
#define DM_XML_VALIDATE_PTR(__ptr)  do{ if (NULL == (__ptr)) return DM_XML_RET_ENULL; }while(0)

/*******************************************************************************
    TYPES AND STRUCTURES
*******************************************************************************/

/** This type defines the decision matrix xml parser context. */
typedef struct
{
    XML_Parser          xmlParser;  /**< XML parser */

#if VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM )

    vscp_dm_MatrixRow*  dmStorage;  /**< Standard decision matrix storage */

#endif  /* VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM ) */

#if VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM_EXTENSION )

    vscp_dm_ExtRow*     extStorage; /**< Decision matrix extension storage */

#endif  /* VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM_EXTENSION ) */

    uint8_t             maxRows;    /**< Max. decision matrix rows */

    uint8_t             index;      /**< Decision matrix row index */
    uint8_t             id;         /**< Decision matrix extension: Data byte id */
    BOOL                dmFound;    /**< Flag: Decision matrix found */
    BOOL                rowFound;   /**< Flag: Decision matrix row found */
    BOOL                dataFound;  /**< Flag: Decision matrix extension data found */
    XML_Char*           buffer;     /**< Character buffer, used for the text between xml tags. */

    BOOL                isAborted;  /**< Parsing aborted or not */

} dm_xml_Context;

/*******************************************************************************
    PROTOTYPES
*******************************************************************************/

static long dm_xml_getFileSize(FILE* fd);
static char* dm_xml_loadFileToMem(char const * const fileName);

#if VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM )

static unsigned long dm_xml_getDataValue(char const * const str);
static char const * const dm_xml_getAttribute(const XML_Char **atts, char const * const name);
static DM_XML_RET dm_xml_parseStd(dm_xml_Context* const con, char const * const xml);
static void XMLCALL dm_xml_startElementStd(void *userData, const XML_Char *name, const XML_Char **atts);
static void XMLCALL dm_xml_endElementStd(void *userData, const XML_Char *name);
static void dm_xml_characterDataHandler(void *userData, const XML_Char *str, int len);

#endif  /* VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM ) */

#if VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM_EXTENSION )

static DM_XML_RET dm_xml_parseExt(dm_xml_Context* const con, char const * const xml);
static void XMLCALL dm_xml_startElementExt(void *userData, const XML_Char *name, const XML_Char **atts);
static void XMLCALL dm_xml_endElementExt(void *userData, const XML_Char *name);

#endif  /* VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM_EXTENSION ) */

/*******************************************************************************
    LOCAL VARIABLES
*******************************************************************************/

/*******************************************************************************
    GLOBAL VARIABLES
*******************************************************************************/

/*******************************************************************************
    GLOBAL FUNCTIONS
*******************************************************************************/

/**
 * This function initializes this module.
 */
extern void dm_xml_init(void)
{
    /* Implement your code here ... */

    return;
}

#if VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM )

/**
 * This function loads the content of a standard decision matrix xml file to the
 * specified memory location.
 *
 * @param[in]   fileName    Standard decision matrix xml file name
 * @param[out]  dm          Decision matrix storage
 * @param[in]   rows        Max. number of decision matrix rows in the storage
 * @return Status
 */
extern DM_XML_RET dm_xml_loadStd(char const * const fileName, vscp_dm_MatrixRow * const dm, uint8_t rows)
{
    DM_XML_RET      ret         = DM_XML_RET_OK;
    char*           fileBuffer  = NULL;
    dm_xml_Context  parserContext;

    DM_XML_VALIDATE_PTR(fileName);
    DM_XML_VALIDATE_PTR(dm);

    /* Load standard decision matrix xml file */
    fileBuffer = dm_xml_loadFileToMem(fileName);

    if (NULL == fileBuffer)
    {
        return DM_XML_RET_EFILE;
    }

    /* Clear standard decision matrix */
    memset(dm, 0, sizeof(vscp_dm_MatrixRow) * rows);

    /* Initialize parser context */
    memset(&parserContext, 0, sizeof(parserContext));

    parserContext.dmStorage = dm;
    parserContext.maxRows   = rows;

    ret = dm_xml_parseStd(&parserContext, fileBuffer);

    /* If any error happened, clear the standard decision matrix. */
    if ((DM_XML_RET_OK != ret) ||
        (TRUE == parserContext.isAborted))
    {
        /* Clear standard decision matrix */
        memset(dm, 0, sizeof(vscp_dm_MatrixRow) * rows);

        if ((DM_XML_RET_OK == ret) &&
            (TRUE == parserContext.isAborted))
        {
            ret = DM_XML_RET_ERROR;
        }
    }

    /* Free allocated ressources */
    free(fileBuffer);
    fileBuffer = NULL;

    return ret;
}

#endif  /* VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM ) */

#if VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM_EXTENSION )

/**
 * This function loads the content of a extended decision matrix xml file to the
 * specified memory locations.
 *
 * @param[in]   fileName    Extended decision matrix xml file name
 * @param[out]  dm          Decision matrix storage
 * @param[out]  ext         Decision matrix extension storage
 * @param[in]   rows        Max. number of decision matrix rows in the storage
 * @return Status
 */
extern DM_XML_RET dm_xml_loadExt(char const * const fileName, vscp_dm_MatrixRow * const dm, vscp_dm_ExtRow * const ext, uint8_t rows)
{
    DM_XML_RET      ret         = DM_XML_RET_OK;
    char*           fileBuffer  = NULL;
    dm_xml_Context  parserContext;

    DM_XML_VALIDATE_PTR(fileName);
    DM_XML_VALIDATE_PTR(dm);
    DM_XML_VALIDATE_PTR(ext);

    /* Load extended decision matrix xml file */
    fileBuffer = dm_xml_loadFileToMem(fileName);

    if (NULL == fileBuffer)
    {
        return DM_XML_RET_EFILE;
    }

    /* Clear extended decision matrix */
    memset(dm, 0, sizeof(vscp_dm_MatrixRow) * rows);
    memset(ext, 0, sizeof(vscp_dm_ExtRow) * rows);

    /* Initialize parser context */
    memset(&parserContext, 0, sizeof(parserContext));

    parserContext.dmStorage     = dm;
    parserContext.extStorage    = ext;
    parserContext.maxRows       = rows;

    ret = dm_xml_parseExt(&parserContext, fileBuffer);

    /* If any error happened, clear the standard decision matrix. */
    if ((DM_XML_RET_OK != ret) ||
        (TRUE == parserContext.isAborted))
    {
        /* Clear extended decision matrix */
        memset(dm, 0, sizeof(vscp_dm_MatrixRow) * rows);
        memset(ext, 0, sizeof(vscp_dm_ExtRow) * rows);

        if ((DM_XML_RET_OK == ret) &&
            (TRUE == parserContext.isAborted))
        {
            ret = DM_XML_RET_ERROR;
        }
    }

    /* Free allocated ressources */
    free(fileBuffer);
    fileBuffer = NULL;

    return ret;
}

#endif  /* VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM_EXTENSION ) */

/**
 * This function loads the content of a decision matrix next generation xml file to the
 * specified memory location.
 *
 * @param[in]   fileName    Eecision matrix next generation xml file name
 * @param[out]  storage     Decision matrix next generation storage
 * @param[in]   size        Decision matrix next generation storage size in bytes
 * @return Status
 */
extern DM_XML_RET dm_xml_loadNG(char const * const fileName, uint8_t* storage, uint8_t size)
{
    DM_XML_RET      ret         = DM_XML_RET_OK;
    char*           fileBuffer  = NULL;
    dm_xml_Context  parserContext;

    DM_XML_VALIDATE_PTR(fileName);
    DM_XML_VALIDATE_PTR(storage);

    /* Load decision matrix next generation xml file */
    fileBuffer = dm_xml_loadFileToMem(fileName);

    if (NULL == fileBuffer)
    {
        return DM_XML_RET_EFILE;
    }

    /* Initialize parser context */
    memset(&parserContext, 0, sizeof(parserContext));

    /* Free allocated ressources */
    free(fileBuffer);
    fileBuffer = NULL;

    return ret;
}

/*******************************************************************************
    LOCAL FUNCTIONS
*******************************************************************************/

/**
 * This function returns the file size.
 *
 * @param[in]   fd  File descriptor
 * @return File size in byte
 */
static long dm_xml_getFileSize(FILE* fd)
{
    long    fileSize    = 0;
    long    oldPos      = 0;
    BOOL    error       = FALSE;

    if (NULL == fd)
    {
        return 0;
    }

    /* Remember current position */
    oldPos = ftell(fd);

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

    if (TRUE == error)
    {
        fileSize = 0;
    }

    /* Jump back to old position */
    fseek(fd, oldPos, SEEK_SET);

    return fileSize;
}

/**
 * This function loads a file to memory.
 * The returned buffer will be '\0' terminated.
 * Don't forget to free the buffer later!
 *
 * @param[in]   fileName    Decision matrix xml file name
 * @return Pointer to buffer
 */
static char* dm_xml_loadFileToMem(char const * const fileName)
{
    FILE    *fd         = NULL;
    long    fileSize    = 0;
    char    *buffer     = NULL;

    /* If a decision matrix file exists, it will be loaded. */
    fd = fopen(fileName, "rb");

    /* No decision matrix file found? */
    if (NULL == fd)
    {
        return NULL;
    }

    /* Determine file size */
    fileSize = dm_xml_getFileSize(fd);

    /* Read the whole file to the memory and process it. */
    buffer = (char*)malloc(fileSize + 1);

    if (NULL != buffer)
    {
        /* Read whole file to memory */
        if (fileSize != fread(buffer, 1, fileSize, fd))
        {
            /* Error happened, clear buffer */
            free(buffer);
            buffer = NULL;
        }
        else
        {
            buffer[fileSize] = '\0';
        }
    }

    /* Close decision matrix file */
    fclose(fd);
    fd = NULL;

    return buffer;
}

#if VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM )

/**
 * This function returns a data value from the string.
 * The data can be coded as decimal value or hexa-decimal value.
 *
 * @param[in]   str Data string
 * @return Value
 */
static unsigned long dm_xml_getDataValue(char const * const str)
{
    unsigned long   value   = 0;
    char*           nstop   = NULL;

    if ((NULL != strchr(str, 'x' )) ||
        (NULL != strchr(str, 'X' )))
    {
        value = strtoul(str, &nstop, 16);
    }
    else
    {
        value = strtoul(str, &nstop, 10);
    }

    return value;
}

/**
 * This function searches for the given attribute name and returns it value.
 *
 * @param[in]   atts    Array of attributes
 * @param[in]   name    Attribute name
 * @return Attribute value
 */
static char const * const dm_xml_getAttribute(const XML_Char **atts, char const * const name)
{
    XML_Char const *    attrValue   = NULL;

    if (NULL != atts)
    {
        uint32_t    index   = 0;

        while(NULL != atts[index])
        {
            XML_Char const * const  attrName    = atts[index];

            attrValue = atts[index + 1];

            /* Internal error happened? */
            if ((NULL == attrName) ||
                (NULL == attrValue))
            {
                break;
            }
            /* Attribute found? */
            else if (0 == strcmp(attrName, name))
            {
                break;
            }

            index += 2;
        }
    }

    return attrValue;
}

/**
 * This function parse the standard decision matrix xml and writes it to the
 * defined storage.
 *
 * @param[in]   con Parser context
 * @param[in]   xml Decision matrix xml as terminated string.
 * @return Status
 * @retval FALSE    Failed
 * @retval TRUE     Successful
 */
static DM_XML_RET dm_xml_parseStd(dm_xml_Context* const con, char const * const xml)
{
    DM_XML_RET  ret = DM_XML_RET_OK;

    DM_XML_VALIDATE_PTR(con);
    DM_XML_VALIDATE_PTR(xml);

    /* Setup expat XML parser */
    con->xmlParser = XML_ParserCreate(NULL);

    XML_SetUserData(con->xmlParser, con);
    XML_SetElementHandler(con->xmlParser, dm_xml_startElementStd, dm_xml_endElementStd);
    XML_SetCharacterDataHandler(con->xmlParser, dm_xml_characterDataHandler);

    if (XML_STATUS_ERROR == XML_Parse(con->xmlParser, xml, strlen(xml), 1))
    {
        /* Parser error */
        LOG_ERROR_INT32(XML_ErrorString(XML_GetErrorCode(con->xmlParser)), XML_GetCurrentLineNumber(con->xmlParser));
        ret = DM_XML_RET_ERROR;
    }

    XML_ParserFree(con->xmlParser);

    /* Free allocated ressources */
    if (NULL != con->buffer)
    {
        free(con->buffer);
        con->buffer = NULL;
    }

    return ret;
}

/**
 * This function is called by the xml parser for every found element start (xml tag).
 *
 * @param[in]   userData    User data
 * @param[in]   name        Element name
 * @param[in]   atts        Array of attributes
 */
static void XMLCALL dm_xml_startElementStd(void *userData, const XML_Char *name, const XML_Char **atts)
{
    dm_xml_Context* con     = (dm_xml_Context*)userData;
    BOOL            abort   = FALSE;

    if ((NULL == userData) ||
        (NULL == name))
    {
        return;
    }

    if (0 == strcmp(name, "dm"))
    {
        XML_Char const *    attrValue       = NULL;
        BOOL                versionCorrect  = FALSE;
        BOOL                levelCorrect    = FALSE;
        BOOL                typeCorrect     = FALSE;

        con->dmFound = TRUE;

        /* Get the standard decision matrix file format version */
        attrValue = dm_xml_getAttribute(atts, "version");

        if (NULL == attrValue)
        {
            LOG_ERROR_INT32("Attribute version missing at line", XML_GetCurrentLineNumber(con->xmlParser));
        }
        else
        {
            /* Check the version */
            if (0 != strcmp(attrValue, DM_STD_XML_VERSION))
            {
                /* Wrong xml version */
                LOG_ERROR("Wrong standard decision matrix file format version.");
            }
            else
            {
                versionCorrect = TRUE;
            }
        }

        /* Get the decision matrix level */
        attrValue = dm_xml_getAttribute(atts, "level");

        if (NULL == attrValue)
        {
            LOG_ERROR_INT32("Attribute level missing at line", XML_GetCurrentLineNumber(con->xmlParser));
        }
        else
        {
            /* Check the level */
            if (0 != strcmp(attrValue, DM_LEVEL))
            {
                /* Wrong decision matrix level */
                LOG_ERROR("Wrong decision matrix level.");
            }
            else
            {
                levelCorrect = TRUE;
            }
        }

        /* Get the decision matrix type */
        attrValue = dm_xml_getAttribute(atts, "type");

        if (NULL == attrValue)
        {
            LOG_ERROR_INT32("Attribute type missing at line", XML_GetCurrentLineNumber(con->xmlParser));
        }
        else
        {
            /* Check the type */
            if (0 != strcmp(attrValue, "std"))
            {
                /* Wrong decision matrix type */
                LOG_ERROR("Wrong decision matrix type.");
            }
            else
            {
                typeCorrect = TRUE;
            }
        }

        /* All attributes are required */
        if ((FALSE == versionCorrect) ||
            (FALSE == levelCorrect) ||
            (FALSE == typeCorrect))
        {
            abort = TRUE;
        }
    }
    else if (FALSE == con->dmFound)
    {
        LOG_ERROR_INT32("Invalid standard decision matrix XML at line", XML_GetCurrentLineNumber(con->xmlParser));
        abort = TRUE;
    }
    else if (0 == strcmp(name, "row"))
    {
        XML_Char const *    attrValue   = NULL;

        con->rowFound = TRUE;

        /* Clear dm row */
        memset(&con->dmStorage[con->index], 0, sizeof(vscp_dm_MatrixRow));

        /* Is the decision matrix row enabled? */
        attrValue = dm_xml_getAttribute(atts, "enabled");

        if (NULL == attrValue)
        {
            LOG_ERROR_INT32("Attribute enabled missing at line", XML_GetCurrentLineNumber(con->xmlParser));
            abort = TRUE;
        }
        else
        {
            if (0 == strcmp(attrValue, "true"))
            {
                con->dmStorage[con->index].flags |= VSCP_DM_FLAG_ENABLE;
            }
        }
    }
    else if (FALSE == con->rowFound)
    {
        LOG_ERROR_INT32("Invalid standard decision matrix XML at line", XML_GetCurrentLineNumber(con->xmlParser));
        abort = TRUE;
    }
    else if (0 == strcmp(name, "mask"))
    {
        XML_Char const *    attrValue   = NULL;

        /* Get class mask */
        attrValue = dm_xml_getAttribute(atts, "class");

        if (NULL == attrValue)
        {
            LOG_ERROR_INT32("Attribute class missing at line", XML_GetCurrentLineNumber(con->xmlParser));
            abort = TRUE;
        }
        else
        {
            unsigned long value = dm_xml_getDataValue(attrValue);

            if (0x1ff >= value)
            {
                con->dmStorage[con->index].classMask = (uint8_t)(value & 0xff);

                if (0 != (value & 0x100))
                {
                    con->dmStorage[con->index].flags |= VSCP_DM_FLAG_CLASS_MASK_BIT8;
                }
            }
            else
            {
                LOG_ERROR_INT32("Attribute class out of range at line", XML_GetCurrentLineNumber(con->xmlParser));
                abort = TRUE;
            }
        }

        /* Get type mask */
        attrValue = dm_xml_getAttribute(atts, "type");

        if (NULL == attrValue)
        {
            LOG_ERROR_INT32("Attribute type missing at line", XML_GetCurrentLineNumber(con->xmlParser));
            abort = TRUE;
        }
        else
        {
            unsigned long value = dm_xml_getDataValue(attrValue);

            if (0xff >= value)
            {
                con->dmStorage[con->index].typeMask = value;
            }
            else
            {
                LOG_ERROR_INT32("Attribute type out of range at line", XML_GetCurrentLineNumber(con->xmlParser));
                abort = TRUE;
            }
        }
    }
    else if (0 == strcmp(name, "filter"))
    {
        XML_Char const *    attrValue   = NULL;

        /* Get class filter */
        attrValue = dm_xml_getAttribute(atts, "class");

        if (NULL == attrValue)
        {
            LOG_ERROR_INT32("Attribute class missing at line", XML_GetCurrentLineNumber(con->xmlParser));
            abort = TRUE;
        }
        else
        {
            unsigned long value = dm_xml_getDataValue(attrValue);

            if (0x1ff >= value)
            {
                con->dmStorage[con->index].classFilter = (uint8_t)(value & 0xff);

                if (0 != (value & 0x100))
                {
                    con->dmStorage[con->index].flags |= VSCP_DM_FLAG_CLASS_FILTER_BIT8;
                }
            }
            else
            {
                LOG_ERROR_INT32("Attribute class out of range at line", XML_GetCurrentLineNumber(con->xmlParser));
                abort = TRUE;
            }
        }

        /* Get type filter */
        attrValue = dm_xml_getAttribute(atts, "type");

        if (NULL == attrValue)
        {
            LOG_ERROR_INT32("Attribute type missing at line", XML_GetCurrentLineNumber(con->xmlParser));
            abort = TRUE;
        }
        else
        {
            unsigned long value = dm_xml_getDataValue(attrValue);

            if (0xff >= value)
            {
                con->dmStorage[con->index].typeFilter = value;
            }
            else
            {
                LOG_ERROR_INT32("Attribute type out of range at line", XML_GetCurrentLineNumber(con->xmlParser));
                abort = TRUE;
            }
        }
    }
    else if (0 == strcmp(name, "zone"))
    {
        XML_Char const *    attrValue   = NULL;

        /* Get zone enabled flag */
        attrValue = dm_xml_getAttribute(atts, "enabled");

        if (NULL == attrValue)
        {
            LOG_ERROR_INT32("Attribute enabled missing at line", XML_GetCurrentLineNumber(con->xmlParser));
            abort = TRUE;
        }
        else
        {
            if (0 == strcmp(attrValue, "true"))
            {
                con->dmStorage[con->index].flags |= VSCP_DM_FLAG_MATCH_ZONE;
            }
            else if (0 == strcmp(attrValue, "false"))
            {
                con->dmStorage[con->index].flags &= ~VSCP_DM_FLAG_MATCH_ZONE;
            }
            else
            {
                LOG_ERROR_INT32("Invalid value at line", XML_GetCurrentLineNumber(con->xmlParser));
                abort = TRUE;
            }
        }
    }
    else if (0 == strcmp(name, "subzone"))
    {
        XML_Char const *    attrValue   = NULL;

        /* Get sub-zone enabled flag */
        attrValue = dm_xml_getAttribute(atts, "enabled");

        if (NULL == attrValue)
        {
            LOG_ERROR_INT32("Attribute enabled missing at line", XML_GetCurrentLineNumber(con->xmlParser));
            abort = TRUE;
        }
        else
        {
            if (0 == strcmp(attrValue, "true"))
            {
                con->dmStorage[con->index].flags |= VSCP_DM_FLAG_MATCH_SUB_ZONE;
            }
            else if (0 == strcmp(attrValue, "false"))
            {
                con->dmStorage[con->index].flags &= ~VSCP_DM_FLAG_MATCH_SUB_ZONE;
            }
            else
            {
                LOG_ERROR_INT32("Invalid value at line", XML_GetCurrentLineNumber(con->xmlParser));
                abort = TRUE;
            }
        }
    }
    else if (0 == strcmp(name, "action"))
    {
        /* Nothing to do */
    }
    else if (0 == strcmp(name, "param"))
    {
        /* Nothing to do */
    }
    else if (0 == strcmp(name, "oaddr"))
    {
        XML_Char const *    attrValue   = NULL;

        /* Get originating address enabled flag */
        attrValue = dm_xml_getAttribute(atts, "enabled");

        if (NULL == attrValue)
        {
            LOG_ERROR_INT32("Attribute enabled missing at line", XML_GetCurrentLineNumber(con->xmlParser));
            abort = TRUE;
        }
        else
        {
            if (0 == strcmp(attrValue, "true"))
            {
                con->dmStorage[con->index].flags |= VSCP_DM_FLAG_CHECK_OADDR;
            }
            else if (0 == strcmp(attrValue, "false"))
            {
                con->dmStorage[con->index].flags &= ~VSCP_DM_FLAG_CHECK_OADDR;
            }
            else
            {
                LOG_ERROR_INT32("Invalid value at line", XML_GetCurrentLineNumber(con->xmlParser));
                abort = TRUE;
            }
        }
    }
    else if (0 == strcmp(name, "hardcoded"))
    {
        XML_Char const *    attrValue   = NULL;

        /* Get hardcoded node enabled flag */
        attrValue = dm_xml_getAttribute(atts, "enabled");

        if (NULL == attrValue)
        {
            LOG_ERROR_INT32("Attribute enabled missing at line", XML_GetCurrentLineNumber(con->xmlParser));
            abort = TRUE;
        }
        else
        {
            if (0 == strcmp(attrValue, "true"))
            {
                con->dmStorage[con->index].flags |= VSCP_DM_FLAG_HARDCODED;
            }
            else if (0 == strcmp(attrValue, "false"))
            {
                con->dmStorage[con->index].flags &= ~VSCP_DM_FLAG_HARDCODED;
            }
            else
            {
                LOG_ERROR_INT32("Invalid value at line", XML_GetCurrentLineNumber(con->xmlParser));
                abort = TRUE;
            }
        }
    }

    /* Abort parsing? */
    if (TRUE == abort)
    {
        con->isAborted = TRUE;

        XML_SetElementHandler(con->xmlParser, NULL, NULL);
        XML_SetCharacterDataHandler(con->xmlParser, NULL);
    }

    return;
}

/**
 * This function is called by the xml parser for every found element end (xml tag).
 *
 * @param[in]   userData    User data
 * @param[in]   name        Element name
 */
static void XMLCALL dm_xml_endElementStd(void *userData, const XML_Char *name)
{
    dm_xml_Context* con     = (dm_xml_Context*)userData;
    BOOL            abort   = FALSE;

    if ((NULL == userData) ||
        (NULL == name))
    {
        return;
    }

    if (0 == strcmp(name, "dm"))
    {
        con->dmFound = FALSE;
    }
    else if (FALSE == con->dmFound)
    {
        LOG_ERROR_INT32("Invalid standard decision matrix XML at line", XML_GetCurrentLineNumber(con->xmlParser));
        abort = TRUE;
    }
    else if (0 == strcmp(name, "row"))
    {
        /* Avoid decision matrix storage overflow by writing more rows than reserved for the decision matrix. */
        if (VSCP_CONFIG_DM_ROWS > con->index)
        {
            con->rowFound = FALSE;
            ++con->index;
        }
        else
        {
            LOG_ERROR("DM XML file contains more rows, than max. possible.");
            abort = TRUE;
        }
    }
    else if (FALSE == con->rowFound)
    {
        LOG_ERROR_INT32("Invalid standard decision matrix XML at line", XML_GetCurrentLineNumber(con->xmlParser));
        abort = TRUE;
    }
    else if (0 == strcmp(name, "mask"))
    {
        /* Nothing to do */
    }
    else if (0 == strcmp(name, "filter"))
    {
        /* Nothing to do */
    }
    else if (0 == strcmp(name, "zone"))
    {
        /* Nothing to do */
    }
    else if (0 == strcmp(name, "subzone"))
    {
        /* Nothing to do */
    }
    else if (0 == strcmp(name, "action"))
    {
        if (NULL != con->buffer)
        {
            unsigned long value = dm_xml_getDataValue(con->buffer);

            if (0xff >= value)
            {
                con->dmStorage[con->index].action = value;
            }
            else
            {
                LOG_ERROR_INT32("Invalid action value. Can only be in the range 0 - 255. Line", XML_GetCurrentLineNumber(con->xmlParser));
                abort = TRUE;
            }
        }
        else
        {
            LOG_ERROR_INT32("Missing action value. Line", XML_GetCurrentLineNumber(con->xmlParser));
            abort = TRUE;
        }
    }
    else if (0 == strcmp(name, "param"))
    {
        if (NULL != con->buffer)
        {
            unsigned long value = dm_xml_getDataValue(con->buffer);

            if (0xff >= value)
            {
                con->dmStorage[con->index].actionPar = value;
            }
            else
            {
                LOG_ERROR_INT32("Invalid action parameter value. Can only be in the range 0 - 255. Line", XML_GetCurrentLineNumber(con->xmlParser));
                abort = TRUE;
            }
        }
        else
        {
            LOG_ERROR_INT32("Missing param value. Line", XML_GetCurrentLineNumber(con->xmlParser));
            abort = TRUE;
        }
    }
    else if (0 == strcmp(name, "oaddr"))
    {
        if (NULL != con->buffer)
        {
            unsigned long value = dm_xml_getDataValue(con->buffer);

            if (0xff >= value)
            {
                con->dmStorage[con->index].oaddr = value;
            }
            else
            {
                LOG_ERROR_INT32("Invalid oaddr value. Can only be in the range 0 - 255. Line", XML_GetCurrentLineNumber(con->xmlParser));
                abort = TRUE;
            }
        }
        else
        {
            LOG_ERROR_INT32("Missing oaddr value. Line", XML_GetCurrentLineNumber(con->xmlParser));
            abort = TRUE;
        }
    }
    else if (0 == strcmp(name, "hardcoded"))
    {
        /* Nothing to do */
    }

    /* Abort parsing? */
    if (TRUE == abort)
    {
        con->isAborted = TRUE;

        XML_SetElementHandler(con->xmlParser, NULL, NULL);
        XML_SetCharacterDataHandler(con->xmlParser, NULL);
    }

    return;
}

/**
 * This function is called by the xml parser for every element text.
 *
 * @param[in]   userData    User data
 * @param[in]   str         Element text
 * @param[in]   len         Element text length
 */
static void dm_xml_characterDataHandler(void *userData, const XML_Char *str, int len)
{
    dm_xml_Context* con = (dm_xml_Context*)userData;

    if ((NULL == userData) ||
        (NULL == str))
    {
        return;
    }

    if (NULL != con->buffer)
    {
        free(con->buffer);
        con->buffer = NULL;
    }

    con->buffer = (XML_Char*)malloc(len + 1);

    if (NULL != con->buffer)
    {
        strncpy(con->buffer, str, len);
        con->buffer[len] = '\0';
    }

    return;
}

#endif  /* VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM ) */

#if VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM_EXTENSION )

/**
 * This function parse the decision matrix extension xml and writes it to the
 * defined storage.
 *
 * @param[in]   con Parser context
 * @param[in]   xml Decision matrix extension xml as terminated string.
 * @return Status
 * @retval FALSE    Failed
 * @retval TRUE     Successful
 */
static DM_XML_RET dm_xml_parseExt(dm_xml_Context* const con, char const * const xml)
{
    DM_XML_RET  ret = DM_XML_RET_OK;

    DM_XML_VALIDATE_PTR(con);
    DM_XML_VALIDATE_PTR(xml);

    /* Setup expat XML parser */
    con->xmlParser = XML_ParserCreate(NULL);

    XML_SetUserData(con->xmlParser, con);
    XML_SetElementHandler(con->xmlParser, dm_xml_startElementExt, dm_xml_endElementExt);
    XML_SetCharacterDataHandler(con->xmlParser, dm_xml_characterDataHandler);

    if (XML_STATUS_ERROR == XML_Parse(con->xmlParser, xml, strlen(xml), 1))
    {
        /* Parser error */
        LOG_ERROR_INT32(XML_ErrorString(XML_GetErrorCode(con->xmlParser)), XML_GetCurrentLineNumber(con->xmlParser));
        ret = DM_XML_RET_ERROR;
    }

    XML_ParserFree(con->xmlParser);

    /* Free allocated ressources */
    if (NULL != con->buffer)
    {
        free(con->buffer);
        con->buffer = NULL;
    }

    return ret;
}

/**
 * This function is called by the xml parser for every found element start (xml tag).
 *
 * @param[in]   userData    User data
 * @param[in]   name        Element name
 * @param[in]   atts        Array of attributes
 */
static void XMLCALL dm_xml_startElementExt(void *userData, const XML_Char *name, const XML_Char **atts)
{
    dm_xml_Context* con     = (dm_xml_Context*)userData;
    BOOL            abort   = FALSE;

    if ((NULL == userData) ||
        (NULL == name))
    {
        return;
    }

    if (0 == strcmp(name, "dm"))
    {
        XML_Char const *    attrValue       = NULL;
        BOOL                versionCorrect  = FALSE;
        BOOL                levelCorrect    = FALSE;
        BOOL                typeCorrect     = FALSE;

        con->dmFound = TRUE;

        /* Get the standard decision matrix file format version */
        attrValue = dm_xml_getAttribute(atts, "version");

        if (NULL == attrValue)
        {
            LOG_ERROR_INT32("Attribute version missing at line", XML_GetCurrentLineNumber(con->xmlParser));
        }
        else
        {
            /* Check the version */
            if (0 != strcmp(attrValue, DM_STD_XML_VERSION))
            {
                /* Wrong xml version */
                LOG_ERROR("Wrong extended decision matrix file format version.");
            }
            else
            {
                versionCorrect = TRUE;
            }
        }

        /* Get the decision matrix level */
        attrValue = dm_xml_getAttribute(atts, "level");

        if (NULL == attrValue)
        {
            LOG_ERROR_INT32("Attribute level missing at line", XML_GetCurrentLineNumber(con->xmlParser));
        }
        else
        {
            /* Check the level */
            if (0 != strcmp(attrValue, DM_LEVEL))
            {
                /* Wrong decision matrix level */
                LOG_ERROR("Wrong decision matrix level.");
            }
            else
            {
                levelCorrect = TRUE;
            }
        }

        /* Get the decision matrix type */
        attrValue = dm_xml_getAttribute(atts, "type");

        if (NULL == attrValue)
        {
            LOG_ERROR_INT32("Attribute type missing at line", XML_GetCurrentLineNumber(con->xmlParser));
        }
        else
        {
            /* Check the type */
            if (0 != strcmp(attrValue, "ext"))
            {
                /* Wrong decision matrix type */
                LOG_ERROR("Wrong decision matrix type.");
            }
            else
            {
                typeCorrect = TRUE;
            }
        }

        /* All attributes are required */
        if ((FALSE == versionCorrect) ||
            (FALSE == levelCorrect) ||
            (FALSE == typeCorrect))
        {
            abort = TRUE;
        }
    }
    else if (FALSE == con->dmFound)
    {
        LOG_ERROR_INT32("Invalid extended decision matrix XML at line", XML_GetCurrentLineNumber(con->xmlParser));
        abort = TRUE;
    }
    else if (0 == strcmp(name, "row"))
    {
        XML_Char const *    attrValue   = NULL;

        con->rowFound = TRUE;

        /* Clear dm row */
        memset(&con->dmStorage[con->index], 0, sizeof(vscp_dm_MatrixRow));
        memset(&con->extStorage[con->index], 0, sizeof(vscp_dm_ExtRow));

        /* Is the decision matrix row enabled? */
        attrValue = dm_xml_getAttribute(atts, "enabled");

        if (NULL == attrValue)
        {
            LOG_ERROR_INT32("Attribute enabled missing at line", XML_GetCurrentLineNumber(con->xmlParser));
            abort = TRUE;
        }
        else
        {
            if (0 == strcmp(attrValue, "true"))
            {
                con->dmStorage[con->index].flags |= VSCP_DM_FLAG_ENABLE;
            }
        }

        /* Standard of extended? */
        attrValue = dm_xml_getAttribute(atts, "type");

        if (NULL == attrValue)
        {
            LOG_ERROR_INT32("Attribute type missing at line", XML_GetCurrentLineNumber(con->xmlParser));
            abort = TRUE;
        }
        else
        {
            if (0 == strcmp(attrValue, "std"))
            {
                /* Nothing to do */
            }
            else if (0 == strcmp(attrValue, "ext"))
            {
                /* Extend the standard decision matrix row */
                con->dmStorage[con->index].action = VSCP_DM_ACTION_EXTENDED_DM;
            }
            else
            {
                LOG_ERROR_INT32("Attribute type is invalid line", XML_GetCurrentLineNumber(con->xmlParser));
                abort = TRUE;
            }
        }
    }
    else if (FALSE == con->rowFound)
    {
        LOG_ERROR_INT32("Invalid extended decision matrix XML at line", XML_GetCurrentLineNumber(con->xmlParser));
        abort = TRUE;
    }
    else if (0 == strcmp(name, "mask"))
    {
        XML_Char const *    attrValue   = NULL;

        /* Get class mask */
        attrValue = dm_xml_getAttribute(atts, "class");

        if (NULL == attrValue)
        {
            LOG_ERROR_INT32("Attribute class missing at line", XML_GetCurrentLineNumber(con->xmlParser));
            abort = TRUE;
        }
        else
        {
            unsigned long value = dm_xml_getDataValue(attrValue);

            if (0x1ff >= value)
            {
                con->dmStorage[con->index].classMask = (uint8_t)(value & 0xff);

                if (0 != (value & 0x100))
                {
                    con->dmStorage[con->index].flags |= VSCP_DM_FLAG_CLASS_MASK_BIT8;
                }
            }
            else
            {
                LOG_ERROR_INT32("Attribute class out of range at line", XML_GetCurrentLineNumber(con->xmlParser));
                abort = TRUE;
            }
        }

        /* Get type mask */
        attrValue = dm_xml_getAttribute(atts, "type");

        if (NULL == attrValue)
        {
            LOG_ERROR_INT32("Attribute type missing at line", XML_GetCurrentLineNumber(con->xmlParser));
            abort = TRUE;
        }
        else
        {
            unsigned long value = dm_xml_getDataValue(attrValue);

            if (0xff >= value)
            {
                con->dmStorage[con->index].typeMask = value;
            }
            else
            {
                LOG_ERROR_INT32("Attribute type out of range at line", XML_GetCurrentLineNumber(con->xmlParser));
                abort = TRUE;
            }
        }
    }
    else if (0 == strcmp(name, "filter"))
    {
        XML_Char const *    attrValue   = NULL;

        /* Get class filter */
        attrValue = dm_xml_getAttribute(atts, "class");

        if (NULL == attrValue)
        {
            LOG_ERROR_INT32("Attribute class missing at line", XML_GetCurrentLineNumber(con->xmlParser));
            abort = TRUE;
        }
        else
        {
            unsigned long value = dm_xml_getDataValue(attrValue);

            if (0x1ff >= value)
            {
                con->dmStorage[con->index].classFilter = (uint8_t)(value & 0xff);

                if (0 != (value & 0x100))
                {
                    con->dmStorage[con->index].flags |= VSCP_DM_FLAG_CLASS_FILTER_BIT8;
                }
            }
            else
            {
                LOG_ERROR_INT32("Attribute class out of range at line", XML_GetCurrentLineNumber(con->xmlParser));
                abort = TRUE;
            }
        }

        /* Get type filter */
        attrValue = dm_xml_getAttribute(atts, "type");

        if (NULL == attrValue)
        {
            LOG_ERROR_INT32("Attribute type missing at line", XML_GetCurrentLineNumber(con->xmlParser));
            abort = TRUE;
        }
        else
        {
            unsigned long value = dm_xml_getDataValue(attrValue);

            if (0xff >= value)
            {
                con->dmStorage[con->index].typeFilter = value;
            }
            else
            {
                LOG_ERROR_INT32("Attribute type out of range at line", XML_GetCurrentLineNumber(con->xmlParser));
                abort = TRUE;
            }
        }
    }
    else if (0 == strcmp(name, "zone"))
    {
        XML_Char const *    attrValue   = NULL;

        /* Get zone enabled flag */
        attrValue = dm_xml_getAttribute(atts, "enabled");

        if (NULL == attrValue)
        {
            LOG_ERROR_INT32("Attribute enabled missing at line", XML_GetCurrentLineNumber(con->xmlParser));
            abort = TRUE;
        }
        else
        {
            if (0 == strcmp(attrValue, "true"))
            {
                con->dmStorage[con->index].flags |= VSCP_DM_FLAG_MATCH_ZONE;
            }
            else if (0 == strcmp(attrValue, "false"))
            {
                con->dmStorage[con->index].flags &= ~VSCP_DM_FLAG_MATCH_ZONE;
            }
            else
            {
                LOG_ERROR_INT32("Invalid value at line", XML_GetCurrentLineNumber(con->xmlParser));
                abort = TRUE;
            }
        }
    }
    else if (0 == strcmp(name, "subzone"))
    {
        XML_Char const *    attrValue   = NULL;

        /* Get sub-zone enabled flag */
        attrValue = dm_xml_getAttribute(atts, "enabled");

        if (NULL == attrValue)
        {
            LOG_ERROR_INT32("Attribute enabled missing at line", XML_GetCurrentLineNumber(con->xmlParser));
            abort = TRUE;
        }
        else
        {
            if (0 == strcmp(attrValue, "true"))
            {
                con->dmStorage[con->index].flags |= VSCP_DM_FLAG_MATCH_SUB_ZONE;
            }
            else if (0 == strcmp(attrValue, "false"))
            {
                con->dmStorage[con->index].flags &= ~VSCP_DM_FLAG_MATCH_SUB_ZONE;
            }
            else
            {
                LOG_ERROR_INT32("Invalid value at line", XML_GetCurrentLineNumber(con->xmlParser));
                abort = TRUE;
            }
        }
    }
    else if (0 == strcmp(name, "action"))
    {
        /* Nothing to do */
    }
    else if (0 == strcmp(name, "param"))
    {
        /* Nothing to do */
    }
    else if (0 == strcmp(name, "oaddr"))
    {
        XML_Char const *    attrValue   = NULL;

        /* Get originating address enabled flag */
        attrValue = dm_xml_getAttribute(atts, "enabled");

        if (NULL == attrValue)
        {
            LOG_ERROR_INT32("Attribute enabled missing at line", XML_GetCurrentLineNumber(con->xmlParser));
            abort = TRUE;
        }
        else
        {
            if (0 == strcmp(attrValue, "true"))
            {
                con->dmStorage[con->index].flags |= VSCP_DM_FLAG_CHECK_OADDR;
            }
            else if (0 == strcmp(attrValue, "false"))
            {
                con->dmStorage[con->index].flags &= ~VSCP_DM_FLAG_CHECK_OADDR;
            }
            else
            {
                LOG_ERROR_INT32("Invalid value at line", XML_GetCurrentLineNumber(con->xmlParser));
                abort = TRUE;
            }
        }
    }
    else if (0 == strcmp(name, "hardcoded"))
    {
        XML_Char const *    attrValue   = NULL;

        /* Get hardcoded node enabled flag */
        attrValue = dm_xml_getAttribute(atts, "enabled");

        if (NULL == attrValue)
        {
            LOG_ERROR_INT32("Attribute enabled missing at line", XML_GetCurrentLineNumber(con->xmlParser));
            abort = TRUE;
        }
        else
        {
            if (0 == strcmp(attrValue, "true"))
            {
                con->dmStorage[con->index].flags |= VSCP_DM_FLAG_HARDCODED;
            }
            else if (0 == strcmp(attrValue, "false"))
            {
                con->dmStorage[con->index].flags &= ~VSCP_DM_FLAG_HARDCODED;
            }
            else
            {
                LOG_ERROR_INT32("Invalid value at line", XML_GetCurrentLineNumber(con->xmlParser));
                abort = TRUE;
            }
        }
    }
    else if (0 == strcmp(name, "data"))
    {
        con->dataFound = TRUE;
    }
    else if (0 == strcmp(name, "byte"))
    {
        if (FALSE == con->dataFound)
        {
            LOG_ERROR_INT32("Invalid extended decision matrix XML at line", XML_GetCurrentLineNumber(con->xmlParser));
            abort = TRUE;
        }
        else
        {
            XML_Char const *    attrValue   = NULL;

            /* Get id */
            attrValue = dm_xml_getAttribute(atts, "id");

            if (NULL == attrValue)
            {
                LOG_ERROR_INT32("Attribute id missing at line", XML_GetCurrentLineNumber(con->xmlParser));
                abort = TRUE;
            }
            else
            {
                unsigned long id  = dm_xml_getDataValue(attrValue);

                if ((0 == id) ||
                    ((3 <= id) && (5 >= id)))
                {
                    con->id = id;
                }
                else
                {
                    LOG_ERROR_INT32("Attribute id out of range at line", XML_GetCurrentLineNumber(con->xmlParser));
                    abort = TRUE;
                }
            }

            /* Get enabled flag */
            attrValue = dm_xml_getAttribute(atts, "enabled");

            if (NULL == attrValue)
            {
                LOG_ERROR_INT32("Attribute enabled missing at line", XML_GetCurrentLineNumber(con->xmlParser));
                abort = TRUE;
            }
            else
            {
                if (0 == strcmp(attrValue, "true"))
                {
                    if (0 == con->id)
                    {
                        con->dmStorage[con->index].actionPar |= VSCP_DM_EXTFLAG_MATCH_PAR_0;
                    }
                    else if (3 == con->id)
                    {
                        con->dmStorage[con->index].actionPar |= VSCP_DM_EXTFLAG_MATCH_PAR_3;
                    }
                    else if (4 == con->id)
                    {
                        con->dmStorage[con->index].actionPar |= VSCP_DM_EXTFLAG_MATCH_PAR_4;
                    }
                    else if (5 == con->id)
                    {
                        con->dmStorage[con->index].actionPar |= VSCP_DM_EXTFLAG_MATCH_PAR_5;
                    }
                }
                else if (0 == strcmp(attrValue, "false"))
                {
                    if (0 == con->id)
                    {
                        con->dmStorage[con->index].actionPar &= ~VSCP_DM_EXTFLAG_MATCH_PAR_0;
                    }
                    else if (3 == con->id)
                    {
                        con->dmStorage[con->index].actionPar &= ~VSCP_DM_EXTFLAG_MATCH_PAR_3;
                    }
                    else if (4 == con->id)
                    {
                        con->dmStorage[con->index].actionPar &= ~VSCP_DM_EXTFLAG_MATCH_PAR_4;
                    }
                    else if (5 == con->id)
                    {
                        con->dmStorage[con->index].actionPar &= ~VSCP_DM_EXTFLAG_MATCH_PAR_5;
                    }
                }
                else
                {
                    LOG_ERROR_INT32("Invalid value at line", XML_GetCurrentLineNumber(con->xmlParser));
                    abort = TRUE;
                }
            }
        }
    }

    /* Abort parsing? */
    if (TRUE == abort)
    {
        con->isAborted = TRUE;

        XML_SetElementHandler(con->xmlParser, NULL, NULL);
        XML_SetCharacterDataHandler(con->xmlParser, NULL);
    }

    return;
}

/**
 * This function is called by the xml parser for every found element end (xml tag).
 *
 * @param[in]   userData    User data
 * @param[in]   name        Element name
 */
static void XMLCALL dm_xml_endElementExt(void *userData, const XML_Char *name)
{
    dm_xml_Context* con     = (dm_xml_Context*)userData;
    BOOL            abort   = FALSE;

    if ((NULL == userData) ||
        (NULL == name))
    {
        return;
    }

    if (0 == strcmp(name, "dm"))
    {
        con->dmFound = FALSE;
    }
    else if (FALSE == con->dmFound)
    {
        LOG_ERROR_INT32("Invalid extended decision matrix XML at line", XML_GetCurrentLineNumber(con->xmlParser));
        abort = TRUE;
    }
    else if (0 == strcmp(name, "row"))
    {
        /* Avoid decision matrix storage overflow by writing more rows than reserved for the decision matrix. */
        if (VSCP_CONFIG_DM_ROWS > con->index)
        {
            con->rowFound = FALSE;
            ++con->index;
        }
        else
        {
            LOG_ERROR("DM XML file contains more rows, than max. possible.");
            abort = TRUE;
        }
    }
    else if (FALSE == con->rowFound)
    {
        LOG_ERROR_INT32("Invalid extended decision matrix XML at line", XML_GetCurrentLineNumber(con->xmlParser));
        abort = TRUE;
    }
    else if (0 == strcmp(name, "mask"))
    {
        /* Nothing to do */
    }
    else if (0 == strcmp(name, "filter"))
    {
        /* Nothing to do */
    }
    else if (0 == strcmp(name, "zone"))
    {
        if (VSCP_DM_ACTION_EXTENDED_DM == con->dmStorage[con->index].action)
        {
            if (NULL != con->buffer)
            {
                unsigned long value = dm_xml_getDataValue(con->buffer);

                if (0xff >= value)
                {
                    con->extStorage[con->index].zone = value;
                }
                else
                {
                    LOG_ERROR_INT32("Invalid zone value. Can only be in the range 0 - 255. Line", XML_GetCurrentLineNumber(con->xmlParser));
                    abort = TRUE;
                }
            }
            else
            {
                LOG_ERROR_INT32("Missing zone value. Line", XML_GetCurrentLineNumber(con->xmlParser));
                abort = TRUE;
            }
        }
    }
    else if (0 == strcmp(name, "subzone"))
    {
        if (VSCP_DM_ACTION_EXTENDED_DM == con->dmStorage[con->index].action)
        {
            if (NULL != con->buffer)
            {
                unsigned long value = dm_xml_getDataValue(con->buffer);

                if (0xff >= value)
                {
                    con->extStorage[con->index].subZone = value;
                }
                else
                {
                    LOG_ERROR_INT32("Invalid sub-zone value. Can only be in the range 0 - 255. Line", XML_GetCurrentLineNumber(con->xmlParser));
                    abort = TRUE;
                }
            }
            else
            {
                LOG_ERROR_INT32("Missing sub-zone value. Line", XML_GetCurrentLineNumber(con->xmlParser));
                abort = TRUE;
            }
        }
    }
    else if (0 == strcmp(name, "action"))
    {
        if (NULL != con->buffer)
        {
            unsigned long value = dm_xml_getDataValue(con->buffer);

            if (0xff >= value)
            {
                con->extStorage[con->index].action = value;
            }
            else
            {
                LOG_ERROR_INT32("Invalid action value. Can only be in the range 0 - 255. Line", XML_GetCurrentLineNumber(con->xmlParser));
                abort = TRUE;
            }
        }
        else
        {
            LOG_ERROR_INT32("Missing action value. Line", XML_GetCurrentLineNumber(con->xmlParser));
            abort = TRUE;
        }
    }
    else if (0 == strcmp(name, "param"))
    {
        if (NULL != con->buffer)
        {
            unsigned long value = dm_xml_getDataValue(con->buffer);

            if (0xff >= value)
            {
                con->extStorage[con->index].actionPar = value;
            }
            else
            {
                LOG_ERROR_INT32("Invalid action parameter value. Can only be in the range 0 - 255. Line", XML_GetCurrentLineNumber(con->xmlParser));
                abort = TRUE;
            }
        }
        else
        {
            LOG_ERROR_INT32("Missing param value. Line", XML_GetCurrentLineNumber(con->xmlParser));
            abort = TRUE;
        }
    }
    else if (0 == strcmp(name, "oaddr"))
    {
        if (NULL != con->buffer)
        {
            unsigned long value = dm_xml_getDataValue(con->buffer);

            if (0xff >= value)
            {
                con->dmStorage[con->index].oaddr = value;
            }
            else
            {
                LOG_ERROR_INT32("Invalid oaddr value. Can only be in the range 0 - 255. Line", XML_GetCurrentLineNumber(con->xmlParser));
                abort = TRUE;
            }
        }
        else
        {
            LOG_ERROR_INT32("Missing oaddr value. Line", XML_GetCurrentLineNumber(con->xmlParser));
            abort = TRUE;
        }
    }
    else if (0 == strcmp(name, "hardcoded"))
    {
        /* Nothing to do */
    }
    else if (0 == strcmp(name, "data"))
    {
        con->dataFound = FALSE;
    }
    else if (0 == strcmp(name, "byte"))
    {
        if (FALSE == con->dataFound)
        {
            LOG_ERROR_INT32("Invalid extended decision matrix XML at line", XML_GetCurrentLineNumber(con->xmlParser));
            abort = TRUE;
        }
        else
        {
            if (NULL != con->buffer)
            {
                unsigned long value = dm_xml_getDataValue(con->buffer);

                if (0xff >= value)
                {
                    if (0 == con->id)
                    {
                        con->extStorage[con->index].par0 = value;
                    }
                    else if (3 == con->id)
                    {
                        con->extStorage[con->index].par3 = value;
                    }
                    else if (4 == con->id)
                    {
                        con->extStorage[con->index].par4 = value;
                    }
                    else if (5 == con->id)
                    {
                        con->extStorage[con->index].par5 = value;
                    }
                }
                else
                {
                    LOG_ERROR_INT32("Invalid byte value. Can only be in the range 0 - 255. Line", XML_GetCurrentLineNumber(con->xmlParser));
                    abort = TRUE;
                }
            }
            else
            {
                LOG_ERROR_INT32("Missing byte value. Line", XML_GetCurrentLineNumber(con->xmlParser));
                abort = TRUE;
            }
        }
    }

    /* Abort parsing? */
    if (TRUE == abort)
    {
        con->isAborted = TRUE;

        XML_SetElementHandler(con->xmlParser, NULL, NULL);
        XML_SetCharacterDataHandler(con->xmlParser, NULL);
    }

    return;
}

#endif  /* VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM_EXTENSION ) */
