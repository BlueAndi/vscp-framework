<?xml version="1.0" encoding="UTF-8"?>

<!--

The MIT License (MIT)

Copyright (c) 2014 - 2015, Andreas Merkle
http://www.blue-andi.de
vscp@blue-andi.de

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

********************************************************************************
This transformation script generates the user persistency access interface.

$Author: amerkle $
$Rev: 455 $
$Date: 2015-01-06 20:30:09 +0100 (Di, 06 Jan 2015) $
********************************************************************************
-->

<!DOCTYPE stylesheet [
    <!ENTITY LF  "&#xA;" >
    <!ENTITY TAB "&#x9;" >
]>

<xsl:stylesheet
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:xs="http://www.w3.org/2001/XMLSchema"
    xmlns:fn="http://www.w3.org/2005/xpath-functions"
    xmlns:c="urn:schemas-microsoft-com:office:component:spreadsheet"
    xmlns:html="http://www.w3.org/TR/REC-html40"
    xmlns:o="urn:schemas-microsoft-com:office:office"
    xmlns:ss="urn:schemas-microsoft-com:office:spreadsheet"
    xmlns:x2="http://schemas.microsoft.com/office/excel/2003/xml"
    xmlns:x="urn:schemas-microsoft-com:office:excel"
    version="2.0">

    <!--
    ****************************************************************************
        INCLUDES
    ****************************************************************************
    -->
    <xsl:include href="../../../../tools/xslt/utilities/ctools.xsl"/> 
    
    <!--
    ****************************************************************************
        OUTPUT CONFIGURATION
    ****************************************************************************
    -->
    <xsl:output method="text" encoding="UTF-8" omit-xml-declaration="yes" indent="no" />

    <!--
    ****************************************************************************
        GLOBAL VARIABLES
    ****************************************************************************
    -->
    <xsl:variable name="global.moduleName">vscp_ps_user</xsl:variable>
    <xsl:variable name="global.author">Andreas Merkle, http://www.blue-andi.de</xsl:variable>
    <xsl:variable name="global.description">This module contains the user specific access to the persistent storage.</xsl:variable>
    
    <!--
    ****************************************************************************
        ROOT TEMPLATE
    ****************************************************************************
    -->
    <xsl:template match="/">
    
        <!-- Generate header file -->
        <xsl:result-document href="{$global.moduleName}.h">
            <xsl:call-template name="h.header" />
            <xsl:call-template name="h.includes" />
            <xsl:call-template name="h.compilerSwitches" />
            <xsl:call-template name="h.constants" />
            <xsl:call-template name="h.macros" />
            <xsl:call-template name="h.typesAndStructures" />
            <xsl:call-template name="h.variables" />
            <xsl:call-template name="h.functions" />
            <xsl:call-template name="h.footer" />
        </xsl:result-document>
    
        <!-- Generate source file -->
        <xsl:result-document href="{$global.moduleName}.c">
            <xsl:call-template name="c.header" />
            <xsl:call-template name="c.includes" />
            <xsl:call-template name="c.compilerSwitches" />
            <xsl:call-template name="c.constants" />
            <xsl:call-template name="c.macros" />
            <xsl:call-template name="c.typesAndStructures" />
            <xsl:call-template name="c.prototypes" />
            <xsl:call-template name="c.localVariables" />
            <xsl:call-template name="c.globalVariables" />
            <xsl:call-template name="c.globalFunctions" />
            <xsl:call-template name="c.localFunctions" />
            <xsl:call-template name="c.footer" />
        </xsl:result-document>
        
    </xsl:template>
    
    <!--
    ****************************************************************************
        HEADER FILE
    ****************************************************************************
    -->
    
    <!-- Header file header -->
    <xsl:template name="h.header">    
        <xsl:call-template name="ctools.hHeaderBlock">
            <xsl:with-param name="license">
            <xsl:text>/* The MIT License (MIT)
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

</xsl:text>
            </xsl:with-param>
            <xsl:with-param name="briefDesc">
                <xsl:text>User specific persistency access.</xsl:text>
            </xsl:with-param>        
            <xsl:with-param name="moduleName">
                <xsl:value-of select="$global.moduleName" />
            </xsl:with-param>
            <xsl:with-param name="author">
                <xsl:value-of select="$global.author" />
            </xsl:with-param>
            <xsl:with-param name="description">
                <xsl:value-of select="$global.description" />
            </xsl:with-param>
        </xsl:call-template>
        <xsl:text>&LF;</xsl:text>
    </xsl:template>
    
    <!-- Header file includes -->
    <xsl:template name="h.includes">
        <xsl:call-template name="ctools.generalBlock">
            <xsl:with-param name="name">
                <xsl:text>INCLUDES</xsl:text>
            </xsl:with-param>
        </xsl:call-template>
        <xsl:text>#include &lt;stdint.h&gt;&LF;</xsl:text>
        <xsl:text>#include "vscp_ps.h"&LF;</xsl:text>
        <xsl:text>&LF;</xsl:text>
    </xsl:template>
    
    <!-- Header file compiler switches -->
    <xsl:template name="h.compilerSwitches">
        <xsl:call-template name="ctools.generalBlock">
            <xsl:with-param name="name">
                <xsl:text>COMPILER SWITCHES</xsl:text>
            </xsl:with-param>
        </xsl:call-template>
        <xsl:text>&LF;</xsl:text>
    </xsl:template>
    
    <!-- Header file constants -->
    <xsl:template name="h.constants">
        <xsl:call-template name="ctools.generalBlock">
            <xsl:with-param name="name">
                <xsl:text>CONSTANTS</xsl:text>
            </xsl:with-param>
        </xsl:call-template>
        <xsl:text>&LF;</xsl:text>
        <xsl:apply-templates select="ss:Workbook/ss:Worksheet[@ss:Name='app_register']" mode="h.constants" />
    </xsl:template>
    
    <xsl:template match="ss:Worksheet[@ss:Name='app_register']" mode="h.constants">

        <!-- Get the persistency base offset. -->
        <xsl:variable name="psBaseOffset">
            <xsl:value-of select="ss:Table/ss:Row[6]/ss:Cell[2]/ss:Data" />
        </xsl:variable>
        
        <xsl:call-template name="ctools.define">
            <xsl:with-param name="comment">
                <xsl:text>Address where the user specific persistent values are stored.</xsl:text>
            </xsl:with-param>
            <xsl:with-param name="moduleName">
                <xsl:value-of select="$global.moduleName" />
            </xsl:with-param>
            <xsl:with-param name="name">
                <xsl:text>BASE ADDR</xsl:text>
            </xsl:with-param>
            <xsl:with-param name="value">
                <xsl:value-of select="$psBaseOffset" />
            </xsl:with-param>
        </xsl:call-template>
        <xsl:text>&LF;</xsl:text>
        
        <xsl:for-each select="ss:Table/ss:Row[(fn:position() > 8)]">
            <!-- Sort after page and register offset. -->
            <xsl:sort order="ascending" data-type="number" select="ss:Cell[1]/ss:Data" />
            <xsl:sort order="ascending" data-type="number" select="ss:Cell[2]/ss:Data" />
            
            <!-- Page -->
            <xsl:variable name="page" select="ss:Cell[1]/ss:Data" />
            <!-- Offset -->
            <xsl:variable name="offset" select="ss:Cell[2]/ss:Data" />
            <!-- Number of elements -->
            <xsl:variable name="numElements" select="ss:Cell[3]/ss:Data" />
            <!-- Element size -->
            <xsl:variable name="elementSize" select="ss:Cell[4]/ss:Data" />
            <!-- Short name -->
            <xsl:variable name="shortName" select="ss:Cell[5]/ss:Data" />
            <!-- Description -->
            <xsl:variable name="description" select="ss:Cell[6]/ss:Data" />
            <!-- Size -->
            <xsl:variable name="size" select="$numElements * $elementSize" />
        
            <xsl:if test="$page != '' and $offset != '' and $numElements != '' and $elementSize != '' and $shortName != ''">

                <!-- Generate #define for the address -->
                <xsl:call-template name="ctools.define">
                    <xsl:with-param name="comment">
                        <xsl:text>Persistent memory address: </xsl:text>
                        <xsl:value-of select="$shortName" />
                    </xsl:with-param>
                    <xsl:with-param name="moduleName">
                        <xsl:value-of select="$global.moduleName" />
                    </xsl:with-param>
                    <xsl:with-param name="name">
                        <xsl:text>ADDR </xsl:text>
                        <xsl:value-of select="$shortName" />
                    </xsl:with-param>
                    <xsl:with-param name="value">
                        <xsl:text>(VSCP_PS_USER_BASE_ADDR + </xsl:text>
                        <xsl:value-of select="$offset" />
                        <xsl:text>)</xsl:text>
                    </xsl:with-param>
                </xsl:call-template>
                <xsl:text>&LF;</xsl:text>
            
                <!-- Generate #define for the size -->
                <xsl:call-template name="ctools.define">
                    <xsl:with-param name="comment">
                        <xsl:text>Persistent memory size of: </xsl:text>
                        <xsl:value-of select="$shortName" />
                    </xsl:with-param>
                    <xsl:with-param name="moduleName">
                        <xsl:value-of select="$global.moduleName" />
                    </xsl:with-param>
                    <xsl:with-param name="name">
                        <xsl:text>SIZE </xsl:text>
                        <xsl:value-of select="$shortName" />
                    </xsl:with-param>
                    <xsl:with-param name="value">
                        <xsl:value-of select="$numElements * $elementSize" />
                    </xsl:with-param>
                </xsl:call-template>
                <xsl:text>&LF;</xsl:text>
            
            </xsl:if>
        
        </xsl:for-each>
        
    </xsl:template>
    
    <!-- Header file macros -->
    <xsl:template name="h.macros">
        <xsl:call-template name="ctools.generalBlock">
            <xsl:with-param name="name">
                <xsl:text>MACROS</xsl:text>
            </xsl:with-param>
        </xsl:call-template>
        <xsl:text>&LF;</xsl:text>
    </xsl:template>
    
    <!-- Header file types and structures -->
    <xsl:template name="h.typesAndStructures">
        <xsl:call-template name="ctools.generalBlock">
            <xsl:with-param name="name">
                <xsl:text>TYPES AND STRUCTURES</xsl:text>
            </xsl:with-param>
        </xsl:call-template>
        <xsl:text>&LF;</xsl:text>
    </xsl:template>
    
    <!-- Header file global variables -->
    <xsl:template name="h.variables">
        <xsl:call-template name="ctools.generalBlock">
            <xsl:with-param name="name">
                <xsl:text>VARIABLES</xsl:text>
            </xsl:with-param>
        </xsl:call-template>
        <xsl:text>&LF;</xsl:text>
    </xsl:template>
    
    <!-- Header file global functions -->
    <xsl:template name="h.functions">
        <xsl:call-template name="ctools.generalBlock">
            <xsl:with-param name="name">
                <xsl:text>FUNCTIONS</xsl:text>
            </xsl:with-param>
        </xsl:call-template>
        <xsl:text>&LF;</xsl:text>
        <xsl:apply-templates select="ss:Workbook/ss:Worksheet[@ss:Name='app_register']" mode="h.functions.init" />
        <xsl:text>&LF;</xsl:text>
        <xsl:apply-templates select="ss:Workbook/ss:Worksheet[@ss:Name='app_register']" mode="h.functions" />
    </xsl:template>
    
    <xsl:template match="ss:Worksheet[@ss:Name='app_register']" mode="h.functions.init">
    
        <xsl:call-template name="ctools.function">
            <xsl:with-param name="comment">
                <xsl:text>This function initializes this module.</xsl:text>
            </xsl:with-param>
            <xsl:with-param name="returnType">
                <xsl:text>extern void</xsl:text>
            </xsl:with-param>
            <xsl:with-param name="moduleName">
                <xsl:value-of select="$global.moduleName" />
            </xsl:with-param>
            <xsl:with-param name="name">
                <xsl:text>init</xsl:text>
            </xsl:with-param>
            <xsl:with-param name="parameter">
                <xsl:text>void</xsl:text>
            </xsl:with-param>
        </xsl:call-template>
    
    </xsl:template>
    
    <xsl:template match="ss:Worksheet[@ss:Name='app_register']" mode="h.functions">
       
        <xsl:for-each select="ss:Table/ss:Row[(fn:position() > 8)]">
            <!-- Sort after page and register offset. -->
            <xsl:sort order="ascending" data-type="number" select="ss:Cell[1]/ss:Data" />
            <xsl:sort order="ascending" data-type="number" select="ss:Cell[2]/ss:Data" />
            
            <!-- Page -->
            <xsl:variable name="page" select="ss:Cell[1]/ss:Data" />
            <!-- Offset -->
            <xsl:variable name="offset" select="ss:Cell[2]/ss:Data" />
            <!-- Number of elements -->
            <xsl:variable name="numElements" select="ss:Cell[3]/ss:Data" />
            <!-- Element size -->
            <xsl:variable name="elementSize" select="ss:Cell[4]/ss:Data" />
            <!-- Short name -->
            <xsl:variable name="shortName" select="ss:Cell[5]/ss:Data" />
            <!-- Description -->
            <xsl:variable name="description" select="ss:Cell[6]/ss:Data" />
            <!-- Size -->
            <xsl:variable name="size" select="$numElements * $elementSize" />
        
            <xsl:if test="$page != '' and $offset != '' and $numElements != '' and $elementSize != '' and $shortName != ''">
                
                <xsl:choose>
                    <!-- Number of elements is 1 -->
                    <xsl:when test="$size = 1">
                        
                        <!-- Read function -->
                        <xsl:call-template name="ctools.function">
                            <xsl:with-param name="comment">
                                <xsl:text>This function reads the </xsl:text>
                                <xsl:value-of select="$shortName" />
                                <xsl:text> from persistent storage.&LF;</xsl:text>
                                <xsl:value-of select="$description" />
                                <xsl:text>&LF;</xsl:text>
                                <xsl:text>&LF;</xsl:text>
                                <xsl:text>@return Value</xsl:text>
                            </xsl:with-param>
                            <xsl:with-param name="returnType">
                                <xsl:text>extern uint8_t</xsl:text>
                            </xsl:with-param>
                            <xsl:with-param name="moduleName">
                                <xsl:value-of select="$global.moduleName" />
                            </xsl:with-param>
                            <xsl:with-param name="name">
                                <xsl:text>read </xsl:text>
                                <xsl:value-of select="$shortName" />
                            </xsl:with-param>
                            <xsl:with-param name="parameter">
                                <xsl:text>void</xsl:text>
                            </xsl:with-param>
                        </xsl:call-template>
                        
                        <xsl:text>&LF;</xsl:text>

                        <!-- Write function -->
                        <xsl:call-template name="ctools.function">
                            <xsl:with-param name="comment">
                                <xsl:text>This function writes the </xsl:text>
                                <xsl:value-of select="$shortName" />
                                <xsl:text> to persistent storage.&LF;</xsl:text>
                                <xsl:value-of select="$description" />
                                <xsl:text>&LF;</xsl:text>
                                <xsl:text>&LF;</xsl:text>
                                <xsl:text>@param[in] value Value to write</xsl:text>
                            </xsl:with-param>
                            <xsl:with-param name="returnType">
                                <xsl:text>extern void</xsl:text>
                            </xsl:with-param>
                            <xsl:with-param name="moduleName">
                                <xsl:value-of select="$global.moduleName" />
                            </xsl:with-param>
                            <xsl:with-param name="name">
                                <xsl:text>write </xsl:text>
                                <xsl:value-of select="$shortName" />
                            </xsl:with-param>
                            <xsl:with-param name="parameter">
                                <xsl:text>uint8_t value</xsl:text>
                            </xsl:with-param>
                        </xsl:call-template>
                        
                    </xsl:when>
                    <!-- Number of elements are greater than 1 -->
                    <xsl:otherwise>
                    
                        <!-- Read function -->
                        <xsl:call-template name="ctools.function">
                            <xsl:with-param name="comment">
                                <xsl:text>This function reads the </xsl:text>
                                <xsl:value-of select="$shortName" />
                                <xsl:text> from persistent storage.&LF;</xsl:text>
                                <xsl:value-of select="$description" />
                                <xsl:text>&LF;</xsl:text>
                                <xsl:text>&LF;</xsl:text>
                                <xsl:text>@param[in] index Index&LF;</xsl:text>
                                <xsl:text>@return Value</xsl:text>
                            </xsl:with-param>
                            <xsl:with-param name="returnType">
                                <xsl:text>extern uint8_t</xsl:text>
                            </xsl:with-param>
                            <xsl:with-param name="moduleName">
                                <xsl:value-of select="$global.moduleName" />
                            </xsl:with-param>
                            <xsl:with-param name="name">
                                <xsl:text>read </xsl:text>
                                <xsl:value-of select="$shortName" />
                            </xsl:with-param>
                            <xsl:with-param name="parameter">
                                <xsl:text>uint8_t index</xsl:text>
                            </xsl:with-param>
                        </xsl:call-template>
                        
                        <xsl:text>&LF;</xsl:text>
                    
                        <!-- Write function -->
                        <xsl:call-template name="ctools.function">
                            <xsl:with-param name="comment">
                                <xsl:text>This function writes the </xsl:text>
                                <xsl:value-of select="$shortName" />
                                <xsl:text> to persistent storage.&LF;</xsl:text>
                                <xsl:value-of select="$description" />
                                <xsl:text>&LF;</xsl:text>
                                <xsl:text>&LF;</xsl:text>
                                <xsl:text>@param[in] index Index&LF;</xsl:text>
                                <xsl:text>@param[in] value Value to write</xsl:text>
                            </xsl:with-param>
                            <xsl:with-param name="returnType">
                                <xsl:text>extern void</xsl:text>
                            </xsl:with-param>
                            <xsl:with-param name="moduleName">
                                <xsl:value-of select="$global.moduleName" />
                            </xsl:with-param>
                            <xsl:with-param name="name">
                                <xsl:text>write </xsl:text>
                                <xsl:value-of select="$shortName" />
                            </xsl:with-param>
                            <xsl:with-param name="parameter">
                                <xsl:text>uint8_t index, uint8_t value</xsl:text>
                            </xsl:with-param>
                        </xsl:call-template>
                    
                    </xsl:otherwise>
                </xsl:choose>
                <xsl:text>&LF;</xsl:text>
                                
            </xsl:if>
            
        </xsl:for-each>
    
    </xsl:template>
    
    <!-- Header file footer -->
    <xsl:template name="h.footer">
        <xsl:call-template name="ctools.hFooterBlock">
            <xsl:with-param name="moduleName">
                <xsl:value-of select="$global.moduleName" />
            </xsl:with-param>
        </xsl:call-template>
    </xsl:template>
    
    <!--
    ****************************************************************************
        SOURCE FILE
    ****************************************************************************
    -->
    
    <!-- Source file header -->
    <xsl:template name="c.header">
        <xsl:call-template name="ctools.cHeaderBlock">
            <xsl:with-param name="license">
            <xsl:text>/* The MIT License (MIT)
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

</xsl:text>
            </xsl:with-param>
            <xsl:with-param name="briefDesc">
                <xsl:text>User specific persistency access.</xsl:text>
            </xsl:with-param>
            <xsl:with-param name="moduleName">
                <xsl:value-of select="$global.moduleName" />
            </xsl:with-param>
            <xsl:with-param name="author">
                <xsl:value-of select="$global.author" />
            </xsl:with-param>
        </xsl:call-template>
        <xsl:text>&LF;</xsl:text>
    </xsl:template>
    
    <!-- Source file includes -->
    <xsl:template name="c.includes">
        <xsl:call-template name="ctools.generalBlock">
            <xsl:with-param name="name">
                <xsl:text>INCLUDES</xsl:text>
            </xsl:with-param>
        </xsl:call-template>
        <xsl:text>#include "</xsl:text><xsl:value-of select="$global.moduleName" /><xsl:text>.h"&LF;</xsl:text>
        <xsl:text>#include "vscp_types.h"&LF;</xsl:text>
        <xsl:text>#include "vscp_ps_access.h"&LF;</xsl:text>
        <xsl:text>&LF;</xsl:text>
    </xsl:template>
    
    <!-- Source file compiler switches -->
    <xsl:template name="c.compilerSwitches">
        <xsl:call-template name="ctools.generalBlock">
            <xsl:with-param name="name">
                <xsl:text>COMPILER SWITCHES</xsl:text>
            </xsl:with-param>
        </xsl:call-template>
        <xsl:text>&LF;</xsl:text>
    </xsl:template>
    
    <!-- Source file constants -->
    <xsl:template name="c.constants">
        <xsl:call-template name="ctools.generalBlock">
            <xsl:with-param name="name">
                <xsl:text>CONSTANTS</xsl:text>
            </xsl:with-param>
        </xsl:call-template>
        <xsl:text>&LF;</xsl:text>
    </xsl:template>
    
    <!-- Source file macros -->
    <xsl:template name="c.macros">
        <xsl:call-template name="ctools.generalBlock">
            <xsl:with-param name="name">
                <xsl:text>MACROS</xsl:text>
            </xsl:with-param>
        </xsl:call-template>
        <xsl:text>&LF;</xsl:text>
    </xsl:template>
    
    <!-- Source file types and structures -->
    <xsl:template name="c.typesAndStructures">
        <xsl:call-template name="ctools.generalBlock">
            <xsl:with-param name="name">
                <xsl:text>TYPES AND STRUCTURES</xsl:text>
            </xsl:with-param>
        </xsl:call-template>
        <xsl:text>&LF;</xsl:text>
    </xsl:template>
    
    <!-- Source file prototypes -->
    <xsl:template name="c.prototypes">
        <xsl:call-template name="ctools.generalBlock">
            <xsl:with-param name="name">
                <xsl:text>PROTOTYPES</xsl:text>
            </xsl:with-param>
        </xsl:call-template>
        <xsl:text>&LF;</xsl:text>
    </xsl:template>
    
    <!-- Source file local variables -->
    <xsl:template name="c.localVariables">
        <xsl:call-template name="ctools.generalBlock">
            <xsl:with-param name="name">
                <xsl:text>LOCAL VARIABLES</xsl:text>
            </xsl:with-param>
        </xsl:call-template>
        <xsl:text>&LF;</xsl:text>
    </xsl:template>
    
    <!-- Source file global variables -->
    <xsl:template name="c.globalVariables">
        <xsl:call-template name="ctools.generalBlock">
            <xsl:with-param name="name">
                <xsl:text>GLOBAL VARIABLES</xsl:text>
            </xsl:with-param>
        </xsl:call-template>
        <xsl:text>&LF;</xsl:text>
    </xsl:template>
    
    <!-- Source file global functions -->
    <xsl:template name="c.globalFunctions">
        <xsl:call-template name="ctools.generalBlock">
            <xsl:with-param name="name">
                <xsl:text>GLOBAL FUNCTIONS</xsl:text>
            </xsl:with-param>
        </xsl:call-template>
        <xsl:text>&LF;</xsl:text>
        <xsl:apply-templates select="ss:Workbook/ss:Worksheet[@ss:Name='app_register']" mode="c.globalFunctions.init" />
        <xsl:text>&LF;</xsl:text>
        <xsl:apply-templates select="ss:Workbook/ss:Worksheet[@ss:Name='app_register']" mode="c.globalFunctions" />
    </xsl:template>
    
    <xsl:template match="ss:Worksheet[@ss:Name='app_register']" mode="c.globalFunctions.init">
    
        <xsl:call-template name="ctools.function">
            <xsl:with-param name="comment">
                <xsl:text>This function initializes this module.</xsl:text>
            </xsl:with-param>
            <xsl:with-param name="returnType">
                <xsl:text>extern void</xsl:text>
            </xsl:with-param>
            <xsl:with-param name="moduleName">
                <xsl:value-of select="$global.moduleName" />
            </xsl:with-param>
            <xsl:with-param name="name">
                <xsl:text>init</xsl:text>
            </xsl:with-param>
            <xsl:with-param name="parameter">
                <xsl:text>void</xsl:text>
            </xsl:with-param>
            <xsl:with-param name="body">
                <xsl:text>&TAB;/* Nothing to do. */&LF;</xsl:text>
                <xsl:text>&LF;</xsl:text>
                <xsl:text>&TAB;return;&LF;</xsl:text>
            </xsl:with-param>
        </xsl:call-template>
    
    </xsl:template>
    
    <xsl:template match="ss:Worksheet[@ss:Name='app_register']" mode="c.globalFunctions">
    
        <xsl:for-each select="ss:Table/ss:Row[(fn:position() > 8)]">
            <!-- Sort after page and register offset. -->
            <xsl:sort order="ascending" data-type="number" select="ss:Cell[1]/ss:Data" />
            <xsl:sort order="ascending" data-type="number" select="ss:Cell[2]/ss:Data" />
            
            <!-- Page -->
            <xsl:variable name="page" select="ss:Cell[1]/ss:Data" />
            <!-- Offset -->
            <xsl:variable name="offset" select="ss:Cell[2]/ss:Data" />
            <!-- Number of elements -->
            <xsl:variable name="numElements" select="ss:Cell[3]/ss:Data" />
            <!-- Element size -->
            <xsl:variable name="elementSize" select="ss:Cell[4]/ss:Data" />
            <!-- Short name -->
            <xsl:variable name="shortName" select="ss:Cell[5]/ss:Data" />
            <!-- Description -->
            <xsl:variable name="description" select="ss:Cell[6]/ss:Data" />
            <!-- Size -->
            <xsl:variable name="size" select="$numElements * $elementSize" />
        
            <xsl:if test="$page != '' and $offset != '' and $numElements != '' and $elementSize != '' and $shortName != ''">
                
                <!-- Build persistency access function name -->
                <xsl:variable name="ps.user.read.name">
                
                    <xsl:text>vscp_ps_user_</xsl:text>
                    <xsl:call-template name="ctools.hungarianForm">
                        <xsl:with-param name="string">
                            <xsl:text>read </xsl:text>
                            <xsl:value-of select="$shortName" />
                        </xsl:with-param>
                    </xsl:call-template>
                
                </xsl:variable>
                
                <!-- Build #define name of offset -->
                <xsl:variable name="addr.define.name">
                
                    <xsl:call-template name="ctools.define.name">
                        <xsl:with-param name="moduleName">
                            <xsl:value-of select="$global.moduleName" />
                        </xsl:with-param>
                        <xsl:with-param name="name">
                            <xsl:text>ADDR </xsl:text>
                            <xsl:value-of select="$shortName" />
                        </xsl:with-param>
                    </xsl:call-template>
                    
                </xsl:variable>
                
                <!-- Build #define name of size -->
                <xsl:variable name="size.define.name">
                
                    <xsl:call-template name="ctools.define.name">
                        <xsl:with-param name="moduleName">
                            <xsl:value-of select="$global.moduleName" />
                        </xsl:with-param>
                        <xsl:with-param name="name">
                            <xsl:text>SIZE </xsl:text>
                            <xsl:value-of select="$shortName" />
                        </xsl:with-param>
                    </xsl:call-template>
                    
                </xsl:variable>
                
                <xsl:choose>
                    <!-- Number of elements is 1 -->
                    <xsl:when test="$size = 1">
                        
                        <!-- Read function -->
                        <xsl:call-template name="ctools.function">
                            <xsl:with-param name="comment">
                                <xsl:text>This function reads the </xsl:text>
                                <xsl:value-of select="$shortName" />
                                <xsl:text> from persistent storage.&LF;</xsl:text>
                                <xsl:value-of select="$description" />
                                <xsl:text>&LF;</xsl:text>
                                <xsl:text>&LF;</xsl:text>
                                <xsl:text>@return Value</xsl:text>
                            </xsl:with-param>
                            <xsl:with-param name="returnType">
                                <xsl:text>extern uint8_t</xsl:text>
                            </xsl:with-param>
                            <xsl:with-param name="moduleName">
                                <xsl:value-of select="$global.moduleName" />
                            </xsl:with-param>
                            <xsl:with-param name="name">
                                <xsl:text>read </xsl:text>
                                <xsl:value-of select="$shortName" />
                            </xsl:with-param>
                            <xsl:with-param name="parameter">
                                <xsl:text>void</xsl:text>
                            </xsl:with-param>
                            <xsl:with-param name="body">
                                <xsl:text>&TAB;return vscp_ps_access_read8(</xsl:text>
                                <xsl:value-of select="$addr.define.name" />
                                <xsl:text>);&LF;</xsl:text>
                            </xsl:with-param>
                        </xsl:call-template>
                        
                        <xsl:text>&LF;</xsl:text>

                        <!-- Write function -->
                        <xsl:call-template name="ctools.function">
                            <xsl:with-param name="comment">
                                <xsl:text>This function writes the </xsl:text>
                                <xsl:value-of select="$shortName" />
                                <xsl:text> to persistent storage.&LF;</xsl:text>
                                <xsl:value-of select="$description" />
                                <xsl:text>&LF;</xsl:text>
                                <xsl:text>&LF;</xsl:text>
                                <xsl:text>@param[in] value Value to write</xsl:text>
                            </xsl:with-param>
                            <xsl:with-param name="returnType">
                                <xsl:text>extern void</xsl:text>
                            </xsl:with-param>
                            <xsl:with-param name="moduleName">
                                <xsl:value-of select="$global.moduleName" />
                            </xsl:with-param>
                            <xsl:with-param name="name">
                                <xsl:text>write </xsl:text>
                                <xsl:value-of select="$shortName" />
                            </xsl:with-param>
                            <xsl:with-param name="parameter">
                                <xsl:text>uint8_t value</xsl:text>
                            </xsl:with-param>
                            <xsl:with-param name="body">
                                <xsl:text>&TAB;vscp_ps_access_write8(</xsl:text>
                                <xsl:value-of select="$addr.define.name" />
                                <xsl:text>, value);&LF;</xsl:text>
                                <xsl:text>&LF;</xsl:text>
                                <xsl:text>&TAB;return;&LF;</xsl:text>
                            </xsl:with-param>
                        </xsl:call-template>
                        
                    </xsl:when>
                    <!-- Number of elements are greater than 1 -->
                    <xsl:otherwise>
                    
                        <!-- Read function -->
                        <xsl:call-template name="ctools.function">
                            <xsl:with-param name="comment">
                                <xsl:text>This function reads the </xsl:text>
                                <xsl:value-of select="$shortName" />
                                <xsl:text> from persistent storage.&LF;</xsl:text>
                                <xsl:value-of select="$description" />
                                <xsl:text>&LF;</xsl:text>
                                <xsl:text>&LF;</xsl:text>
                                <xsl:text>@param[in] index Index&LF;</xsl:text>
                                <xsl:text>@return Value</xsl:text>
                            </xsl:with-param>
                            <xsl:with-param name="returnType">
                                <xsl:text>extern uint8_t</xsl:text>
                            </xsl:with-param>
                            <xsl:with-param name="moduleName">
                                <xsl:value-of select="$global.moduleName" />
                            </xsl:with-param>
                            <xsl:with-param name="name">
                                <xsl:text>read </xsl:text>
                                <xsl:value-of select="$shortName" />
                            </xsl:with-param>
                            <xsl:with-param name="parameter">
                                <xsl:text>uint8_t index</xsl:text>
                            </xsl:with-param>
                            <xsl:with-param name="body">
                                <xsl:text>&TAB;uint8_t&TAB;value = 0;&LF;</xsl:text>
                                <xsl:text>&LF;</xsl:text>
                                <xsl:text>&TAB;if (</xsl:text>
                                <xsl:value-of select="$size.define.name" />
                                <xsl:text> &gt; index)&LF;</xsl:text>
                                <xsl:text>&TAB;{&LF;</xsl:text>
                                <xsl:text>&TAB;&TAB; value = vscp_ps_access_read8(</xsl:text>
                                <xsl:value-of select="$addr.define.name" />
                                <xsl:text> + index);&LF;</xsl:text>
                                <xsl:text>&TAB;}&LF;</xsl:text>
                                <xsl:text>&LF;</xsl:text>
                                <xsl:text>&TAB;return value;&LF;</xsl:text>
                            </xsl:with-param>
                        </xsl:call-template>
                        
                        <xsl:text>&LF;</xsl:text>
                    
                        <!-- Write function -->
                        <xsl:call-template name="ctools.function">
                            <xsl:with-param name="comment">
                                <xsl:text>This function writes the </xsl:text>
                                <xsl:value-of select="$shortName" />
                                <xsl:text> to persistent storage.&LF;</xsl:text>
                                <xsl:value-of select="$description" />
                                <xsl:text>&LF;</xsl:text>
                                <xsl:text>&LF;</xsl:text>
                                <xsl:text>@param[in] index Index&LF;</xsl:text>
                                <xsl:text>@param[in] value Value to write</xsl:text>
                            </xsl:with-param>
                            <xsl:with-param name="returnType">
                                <xsl:text>extern void</xsl:text>
                            </xsl:with-param>
                            <xsl:with-param name="moduleName">
                                <xsl:value-of select="$global.moduleName" />
                            </xsl:with-param>
                            <xsl:with-param name="name">
                                <xsl:text>write </xsl:text>
                                <xsl:value-of select="$shortName" />
                            </xsl:with-param>
                            <xsl:with-param name="parameter">
                                <xsl:text>uint8_t index, uint8_t value</xsl:text>
                            </xsl:with-param>
                            <xsl:with-param name="body">
                                <xsl:text>&TAB;if (</xsl:text>
                                <xsl:value-of select="$size.define.name" />
                                <xsl:text> &gt; index)&LF;</xsl:text>
                                <xsl:text>&TAB;{&LF;</xsl:text>
                                <xsl:text>&TAB;&TAB;vscp_ps_access_write8(</xsl:text>
                                <xsl:value-of select="$addr.define.name" />
                                <xsl:text> + index, value);&LF;</xsl:text>
                                <xsl:text>&TAB;}&LF;</xsl:text>
                                <xsl:text>&LF;</xsl:text>
                                <xsl:text>&TAB;return;&LF;</xsl:text>
                            </xsl:with-param>
                        </xsl:call-template>
                    
                    </xsl:otherwise>
                </xsl:choose>
                <xsl:text>&LF;</xsl:text>
                                
            </xsl:if>
            
        </xsl:for-each>
        
    </xsl:template>
    
    <!-- Source file local functions -->
    <xsl:template name="c.localFunctions">
        <xsl:call-template name="ctools.generalBlock">
            <xsl:with-param name="name">
                <xsl:text>LOCAL FUNCTIONS</xsl:text>
            </xsl:with-param>
        </xsl:call-template>
        <xsl:text>&LF;</xsl:text>
    </xsl:template>
    
    <!-- Source file footer -->
    <xsl:template name="c.footer">
        <xsl:call-template name="ctools.cFooterBlock" />
    </xsl:template>
    
    <!-- Garbage collector -->
    <xsl:template match="text()">
    </xsl:template>
    
</xsl:stylesheet>
