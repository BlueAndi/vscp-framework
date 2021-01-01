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
@brief  Command line parser
@file   cmdLineParser.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
@see cmdLineParser.h

*******************************************************************************/

/*******************************************************************************
	INCLUDES
*******************************************************************************/
#include "cmdLineParser.h"
#include <string.h>
#include <stdio.h>

/*******************************************************************************
	COMPILER SWITCHES
*******************************************************************************/

/*******************************************************************************
	CONSTANTS
*******************************************************************************/

/** Delimiter between several argument names */
#define CMDLINEPARSER_NAME_DELIMITER    ' '

/** Delimiter between several description lines */
#define CMDLINEPARSER_DESC_DELIMITER    '\n'

/*******************************************************************************
	MACROS
*******************************************************************************/

/*******************************************************************************
	TYPES AND STRUCTURES
*******************************************************************************/

/*******************************************************************************
	PROTOTYPES
*******************************************************************************/

static cmdLineParser_Arg const * cmdLineParser_find(cmdLineParser_Arg const * const possibleArgs, uint32_t num, char const * const name);
static uint32_t cmdLineParser_getNameMaxLen(cmdLineParser_Arg const * const config, uint32_t num);
static uint32_t cmdLineParser_showName(char const * const name);
static void cmdLineParser_showDescription(char const * const description, uint32_t max);

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
 * This function intializes the module.
 */
extern void cmdLineParser_init(void)
{
    /* Nothing to do */
    
    return;
}

/**
 * This function parses the command line arguments and handle them accordingly
 * to the given parser configuration.
 *
 * @param[in]   config  Parser configuration
 * @param[in]   num     Number of configuration elements
 * @param[in]   argc    Number of command line arguments
 * @param[in]   argv    Command line argument vector
 *
 * @return Status
 */
extern CMDLINEPARSER_RET cmdLineParser_parse(cmdLineParser_Arg const * const config, uint32_t num, int argc, char* argv[])
{
    CMDLINEPARSER_RET status = CMDLINEPARSER_RET_OK;
    
    if ((NULL == config) ||
        (NULL == argv))
    {
        status = CMDLINEPARSER_RET_ENULL;
    }
    /* Anything to parse? */
    else if ((0 < num) &&
             (0 < argc))
    {
        char const *                progName    = argv[0];
        cmdLineParser_Arg const *   clpArg      = NULL;
        int32_t                     index       = 0;
        uint32_t                    cIndex      = 0;
        BOOL                        withPar     = FALSE;
        
        /* Reset all boolean parameters in the configuration */
        for(cIndex = 0; cIndex < num; ++cIndex)
        {
            if (NULL != config[cIndex].boolPar)
            {
                *(config[cIndex].boolPar) = FALSE;
            }
        }
        
        /* Program name requested? */
        clpArg = cmdLineParser_find(config, num, CMDLINEPARSER_PROG_NAME_STD);
        
        if (NULL != clpArg)
        {
            if (NULL != clpArg->strPar)
            {
                *(clpArg->strPar) = progName;
            }
            
            if (NULL != clpArg->boolPar)
            {
                *(clpArg->boolPar) = TRUE;
            }
        
            if (NULL != clpArg->handler)
            {
                status = clpArg->handler(clpArg->userData, progName, NULL);
            }
        }
        
        /* Program name without path requested? */
        clpArg = cmdLineParser_find(config, num, CMDLINEPARSER_PROG_NAME_WP);
    
        if (NULL != clpArg)
        {
            progName = strrchr(progName, '/');
            
            if (NULL == progName)
            {
                progName = strrchr(argv[0], '\\');
            }
            
            if (NULL == progName)
            {
                progName = argv[0];
            }
            else
            {
                ++progName;
            }

            if (NULL != clpArg->strPar)
            {
                *(clpArg->strPar) = progName;
            }
            
            if (NULL != clpArg->boolPar)
            {
                *(clpArg->boolPar) = TRUE;
            }
            
            if (NULL != clpArg->handler)
            {
                status = clpArg->handler(clpArg->userData, progName, NULL);
            }
        }
        
        /* Walk through the command line arguments */
        index = 1;
        while((index < argc) && (CMDLINEPARSER_RET_OK == status))
        {
            clpArg = cmdLineParser_find(config, num, argv[index]);
            
            if (NULL != clpArg)
            {
                /* Does the argument expects a parameter */
                if (NULL == strchr(clpArg->name, '<'))
                {
                    withPar = FALSE;
                }
                else
                {
                    withPar = TRUE;
                }
                
                /* Less number of arguments available, than it should? */
                if ((TRUE == withPar) &&
                    (1 > (argc - index - 1)))
                {
                    status = CMDLINEPARSER_RET_ERROR;
                }
                else
                {
                    if ((TRUE == withPar) &&
                        (NULL != clpArg->strPar))
                    {
                        *(clpArg->strPar) = argv[index + 1];
                    }
                    
                    if (NULL != clpArg->boolPar)
                    {
                        *(clpArg->boolPar) = TRUE;
                    }

                    if (NULL != clpArg->handler)
                    {
                        if (FALSE == withPar)
                        {
                            status = clpArg->handler(clpArg->userData, argv[index], NULL);
                        }
                        else
                        {
                            status = clpArg->handler(clpArg->userData, argv[index], argv[index + 1]);
                        }
                    }
                }
                
                /* Overstep the argument parameter */
                if (TRUE == withPar)
                {
                    ++index;
                }
            }
            else
            {
                clpArg = cmdLineParser_find(config, num, CMDLINEPARSER_UNKONWN);
            
                if (NULL != clpArg)
                {                    
                    if (NULL != clpArg->handler)
                    {
                        status = clpArg->handler(clpArg->userData, argv[index], NULL);
                    }
                }
                else
                {
                    status = CMDLINEPARSER_RET_ERROR;
                }
            }

            /* Next argument */
            ++index;
        }
    }
    
    return status;
}

/**
 * This function shows all possible arguments with its description.
 *
 * @param[in]   config  Parser configuration
 * @param[in]   num     Number of configuration elements
 */
extern void cmdLineParser_show(cmdLineParser_Arg const * const config, uint32_t num)
{
    uint32_t                    index   = 0;
    cmdLineParser_Arg const *   arg     = NULL;
    uint32_t                    max     = cmdLineParser_getNameMaxLen(config, num) + 4;

    if (NULL == config)
    {
        return;
    }
    
    /* Show all possible program arguments */
    for(index = 0; index < num; ++index)
    {
        uint32_t    len         = 0;
        uint32_t    spaceIndex  = 0;
        
        arg = &config[index];
                
        if ((NULL != arg->name) &&
            (0 != strcmp(arg->name, CMDLINEPARSER_PROG_NAME_STD)) &&
            (0 != strcmp(arg->name, CMDLINEPARSER_PROG_NAME_WP)) &&
            (0 != strcmp(arg->name, CMDLINEPARSER_UNKONWN)))
        {
            /* Show argument name(s) */
            len = cmdLineParser_showName(arg->name);
            
            /* The description shall have a constant distance from the left
             * screen border.
             */
            for(spaceIndex = 0; spaceIndex < (max - len); ++spaceIndex)
            {
                printf(" ");
            }
            
            /* No description available? */
            if (NULL == arg->description)
            {
                printf("-- no description --\n");
            }
            else
            {
                /* Show the description */
                cmdLineParser_showDescription(arg->description, max);
            }
            
            /* Empty line after every argument */
            printf("\n");
        }
        else if (NULL == arg->name)
        {
            /* Show the description */
            cmdLineParser_showDescription(arg->description, max);            
        }
    }
    
    return;
}

/*******************************************************************************
	LOCAL FUNCTIONS
*******************************************************************************/

/**
 * This function search for the given name in the parser configuration and
 * returns the appropriate configuration element.
 *
 * @param[in]   config  Parser configuration
 * @param[in]   num     Number of configuration elements
 * @param[in]   name    Argument name
 *
 * @return Status
 */
static cmdLineParser_Arg const * cmdLineParser_find(cmdLineParser_Arg const * const config, uint32_t num, char const * const name)
{
    uint32_t                    index   = 0;
    cmdLineParser_Arg const *   found   = NULL;
    char const *                ptr     = NULL;

    /* Walk through all possible program arguments */
    for(index = 0; index < num; ++index)
    {
        cmdLineParser_Arg const *   configElem  = &config[index];
        uint32_t                    len         = 0;
        
        if (NULL != configElem->name)
        {
            ptr = configElem->name;
            while(('\0' != ptr[len]) && (NULL == found))
            {
                /* Determine argument name and note that a delimiter divides between
                 * a parameter or several arguments with equal meaning.
                 */
                len = 0;
                while(('\0' != ptr[len]) && (CMDLINEPARSER_NAME_DELIMITER != ptr[len]))
                {
                    ++len;
                }
                
                if ((len == strlen(name)) &&
                    (0 == strncmp(ptr, name, len)))
                {
                    found = configElem;
                }
                else
                /* Maybe a parameter or another argument follows */
                if (CMDLINEPARSER_NAME_DELIMITER == ptr[len])
                {
                    /* Overstep delimiter */
                    while(CMDLINEPARSER_NAME_DELIMITER == ptr[len])
                    {
                        ++len;
                    }
                    
                    /* Does a parameter follow? */
                    if ('<' == ptr[len])
                    {
                        /* Overstep the parameter */
                        while(('\0' != ptr[len]) && ('>' != ptr[len]))
                        {
                            ++len;
                        }
                        
                        if ('>' == ptr[len])
                        {
                            ++len;
                        }
                    }
                    
                    ptr = &ptr[len];
                    len = 0;
                }
            }
        }
        
        if (NULL != found)
        {
            break;
        }
    }
    
    return found;
}

/**
 * This function returns the length of the longest argument name.
 *
 * @param[in]   config  Parser configuration
 * @param[in]   num     Number of configuration elements
 *
 * @return Max. length of the argument name
 */
static uint32_t cmdLineParser_getNameMaxLen(cmdLineParser_Arg const * const config, uint32_t num)
{
    uint32_t                    index       = 0;
    cmdLineParser_Arg const *   arg         = NULL;
    char const *                ptr         = NULL;
    uint32_t                    max         = 0;
    BOOL                        parFound    = FALSE;
    
    if (NULL != config)
    {
        /* Walk through every possible command line argument */
        for(index = 0; index < num; ++index)
        {
            uint32_t    len = 0;
            
            arg = &config[index];
            
            if ((NULL != arg->name) &&
                (0 != strcmp(arg->name, CMDLINEPARSER_PROG_NAME_STD)) &&
                (0 != strcmp(arg->name, CMDLINEPARSER_PROG_NAME_WP)) &&
                (0 != strcmp(arg->name, CMDLINEPARSER_UNKONWN)))
            {
                ptr = arg->name;
                while('\0' != ptr[len])
                {
                    len = 0;
                    while(('\0' != ptr[len]) && (CMDLINEPARSER_NAME_DELIMITER != ptr[len]))
                    {
                        ++len;
                        
                        /* Parameter end found? */
                        if ((TRUE == parFound) &&
                            ('>' == ptr[len]))
                        {
                            parFound = FALSE;
                        }
                        /* Parameter found? */
                        else if ((FALSE == parFound) &&
                                 ('<' == ptr[len]))
                        {
                            parFound = TRUE;
                        }
                        /* Delimiter found? */
                        else if (CMDLINEPARSER_NAME_DELIMITER == ptr[len])
                        {
                            /* Does a parameter follow? */
                            if ('<' == ptr[len + 1])
                            {
                                ++len;
                                
                                parFound = TRUE;
                            }
                            /* Inside a parameter? */
                            else if (TRUE == parFound)
                            {
                                ++len;
                            }
                        }
                    }
                    
                    if (len > max)
                    {
                        max = len;
                    }
                    
                    /* Overstep delimiter */
                    if (CMDLINEPARSER_NAME_DELIMITER == ptr[len])
                    {
                        ++len;
                        ptr = &ptr[len];
                        len = 0;
                    }
                }
            }
        }
    }
    
    return max;
}

/**
 * This function shows a argument name.
 *
 * @param[in] name  Argument name
 *
 * @return Length of last argument name
 */
static uint32_t cmdLineParser_showName(char const * const name)
{
    uint32_t    index       = 0;
    uint32_t    len         = 0;
    BOOL        parFound    = FALSE;
    
    if (NULL == name)
    {
        return 0;
    }
    
    while('\0' != name[index])
    {
        printf("  ");
        
        while(('\0' != name[index]) && (CMDLINEPARSER_NAME_DELIMITER != name[index]))
        {
            printf("%c", name[index]);
            ++index;
            
            /* Parameter end found? */
            if ((TRUE == parFound) &&
                ('>' == name[index]))
            {
                parFound = FALSE;
            }
            /* Parameter found? */
            else if ((FALSE == parFound) &&
                     ('<' == name[index]))
            {
                parFound = TRUE;
            }
            /* Delimiter found? */
            else if (CMDLINEPARSER_NAME_DELIMITER == name[index])
            {
                /* Does a parameter follow? */
                if ('<' == name[index + 1])
                {
                    printf("%c", CMDLINEPARSER_NAME_DELIMITER);
                    ++index;
                    
                    parFound = TRUE;
                }
                /* Inside a parameter? */
                else if (TRUE == parFound)
                {
                    printf("%c", CMDLINEPARSER_NAME_DELIMITER);
                    ++index;
                }
            }
        }
        
        /* Overstep delimiter */
        if (CMDLINEPARSER_NAME_DELIMITER == name[index])
        {
            printf("\n");
            ++index;
            
            /* Calculate length of last shown name */
            len = index;
        }
        else
        {
            /* Calculate length of last shown name */
            len = index - len;
        }
    }
    
    return len;
}

/**
 * This function shows a argument description.
 *
 * @param[in] name  Argument description
 * @param[in] max   Number of spaces from left border
 */
static void cmdLineParser_showDescription(char const * const description, uint32_t max)
{
    uint32_t    index       = 0;
    uint32_t    spaceIndex  = 0;
    
    if (NULL == description)
    {
        return;
    }

    while('\0' != description[index])
    {
        /* Show description until string termination or a delimiter is found. */
        while(('\0' != description[index]) && (CMDLINEPARSER_DESC_DELIMITER != description[index]))
        {
            printf("%c", description[index]);
            ++index;
        }
        
        /* If a delimiter is found, overstep it. */
        if (CMDLINEPARSER_DESC_DELIMITER == description[index])
        {
            printf("\n");
            ++index;
            
            /* The description shall have a constant distance from the left
             * screen border.
             */
            printf("  ");
            for(spaceIndex = 0; spaceIndex < max; ++spaceIndex)
            {
                printf(" ");
            }
        }
    }
    
    printf("\n");
    
    return;
}
