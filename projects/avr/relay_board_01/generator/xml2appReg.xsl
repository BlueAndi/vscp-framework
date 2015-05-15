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
This transformation script generates the application register interface of the
VSCP framework.

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
    <xsl:variable name="global.moduleName">vscp_app_reg</xsl:variable>
    <xsl:variable name="global.author">Andreas Merkle, http://www.blue-andi.de</xsl:variable>
    <xsl:variable name="global.description">This module contains the access to the application specific registers.</xsl:variable>
    
    <!--
    ****************************************************************************
        ROOT TEMPLATE
    ****************************************************************************
    -->
    <xsl:template match="/">
    
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
                <xsl:text>VSCP application registers</xsl:text>
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
        <xsl:text>#include "vscp_ps_user.h"&LF;</xsl:text>
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
        <xsl:apply-templates select="ss:Workbook/ss:Worksheet[@ss:Name='app_register']" mode="c.constants" />
    </xsl:template>
    
    <xsl:template match="ss:Worksheet[@ss:Name='app_register']" mode="c.constants">
    
        <!-- Get the persistency base offset. -->
        <xsl:variable name="psBaseOffset">
            <xsl:value-of select="ss:Table/ss:Row[6]/ss:Cell[2]/ss:Data" />
        </xsl:variable>
    
        <!-- Walk through all rows with data -->
        <xsl:for-each select="ss:Table/ss:Row[fn:position() > 8]">
            
            <!-- Page -->
            <xsl:variable name="page" select="ss:Cell[1]/ss:Data" />
            <!-- Register offset -->
            <xsl:variable name="offset" select="ss:Cell[2]/ss:Data" />
            <!-- Persistency offset -->
            <xsl:variable name="persistencyOffset" select="ss:Cell[3]/ss:Data" />
            <!-- Number of elements -->
            <xsl:variable name="numElements" select="ss:Cell[4]/ss:Data" />
            <!-- Element size -->
            <xsl:variable name="elementSize" select="ss:Cell[5]/ss:Data" />
            <!-- Short name -->
            <xsl:variable name="shortName" select="ss:Cell[6]/ss:Data" />
            <!-- Description -->
            <xsl:variable name="description" select="ss:Cell[7]/ss:Data" />
            <!-- Factory default setting -->
            <xsl:variable name="default" select="ss:Cell[8]/ss:Data" />
            <!-- User persistency read -->
            <xsl:variable name="userPersistencyRead" select="ss:Cell[9]/ss:Data" />
            <!-- User persistency write -->
            <xsl:variable name="userPersistencyWrite" select="ss:Cell[10]/ss:Data" />
            
            <xsl:if test="$page != '' and $offset != '' and $numElements != '' and $elementSize != '' and $shortName != '' and $description != '' and $default != ''">
            
                <xsl:call-template name="ctools.define">
                
                    <xsl:with-param name="comment">
                        <xsl:text>Application register offset of page </xsl:text>
                        <xsl:value-of select="$page" />
                        <xsl:text> of </xsl:text>
                        <xsl:value-of select="$shortName" />
                        <xsl:text>.</xsl:text>
                    </xsl:with-param>
                    
                    <xsl:with-param name="moduleName">
                        <xsl:value-of select="$global.moduleName" />
                    </xsl:with-param>
                    
                    <xsl:with-param name="name">
                        <xsl:text>PAGE </xsl:text>
                        <xsl:value-of select="$page" />
                        <xsl:text> </xsl:text>
                        <xsl:text>OFFSET </xsl:text>
                        <xsl:value-of select="$shortName" />
                    </xsl:with-param>
                    
                    <xsl:with-param name="value">
                        <xsl:value-of select="$offset" />
                    </xsl:with-param>
                    
                </xsl:call-template>
                
                <xsl:text>&LF;</xsl:text>
                
            </xsl:if>
            
        </xsl:for-each>
        
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
        <xsl:apply-templates select="ss:Workbook/ss:Worksheet[@ss:Name='app_register']" mode="c.globalFunctions.restoreFactoryDefaultSettings" />
        <xsl:text>&LF;</xsl:text>
        <xsl:apply-templates select="ss:Workbook/ss:Worksheet[@ss:Name='app_register']" mode="c.globalFunctions.getPagesUsed" />
        <xsl:text>&LF;</xsl:text>
        <xsl:apply-templates select="ss:Workbook/ss:Worksheet[@ss:Name='app_register']" mode="c.globalFunctions.readRegister" />
        <xsl:text>&LF;</xsl:text>
        <xsl:apply-templates select="ss:Workbook/ss:Worksheet[@ss:Name='app_register']" mode="c.globalFunctions.writeRegister" />
        <xsl:text>&LF;</xsl:text>
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
                <xsl:text>&TAB;/* Initialize user specific persistent storage */&LF;</xsl:text>
                <xsl:text>&TAB;vscp_ps_user_init();&LF;</xsl:text>
                <xsl:text>&LF;</xsl:text>
                <xsl:text>&TAB;return;&LF;</xsl:text>
            </xsl:with-param>
        </xsl:call-template>
    
    </xsl:template>
    
    <xsl:template match="ss:Worksheet[@ss:Name='app_register']" mode="c.globalFunctions.getPagesUsed">
    
        <xsl:call-template name="ctools.function">
            <xsl:with-param name="comment">
                <xsl:text>This function returns the number of used pages.&LF;</xsl:text>
                <xsl:text>Its used in the register abstraction model.&LF;</xsl:text>
                <xsl:text>Deprecated since VSCP spec. v1.10.2&LF;</xsl:text>
                <xsl:text>&LF;</xsl:text>
                <xsl:text>@return Pages used&LF;</xsl:text>
                <xsl:text>@retval 0&TAB;More than 255 pages are used.</xsl:text>
            </xsl:with-param>
            <xsl:with-param name="returnType">
                <xsl:text>extern uint8_t</xsl:text>
            </xsl:with-param>
            <xsl:with-param name="moduleName">
                <xsl:value-of select="$global.moduleName" />
            </xsl:with-param>
            <xsl:with-param name="name">
                <xsl:text>get pages used</xsl:text>
            </xsl:with-param>
            <xsl:with-param name="parameter">
                <xsl:text>void</xsl:text>
            </xsl:with-param>
            <xsl:with-param name="body">
                <xsl:text>&TAB;uint8_t&TAB;pagesUsed = 1;&TAB;/* At least one page, which is mandatory. */&LF;</xsl:text>
                <xsl:text>&LF;</xsl:text>
                <xsl:text>&TAB;/* DM is at page 1 */&LF;</xsl:text>
                <xsl:text>&TAB;++pagesUsed;&LF;</xsl:text>
                <xsl:text>&LF;</xsl:text>
                <xsl:text>&TAB;return pagesUsed;&LF;</xsl:text>
            </xsl:with-param>
        </xsl:call-template>
    
    </xsl:template>
    
    <xsl:template match="ss:Worksheet[@ss:Name='app_register']" mode="c.globalFunctions.restoreFactoryDefaultSettings">
    
        <xsl:call-template name="ctools.function">
            <xsl:with-param name="comment">
                <xsl:text>Restore the application specific factory default settings.</xsl:text>
            </xsl:with-param>
            <xsl:with-param name="returnType">
                <xsl:text>extern void</xsl:text>
            </xsl:with-param>
            <xsl:with-param name="moduleName">
                <xsl:value-of select="$global.moduleName" />
            </xsl:with-param>
            <xsl:with-param name="name">
                <xsl:text>restore factory default settings</xsl:text>
            </xsl:with-param>
            <xsl:with-param name="parameter">
                <xsl:text>void</xsl:text>
            </xsl:with-param>
            <xsl:with-param name="body">
            
                <xsl:text>&TAB;uint8_t&TAB;index = 0;&LF;</xsl:text>
                <xsl:text>&LF;</xsl:text>
            
                <xsl:for-each select="ss:Table/ss:Row[(fn:position() > 8)]">
                    <!-- Sort after page and register offset. -->
                    <xsl:sort order="ascending" data-type="number" select="ss:Cell[1]/ss:Data" />
                    <xsl:sort order="ascending" data-type="number" select="ss:Cell[2]/ss:Data" />
                    
                    <!-- Page -->
                    <xsl:variable name="page" select="ss:Cell[1]/ss:Data" />
                    <!-- Register offset -->
                    <xsl:variable name="offset" select="ss:Cell[2]/ss:Data" />
                    <!-- Persistency offset -->
                    <xsl:variable name="persistencyOffset" select="ss:Cell[3]/ss:Data" />
                    <!-- Number of elements -->
                    <xsl:variable name="numElements" select="ss:Cell[4]/ss:Data" />
                    <!-- Element size -->
                    <xsl:variable name="elementSize" select="ss:Cell[5]/ss:Data" />
                    <!-- Short name -->
                    <xsl:variable name="shortName" select="ss:Cell[6]/ss:Data" />
                    <!-- Description -->
                    <xsl:variable name="description" select="ss:Cell[7]/ss:Data" />
                    <!-- Factory default setting -->
                    <xsl:variable name="default" select="ss:Cell[8]/ss:Data" />
                    <!-- User persistency read -->
                    <xsl:variable name="userPersistencyRead" select="ss:Cell[9]/ss:Data" />
                    <!-- User persistency write -->
                    <xsl:variable name="userPersistencyWrite" select="ss:Cell[10]/ss:Data" />
                    <!-- Size -->
                    <xsl:variable name="size" select="$numElements * $elementSize" />
                
                    <xsl:if test="$page != '' and $offset != '' and $numElements != '' and $elementSize != '' and $shortName != '' and $description != '' and $default != '' and not($userPersistencyWrite)">
                        
                        <!-- Build #define name of size -->
                        <xsl:variable name="size.define.name">
                        
                            <xsl:call-template name="ctools.define.name">
                                <xsl:with-param name="moduleName">
                                    <xsl:value-of select="'vscp_ps_user'" />
                                </xsl:with-param>
                                <xsl:with-param name="name">
                                    <xsl:text>SIZE </xsl:text>
                                    <xsl:value-of select="$shortName" />
                                </xsl:with-param>
                            </xsl:call-template>
                            
                        </xsl:variable>
                        
                        <!-- Build persistency write access function name -->
                        <xsl:variable name="ps.user.write.name">
                        
                            <xsl:text>vscp_ps_user_</xsl:text>
                            <xsl:call-template name="ctools.hungarianForm">
                                <xsl:with-param name="string">
                                    <xsl:text>write </xsl:text>
                                    <xsl:value-of select="$shortName" />
                                </xsl:with-param>
                            </xsl:call-template>
                        
                        </xsl:variable>
                        
                        <xsl:choose>
                            <xsl:when test="$elementSize = 1">
                            
                                <xsl:choose>
                                    <xsl:when test="$numElements = 1" >
                                        <xsl:text>&TAB;</xsl:text>
                                        <xsl:value-of select="$ps.user.write.name" />
                                        <xsl:text>(</xsl:text>
                                        <xsl:value-of select="$default" />
                                        <xsl:text>);&LF;</xsl:text>
                                    </xsl:when>
                                    <xsl:otherwise>
                                        <xsl:text>&TAB;for(index = 0; index &lt; </xsl:text>
                                        <xsl:value-of select="$size.define.name" />
                                        <xsl:text>; ++index)&LF;</xsl:text>
                                        <xsl:text>&TAB;{&LF;</xsl:text>
                                        <xsl:text>&TAB;&TAB;</xsl:text>
                                        <xsl:value-of select="$ps.user.write.name" />
                                        <xsl:text>(index, </xsl:text>
                                        <xsl:value-of select="$default" />
                                        <xsl:text>);&LF;</xsl:text>
                                        <xsl:text>&TAB;}&LF;</xsl:text>
                                    </xsl:otherwise>
                                </xsl:choose>
                                <xsl:text>&LF;</xsl:text>
                                
                            </xsl:when>
                            <xsl:when test="$elementSize = 2">
                            
                                <xsl:text>&TAB;for(index = 0; index &lt; </xsl:text>
                                <xsl:value-of select="$numElements" />
                                <xsl:text>; ++index)&LF;</xsl:text>
                                <xsl:text>&TAB;{&LF;</xsl:text>
                                <xsl:text>&TAB;&TAB;</xsl:text>
                                <xsl:value-of select="$ps.user.write.name" />
                                <xsl:text>(index * 2 + 0, (((uint16_t)</xsl:text>
                                <xsl:value-of select="$default" />
                                <xsl:text>) &gt;&gt; 0) &amp; 0xff);&LF;</xsl:text>
                                <xsl:text>&TAB;&TAB;</xsl:text>
                                <xsl:value-of select="$ps.user.write.name" />
                                <xsl:text>(index * 2 + 1, (((uint16_t)</xsl:text>
                                <xsl:value-of select="$default" />
                                <xsl:text>) &gt;&gt; 8) &amp; 0xff);&LF;</xsl:text>
                                <xsl:text>&TAB;}&LF;</xsl:text>
                                <xsl:text>&LF;</xsl:text>
                                
                            </xsl:when>
                            <xsl:when test="$elementSize = 4">
                            
                                <xsl:text>&TAB;for(index = 0; index &lt; </xsl:text>
                                <xsl:value-of select="$numElements" />
                                <xsl:text>; ++index)&LF;</xsl:text>
                                <xsl:text>&TAB;{&LF;</xsl:text>
                                <xsl:text>&TAB;&TAB;</xsl:text>
                                <xsl:value-of select="$ps.user.write.name" />
                                <xsl:text>(index * 4 + 0, (((uint32_t)</xsl:text>
                                <xsl:value-of select="$default" />
                                <xsl:text>) &gt;&gt; 0) &amp; 0xff);&LF;</xsl:text>
                                <xsl:text>&TAB;&TAB;</xsl:text>
                                <xsl:value-of select="$ps.user.write.name" />
                                <xsl:text>(index * 4 + 1, (((uint32_t)</xsl:text>
                                <xsl:value-of select="$default" />
                                <xsl:text>) &gt;&gt; 8) &amp; 0xff);&LF;</xsl:text>
                                <xsl:text>&TAB;&TAB;</xsl:text>
                                <xsl:value-of select="$ps.user.write.name" />
                                <xsl:text>(index * 4 + 2, (((uint32_t)</xsl:text>
                                <xsl:value-of select="$default" />
                                <xsl:text>) &gt;&gt; 16) &amp; 0xff);&LF;</xsl:text>
                                <xsl:text>&TAB;&TAB;</xsl:text>
                                <xsl:value-of select="$ps.user.write.name" />
                                <xsl:text>(index * 4 + 3, (((uint32_t)</xsl:text>
                                <xsl:value-of select="$default" />
                                <xsl:text>) &gt;&gt; 24) &amp; 0xff);&LF;</xsl:text>
                                <xsl:text>&TAB;}&LF;</xsl:text>
                                <xsl:text>&LF;</xsl:text>
                                
                            </xsl:when>
                            <xsl:otherwise>
                            </xsl:otherwise>
                        </xsl:choose>
                    
                    </xsl:if>
                    
                </xsl:for-each>
            
                <xsl:text>&TAB;return;&LF;</xsl:text>
                
            </xsl:with-param>
        </xsl:call-template>
    
    </xsl:template>
    
    <xsl:template match="ss:Worksheet[@ss:Name='app_register']" mode="c.globalFunctions.readRegister">
        
        <xsl:call-template name="ctools.function">
            <xsl:with-param name="comment">
                <xsl:text>This function reads a application specific register and returns the value.&LF;</xsl:text>
                <xsl:text>&LF;</xsl:text>
                <xsl:text>@param[in]&TAB;page&TAB;Page&LF;</xsl:text>
                <xsl:text>@param[in]&TAB;addr&TAB;Register address</xsl:text>
                <xsl:text>@return Register value</xsl:text>
            </xsl:with-param>
            <xsl:with-param name="returnType">
                <xsl:text>extern uint8_t</xsl:text>
            </xsl:with-param>
            <xsl:with-param name="moduleName">
                <xsl:value-of select="$global.moduleName" />
            </xsl:with-param>
            <xsl:with-param name="name">
                <xsl:text>read register</xsl:text>
            </xsl:with-param>
            <xsl:with-param name="parameter">
                <xsl:text>uint16_t page, uint8_t addr</xsl:text>
            </xsl:with-param>
            <xsl:with-param name="body">
                <xsl:text>&TAB;uint8_t&TAB;value = 0;&LF;</xsl:text>
                <xsl:text>&LF;</xsl:text>
                <xsl:text>&TAB;if (0 == page)&LF;</xsl:text>
                <xsl:text>&TAB;{&LF;</xsl:text>
                <xsl:text>&TAB;&TAB;if ((VSCP_REGISTER_APP_START_ADDR &lt;= addr) &amp;&amp;&LF;</xsl:text>
                <xsl:text>&TAB;&TAB;    (VSCP_REGISTER_APP_END_ADDR &gt;= addr))&LF;</xsl:text>
                <xsl:text>&TAB;&TAB;{&LF;</xsl:text>
        
                <xsl:for-each select="ss:Table/ss:Row[(fn:position() > 8) and (fn:number(ss:Cell[1]/ss:Data)) = 0]">
                    <!-- Sort after page and register offset. -->
                    <xsl:sort order="ascending" data-type="number" select="ss:Cell[1]/ss:Data" />
                    <xsl:sort order="ascending" data-type="number" select="ss:Cell[2]/ss:Data" />
                    
                    <!-- Page -->
                    <xsl:variable name="page" select="ss:Cell[1]/ss:Data" />
                    <!-- Register offset -->
                    <xsl:variable name="offset" select="ss:Cell[2]/ss:Data" />
                    <!-- Persistency offset -->
                    <xsl:variable name="persistencyOffset" select="ss:Cell[3]/ss:Data" />
                    <!-- Number of elements -->
                    <xsl:variable name="numElements" select="ss:Cell[4]/ss:Data" />
                    <!-- Element size -->
                    <xsl:variable name="elementSize" select="ss:Cell[5]/ss:Data" />
                    <!-- Short name -->
                    <xsl:variable name="shortName" select="ss:Cell[6]/ss:Data" />
                    <!-- Description -->
                    <xsl:variable name="description" select="ss:Cell[7]/ss:Data" />
                    <!-- Factory default setting -->
                    <xsl:variable name="default" select="ss:Cell[8]/ss:Data" />
                    <!-- User persistency read -->
                    <xsl:variable name="userPersistencyRead" select="ss:Cell[9]/ss:Data" />
                    <!-- User persistency write -->
                    <xsl:variable name="userPersistencyWrite" select="ss:Cell[10]/ss:Data" />
                    <!-- Size -->
                    <xsl:variable name="size" select="$numElements * $elementSize" />
                
                    <xsl:if test="$page != '' and $offset != '' and $numElements != '' and $elementSize != '' and $shortName != '' and $description != '' and $default != ''">
                    
                        <!-- Build #define name of offset -->
                        <xsl:variable name="addr.define.name">
                        
                            <xsl:call-template name="ctools.define.name">
                                <xsl:with-param name="moduleName">
                                    <xsl:value-of select="$global.moduleName" />
                                </xsl:with-param>
                                <xsl:with-param name="name">
                                    <xsl:text>PAGE </xsl:text>
                                    <xsl:value-of select="$page" />
                                    <xsl:text> </xsl:text>
                                    <xsl:text>OFFSET </xsl:text>
                                    <xsl:value-of select="$shortName" />
                                </xsl:with-param>
                            </xsl:call-template>
                            
                        </xsl:variable>
                        
                        <!-- Build persistency access function name -->
                        <xsl:variable name="ps.user.read.name">
                        
                            <xsl:choose>
                                <xsl:when test="not($userPersistencyRead)">
                                    <xsl:text>vscp_ps_user_</xsl:text>
                                    <xsl:call-template name="ctools.hungarianForm">
                                        <xsl:with-param name="string">
                                            <xsl:text>read </xsl:text>
                                            <xsl:value-of select="$shortName" />
                                        </xsl:with-param>
                                    </xsl:call-template>
                                </xsl:when>
                                <xsl:otherwise>
                                    <xsl:value-of select="$userPersistencyRead" />
                                </xsl:otherwise>
                            </xsl:choose>
                        
                        </xsl:variable>
                        
                        <!-- Build #define name of size -->
                        <xsl:variable name="size.define.name">
                        
                            <xsl:call-template name="ctools.define.name">
                                <xsl:with-param name="moduleName">
                                    <xsl:value-of select="'vscp_ps_user'" />
                                </xsl:with-param>
                                <xsl:with-param name="name">
                                    <xsl:text>SIZE </xsl:text>
                                    <xsl:value-of select="$shortName" />
                                </xsl:with-param>
                            </xsl:call-template>
                            
                        </xsl:variable>
                        
                        <xsl:choose>
                            <xsl:when test="fn:position() = 1">
                                <xsl:text>&TAB;&TAB;&TAB;if (</xsl:text>
                            </xsl:when>
                            <xsl:otherwise>
                                <xsl:text>&TAB;&TAB;&TAB;else if (</xsl:text>
                            </xsl:otherwise>
                        </xsl:choose>
                        
                        <xsl:choose>
                            <!-- Number of elements is 1 -->
                            <xsl:when test="$size = 1">
                                
                                <xsl:value-of select="$addr.define.name" />
                                <xsl:text> == addr)&LF;</xsl:text>

                            </xsl:when>
                            <!-- Number of elements are greater than 1 -->
                            <xsl:otherwise>
                            
                                <xsl:text>(</xsl:text>
                                <xsl:value-of select="$addr.define.name" />
                                <xsl:text> &lt;= addr) &amp;&amp;&LF;</xsl:text>
                                <xsl:text>&TAB;&TAB;&TAB;         ((</xsl:text>
                                <xsl:value-of select="$addr.define.name" />
                                <xsl:text> + </xsl:text>
                                <xsl:value-of select="$size" />
                                <xsl:text>) &gt; addr))&LF;</xsl:text>
                            
                            </xsl:otherwise>
                        </xsl:choose>
                        
                        <xsl:text>&TAB;&TAB;&TAB;{&LF;</xsl:text>
                        
                        <xsl:choose>
                            <!-- Number of elements is 1 -->
                            <xsl:when test="$size = 1">
                                
                                <xsl:text>&TAB;&TAB;&TAB;&TAB;value = </xsl:text>
                                <xsl:value-of select="$ps.user.read.name" />
                                <xsl:text>();&LF;</xsl:text>

                            </xsl:when>
                            <!-- Number of elements are greater than 1 -->
                            <xsl:otherwise>
                            
                                <xsl:text>&TAB;&TAB;&TAB;&TAB;value = </xsl:text>
                                <xsl:value-of select="$ps.user.read.name" />
                                <xsl:text>((</xsl:text>
                                <xsl:value-of select="$size.define.name" />
                                <xsl:text> - (addr - </xsl:text>
                                <xsl:value-of select="$addr.define.name" />
                                <xsl:text>)) - 1);&LF;</xsl:text>
                                                        
                            </xsl:otherwise>
                        </xsl:choose>
                        
                        <xsl:text>&TAB;&TAB;&TAB;}&LF;</xsl:text>
                        
                    </xsl:if>
                    
                </xsl:for-each>
        
                <xsl:text>&TAB;&TAB;}&LF;</xsl:text>
                <xsl:text>&TAB;}&LF;</xsl:text>
                                
                <xsl:for-each select="ss:Table/ss:Row[(fn:position() > 8) and (fn:number(ss:Cell[1]/ss:Data)) > 0]">
                    <!-- Sort after page and register offset. -->
                    <xsl:sort order="ascending" data-type="number" select="ss:Cell[1]/ss:Data" />
                    <xsl:sort order="ascending" data-type="number" select="ss:Cell[2]/ss:Data" />
                    
                    <!-- Page -->
                    <xsl:variable name="page" select="ss:Cell[1]/ss:Data" />
                    <!-- Register offset -->
                    <xsl:variable name="offset" select="ss:Cell[2]/ss:Data" />
                    <!-- Persistency offset -->
                    <xsl:variable name="persistencyOffset" select="ss:Cell[3]/ss:Data" />
                    <!-- Number of elements -->
                    <xsl:variable name="numElements" select="ss:Cell[4]/ss:Data" />
                    <!-- Element size -->
                    <xsl:variable name="elementSize" select="ss:Cell[5]/ss:Data" />
                    <!-- Short name -->
                    <xsl:variable name="shortName" select="ss:Cell[6]/ss:Data" />
                    <!-- Description -->
                    <xsl:variable name="description" select="ss:Cell[7]/ss:Data" />
                    <!-- Factory default setting -->
                    <xsl:variable name="default" select="ss:Cell[8]/ss:Data" />
                    <!-- User persistency read -->
                    <xsl:variable name="userPersistencyRead" select="ss:Cell[9]/ss:Data" />
                    <!-- User persistency write -->
                    <xsl:variable name="userPersistencyWrite" select="ss:Cell[10]/ss:Data" />
                    <!-- Size -->
                    <xsl:variable name="size" select="$numElements * $elementSize" />
                
                    <xsl:if test="$page != '' and $offset != '' and $numElements != '' and $elementSize != '' and $shortName != '' and $description != '' and $default != ''">
                    
                        <!-- Build #define name of offset -->
                        <xsl:variable name="addr.define.name">
                        
                            <xsl:call-template name="ctools.define.name">
                                <xsl:with-param name="moduleName">
                                    <xsl:value-of select="$global.moduleName" />
                                </xsl:with-param>
                                <xsl:with-param name="name">
                                    <xsl:text>PAGE </xsl:text>
                                    <xsl:value-of select="$page" />
                                    <xsl:text> </xsl:text>
                                    <xsl:text>OFFSET </xsl:text>
                                    <xsl:value-of select="$shortName" />
                                </xsl:with-param>
                            </xsl:call-template>
                            
                        </xsl:variable>
                        
                        <!-- Build persistency access function name -->
                        <xsl:variable name="ps.user.read.name">
                        
                            <xsl:choose>
                                <xsl:when test="not($userPersistencyRead)">
                                    <xsl:text>vscp_ps_user_</xsl:text>
                                    <xsl:call-template name="ctools.hungarianForm">
                                        <xsl:with-param name="string">
                                            <xsl:text>read </xsl:text>
                                            <xsl:value-of select="$shortName" />
                                        </xsl:with-param>
                                    </xsl:call-template>
                                </xsl:when>
                                <xsl:otherwise>
                                    <xsl:value-of select="$userPersistencyRead" />
                                </xsl:otherwise>
                            </xsl:choose>
                        
                        </xsl:variable>
                        
                        <xsl:choose>
                            <xsl:when test="fn:position() = 1">
                                <xsl:text>&TAB;else if (</xsl:text>
                                <xsl:value-of select="$page" />
                                <xsl:text> == page)&LF;</xsl:text>
                                <xsl:text>&TAB;{&LF;</xsl:text>
                            </xsl:when>
                            <xsl:when test="preceding-sibling::ss:Row[1]/ss:Cell[1]/ss:Data != $page">
                                <xsl:text>}&LF;</xsl:text>
                                <xsl:text>&TAB;else if (</xsl:text>
                                <xsl:value-of select="$page" />
                                <xsl:text> == page)&LF;</xsl:text>
                                <xsl:text>&TAB;{&LF;</xsl:text>
                            </xsl:when>
                        </xsl:choose>
                        
                        <xsl:choose>
                            <xsl:when test="(fn:position() = 1)  or (preceding-sibling::ss:Row[1]/ss:Cell[1]/ss:Data != $page)">
                                <xsl:text>&TAB;&TAB;if (</xsl:text>
                            </xsl:when>
                            <xsl:otherwise>
                                <xsl:text>&TAB;&TAB;else if (</xsl:text>
                            </xsl:otherwise>
                        </xsl:choose>
                        
                        <xsl:choose>
                            <!-- Number of elements is 1 -->
                            <xsl:when test="$size = 1">
                                
                                <xsl:value-of select="$addr.define.name" />
                                <xsl:text> == addr)&LF;</xsl:text>

                            </xsl:when>
                            <!-- Number of elements are greater than 1 -->
                            <xsl:otherwise>
                            
                                <xsl:text>(</xsl:text>
                                <xsl:value-of select="$addr.define.name" />
                                <xsl:text> &lt;= addr) &amp;&amp;&LF;</xsl:text>
                                <xsl:text>&TAB;&TAB;    ((</xsl:text>
                                <xsl:value-of select="$addr.define.name" />
                                <xsl:text> + </xsl:text>
                                <xsl:value-of select="$size" />
                                <xsl:text>) &gt; addr))&LF;</xsl:text>
                            
                            </xsl:otherwise>
                        </xsl:choose>
                        
                        <xsl:text>&TAB;&TAB;{&LF;</xsl:text>
                        
                        <xsl:choose>
                            <!-- Number of elements is 1 -->
                            <xsl:when test="$size = 1">
                                
                                <xsl:text>&TAB;&TAB;&TAB;value = </xsl:text>
                                <xsl:value-of select="$ps.user.read.name" />
                                <xsl:text>();&LF;</xsl:text>

                            </xsl:when>
                            <!-- Number of elements are greater than 1 -->
                            <xsl:otherwise>
                            
                                <xsl:text>&TAB;&TAB;&TAB;value = </xsl:text>
                                <xsl:value-of select="$ps.user.read.name" />
                                <xsl:text>(addr - </xsl:text>
                                <xsl:value-of select="$addr.define.name" />
                                <xsl:text>);&LF;</xsl:text>
                            
                            </xsl:otherwise>
                        </xsl:choose>
                        
                        <xsl:text>&TAB;&TAB;}&LF;</xsl:text>
                        
                    </xsl:if>
                    
                </xsl:for-each>                
                
                <xsl:text>&TAB;}&LF;</xsl:text>
                <xsl:text>&LF;</xsl:text>
                <xsl:text>&TAB;return value;&LF;</xsl:text>
            </xsl:with-param>
        </xsl:call-template>
        
    </xsl:template>
        
    <xsl:template match="ss:Worksheet[@ss:Name='app_register']" mode="c.globalFunctions.writeRegister">
        
        <xsl:call-template name="ctools.function">
            <xsl:with-param name="comment">
                <xsl:text>This function writes a value to an application specific register.&LF;</xsl:text>
                <xsl:text>&LF;</xsl:text>
                <xsl:text>@param[in]&TAB;page&TAB;Page&LF;</xsl:text>
                <xsl:text>@param[in]&TAB;addr&TAB;Register address</xsl:text>
                <xsl:text>@param[in]&TAB;value&TAB;Value to write</xsl:text>
                <xsl:text>@return Register value</xsl:text>
            </xsl:with-param>
            <xsl:with-param name="returnType">
                <xsl:text>extern uint8_t</xsl:text>
            </xsl:with-param>
            <xsl:with-param name="moduleName">
                <xsl:value-of select="$global.moduleName" />
            </xsl:with-param>
            <xsl:with-param name="name">
                <xsl:text>write register</xsl:text>
            </xsl:with-param>
            <xsl:with-param name="parameter">
                <xsl:text>uint16_t page, uint8_t addr, uint8_t value</xsl:text>
            </xsl:with-param>
            <xsl:with-param name="body">
                <xsl:text>&TAB;uint8_t&TAB;readBackValue = 0;&LF;</xsl:text>
                <xsl:text>&LF;</xsl:text>
                <xsl:text>&TAB;if (0 == page)&LF;</xsl:text>
                <xsl:text>&TAB;{&LF;</xsl:text>
                <xsl:text>&TAB;&TAB;if ((VSCP_REGISTER_APP_START_ADDR &lt;= addr) &amp;&amp;&LF;</xsl:text>
                <xsl:text>&TAB;&TAB;    (VSCP_REGISTER_APP_END_ADDR &gt;= addr))&LF;</xsl:text>
                <xsl:text>&TAB;&TAB;{&LF;</xsl:text>
        
                <xsl:for-each select="ss:Table/ss:Row[(fn:position() > 8) and (fn:number(ss:Cell[1]/ss:Data)) = 0]">
                    <!-- Sort after page and register offset. -->
                    <xsl:sort order="ascending" data-type="number" select="ss:Cell[1]/ss:Data" />
                    <xsl:sort order="ascending" data-type="number" select="ss:Cell[2]/ss:Data" />
                    
                <!-- Page -->
                <xsl:variable name="page" select="ss:Cell[1]/ss:Data" />
                <!-- Register offset -->
                <xsl:variable name="offset" select="ss:Cell[2]/ss:Data" />
                <!-- Persistency offset -->
                <xsl:variable name="persistencyOffset" select="ss:Cell[3]/ss:Data" />
                <!-- Number of elements -->
                <xsl:variable name="numElements" select="ss:Cell[4]/ss:Data" />
                <!-- Element size -->
                <xsl:variable name="elementSize" select="ss:Cell[5]/ss:Data" />
                <!-- Short name -->
                <xsl:variable name="shortName" select="ss:Cell[6]/ss:Data" />
                <!-- Description -->
                <xsl:variable name="description" select="ss:Cell[7]/ss:Data" />
                <!-- Factory default setting -->
                <xsl:variable name="default" select="ss:Cell[8]/ss:Data" />
                <!-- User persistency read -->
                <xsl:variable name="userPersistencyRead" select="ss:Cell[9]/ss:Data" />
                <!-- User persistency write -->
                <xsl:variable name="userPersistencyWrite" select="ss:Cell[10]/ss:Data" />
                    <!-- Size -->
                    <xsl:variable name="size" select="$numElements * $elementSize" />
                
                    <xsl:if test="$page != '' and $offset != '' and $numElements != '' and $elementSize != '' and $shortName != '' and $description != '' and $default != ''">
                    
                        <!-- Build #define name of offset -->
                        <xsl:variable name="addr.define.name">
                        
                            <xsl:call-template name="ctools.define.name">
                                <xsl:with-param name="moduleName">
                                    <xsl:value-of select="$global.moduleName" />
                                </xsl:with-param>
                                <xsl:with-param name="name">
                                    <xsl:text>PAGE </xsl:text>
                                    <xsl:value-of select="$page" />
                                    <xsl:text> </xsl:text>
                                    <xsl:text>OFFSET </xsl:text>
                                    <xsl:value-of select="$shortName" />
                                </xsl:with-param>
                            </xsl:call-template>
                            
                        </xsl:variable>

                        <!-- Build #define name of size -->
                        <xsl:variable name="size.define.name">
                        
                            <xsl:call-template name="ctools.define.name">
                                <xsl:with-param name="moduleName">
                                    <xsl:value-of select="'vscp_ps_user'" />
                                </xsl:with-param>
                                <xsl:with-param name="name">
                                    <xsl:text>SIZE </xsl:text>
                                    <xsl:value-of select="$shortName" />
                                </xsl:with-param>
                            </xsl:call-template>
                            
                        </xsl:variable>
                        
                        <!-- Build persistency access function name -->
                        <xsl:variable name="ps.user.read.name">
                        
                            <xsl:choose>
                                <xsl:when test="not($userPersistencyRead)">
                                    <xsl:text>vscp_ps_user_</xsl:text>
                                    <xsl:call-template name="ctools.hungarianForm">
                                        <xsl:with-param name="string">
                                            <xsl:text>read </xsl:text>
                                            <xsl:value-of select="$shortName" />
                                        </xsl:with-param>
                                    </xsl:call-template>
                                </xsl:when>
                                <xsl:otherwise>
                                    <xsl:value-of select="$userPersistencyRead" />
                                </xsl:otherwise>
                            </xsl:choose>
                        
                        </xsl:variable>
                        
                        <!-- Build persistency write access function name -->
                        <xsl:variable name="ps.user.write.name">
                        
                            <xsl:choose>
                                <xsl:when test="not($userPersistencyWrite)">
                                    <xsl:text>vscp_ps_user_</xsl:text>
                                    <xsl:call-template name="ctools.hungarianForm">
                                        <xsl:with-param name="string">
                                            <xsl:text>write </xsl:text>
                                            <xsl:value-of select="$shortName" />
                                        </xsl:with-param>
                                    </xsl:call-template>
                                </xsl:when>
                                <xsl:otherwise>
                                    <xsl:value-of select="$userPersistencyWrite" />
                                </xsl:otherwise>
                            </xsl:choose>

                        </xsl:variable>
                        
                        <xsl:choose>
                            <xsl:when test="fn:position() = 1">
                                <xsl:text>&TAB;&TAB;&TAB;if (</xsl:text>
                            </xsl:when>
                            <xsl:otherwise>
                                <xsl:text>&TAB;&TAB;&TAB;else if (</xsl:text>
                            </xsl:otherwise>
                        </xsl:choose>
                        
                        <xsl:choose>
                            <!-- Number of elements is 1 -->
                            <xsl:when test="$size = 1">
                                
                                <xsl:value-of select="$addr.define.name" />
                                <xsl:text> == addr)&LF;</xsl:text>

                            </xsl:when>
                            <!-- Number of elements are greater than 1 -->
                            <xsl:otherwise>
                            
                                <xsl:text>(</xsl:text>
                                <xsl:value-of select="$addr.define.name" />
                                <xsl:text> &lt;= addr) &amp;&amp;&LF;</xsl:text>
                                <xsl:text>&TAB;&TAB;&TAB;         ((</xsl:text>
                                <xsl:value-of select="$addr.define.name" />
                                <xsl:text> + </xsl:text>
                                <xsl:value-of select="$size" />
                                <xsl:text>) &gt; addr))&LF;</xsl:text>
                            
                            </xsl:otherwise>
                        </xsl:choose>
                        
                        <xsl:text>&TAB;&TAB;&TAB;{&LF;</xsl:text>
                        
                        <xsl:choose>
                            <!-- Number of elements is 1 -->
                            <xsl:when test="$size = 1">
                                
                                <xsl:text>&TAB;&TAB;&TAB;&TAB;</xsl:text>
                                <xsl:value-of select="$ps.user.write.name" />
                                <xsl:text>(value);&LF;</xsl:text>
                                <xsl:text>&TAB;&TAB;&TAB;&TAB;readBackValue = </xsl:text>
                                <xsl:value-of select="$ps.user.read.name" />
                                <xsl:text>();&LF;</xsl:text>

                            </xsl:when>
                            <!-- Number of elements are greater than 1 -->
                            <xsl:otherwise>
                            
                                <xsl:text>&TAB;&TAB;&TAB;&TAB;</xsl:text>
                                <xsl:value-of select="$ps.user.write.name" />
                                <xsl:text>((</xsl:text>
                                <xsl:value-of select="$size.define.name" />
                                <xsl:text> - (addr - </xsl:text>
                                <xsl:value-of select="$addr.define.name" />
                                <xsl:text>)) - 1), value);&LF;</xsl:text>
                                <xsl:text>&TAB;&TAB;&TAB;&TAB;readBackValue = </xsl:text>
                                <xsl:value-of select="$ps.user.read.name" />
                                <xsl:text>((</xsl:text>
                                <xsl:value-of select="$size.define.name" />
                                <xsl:text> - (addr - </xsl:text>
                                <xsl:value-of select="$addr.define.name" />
                                <xsl:text>)) - 1);&LF;</xsl:text>
                            
                            </xsl:otherwise>
                        </xsl:choose>
                        
                        <xsl:text>&TAB;&TAB;&TAB;}&LF;</xsl:text>
                        
                    </xsl:if>
                    
                </xsl:for-each>
        
                <xsl:text>&TAB;&TAB;}&LF;</xsl:text>
                <xsl:text>&TAB;}&LF;</xsl:text>
                
                <xsl:for-each select="ss:Table/ss:Row[(fn:position() > 8) and (fn:number(ss:Cell[1]/ss:Data)) > 0]">
                    <!-- Sort after page and register offset. -->
                    <xsl:sort order="ascending" data-type="number" select="ss:Cell[1]/ss:Data" />
                    <xsl:sort order="ascending" data-type="number" select="ss:Cell[2]/ss:Data" />
                    
                <!-- Page -->
                <xsl:variable name="page" select="ss:Cell[1]/ss:Data" />
                <!-- Register offset -->
                <xsl:variable name="offset" select="ss:Cell[2]/ss:Data" />
                <!-- Persistency offset -->
                <xsl:variable name="persistencyOffset" select="ss:Cell[3]/ss:Data" />
                <!-- Number of elements -->
                <xsl:variable name="numElements" select="ss:Cell[4]/ss:Data" />
                <!-- Element size -->
                <xsl:variable name="elementSize" select="ss:Cell[5]/ss:Data" />
                <!-- Short name -->
                <xsl:variable name="shortName" select="ss:Cell[6]/ss:Data" />
                <!-- Description -->
                <xsl:variable name="description" select="ss:Cell[7]/ss:Data" />
                <!-- Factory default setting -->
                <xsl:variable name="default" select="ss:Cell[8]/ss:Data" />
                <!-- User persistency read -->
                <xsl:variable name="userPersistencyRead" select="ss:Cell[9]/ss:Data" />
                <!-- User persistency write -->
                <xsl:variable name="userPersistencyWrite" select="ss:Cell[10]/ss:Data" />
                    <!-- Size -->
                    <xsl:variable name="size" select="$numElements * $elementSize" />
                
                    <xsl:if test="$page != '' and $offset != '' and $numElements != '' and $elementSize != '' and $shortName != '' and $description != '' and $default != ''">
                    
                        <!-- Build #define name of offset -->
                        <xsl:variable name="addr.define.name">
                        
                            <xsl:call-template name="ctools.define.name">
                                <xsl:with-param name="moduleName">
                                    <xsl:value-of select="$global.moduleName" />
                                </xsl:with-param>
                                <xsl:with-param name="name">
                                    <xsl:text>PAGE </xsl:text>
                                    <xsl:value-of select="$page" />
                                    <xsl:text> </xsl:text>
                                    <xsl:text>OFFSET </xsl:text>
                                    <xsl:value-of select="$shortName" />
                                </xsl:with-param>
                            </xsl:call-template>
                            
                        </xsl:variable>
                        
                        <!-- Build persistency access function name -->
                        <xsl:variable name="ps.user.read.name">
                        
                            <xsl:choose>
                                <xsl:when test="not($userPersistencyRead)">
                                    <xsl:text>vscp_ps_user_</xsl:text>
                                    <xsl:call-template name="ctools.hungarianForm">
                                        <xsl:with-param name="string">
                                            <xsl:text>read </xsl:text>
                                            <xsl:value-of select="$shortName" />
                                        </xsl:with-param>
                                    </xsl:call-template>
                                </xsl:when>
                                <xsl:otherwise>
                                    <xsl:value-of select="$userPersistencyRead" />
                                </xsl:otherwise>
                            </xsl:choose>
                        
                        </xsl:variable>
                        
                        <!-- Build persistency write access function name -->
                        <xsl:variable name="ps.user.write.name">
                        
                            <xsl:choose>
                                <xsl:when test="not($userPersistencyWrite)">
                                    <xsl:text>vscp_ps_user_</xsl:text>
                                    <xsl:call-template name="ctools.hungarianForm">
                                        <xsl:with-param name="string">
                                            <xsl:text>write </xsl:text>
                                            <xsl:value-of select="$shortName" />
                                        </xsl:with-param>
                                    </xsl:call-template>
                                </xsl:when>
                                <xsl:otherwise>
                                    <xsl:value-of select="$userPersistencyWrite" />
                                </xsl:otherwise>
                            </xsl:choose>
                            
                        </xsl:variable>
                        
                        <xsl:choose>
                            <xsl:when test="fn:position() = 1">
                                <xsl:text>&TAB;else if (</xsl:text>
                                <xsl:value-of select="$page" />
                                <xsl:text> == page)&LF;</xsl:text>
                                <xsl:text>&TAB;{&LF;</xsl:text>
                            </xsl:when>
                            <xsl:when test="preceding-sibling::ss:Row[1]/ss:Cell[1]/ss:Data != $page">
                                <xsl:text>&TAB;}&LF;</xsl:text>
                                <xsl:text>&TAB;else if (</xsl:text>
                                <xsl:value-of select="$page" />
                                <xsl:text> == page)&LF;</xsl:text>
                                <xsl:text>&TAB;{&LF;</xsl:text>
                            </xsl:when>
                        </xsl:choose>
                        
                        <xsl:choose>
                            <xsl:when test="(fn:position() = 1)  or (preceding-sibling::ss:Row[1]/ss:Cell[1]/ss:Data != $page)">
                                <xsl:text>&TAB;&TAB;if (</xsl:text>
                            </xsl:when>
                            <xsl:otherwise>
                                <xsl:text>&TAB;&TAB;else if (</xsl:text>
                            </xsl:otherwise>
                        </xsl:choose>
                        
                        <xsl:choose>
                            <!-- Number of elements is 1 -->
                            <xsl:when test="$size = 1">
                                
                                <xsl:value-of select="$addr.define.name" />
                                <xsl:text> == addr)&LF;</xsl:text>

                            </xsl:when>
                            <!-- Number of elements are greater than 1 -->
                            <xsl:otherwise>
                            
                                <xsl:text>(</xsl:text>
                                <xsl:value-of select="$addr.define.name" />
                                <xsl:text> &lt;= addr) &amp;&amp;&LF;</xsl:text>
                                <xsl:text>&TAB;&TAB;    ((</xsl:text>
                                <xsl:value-of select="$addr.define.name" />
                                <xsl:text> + </xsl:text>
                                <xsl:value-of select="$size" />
                                <xsl:text>) &gt; addr))&LF;</xsl:text>
                            
                            </xsl:otherwise>
                        </xsl:choose>
                        
                        <xsl:text>&TAB;&TAB;{&LF;</xsl:text>
                        
                        <xsl:choose>
                            <!-- Number of elements is 1 -->
                            <xsl:when test="$size = 1">
                                
                                <xsl:text>&TAB;&TAB;&TAB;</xsl:text>
                                <xsl:value-of select="$ps.user.write.name" />
                                <xsl:text>(value);&LF;</xsl:text>
                                <xsl:text>&TAB;&TAB;&TAB;readBackValue = </xsl:text>
                                <xsl:value-of select="$ps.user.read.name" />
                                <xsl:text>();&LF;</xsl:text>

                            </xsl:when>
                            <!-- Number of elements are greater than 1 -->
                            <xsl:otherwise>
                            
                                <xsl:text>&TAB;&TAB;&TAB;</xsl:text>
                                <xsl:value-of select="$ps.user.write.name" />
                                <xsl:text>(addr - </xsl:text>
                                <xsl:value-of select="$addr.define.name" />
                                <xsl:text>, value);&LF;</xsl:text>
                                <xsl:text>&TAB;&TAB;&TAB;readBackValue = </xsl:text>
                                <xsl:value-of select="$ps.user.read.name" />
                                <xsl:text>(addr - </xsl:text>
                                <xsl:value-of select="$addr.define.name" />
                                <xsl:text>);&LF;</xsl:text>
                            
                            </xsl:otherwise>
                        </xsl:choose>
                        
                        <xsl:text>&TAB;&TAB;}&LF;</xsl:text>
                        
                    </xsl:if>
                    
                </xsl:for-each>
                
                <xsl:text>&TAB;}&LF;</xsl:text>
                <xsl:text>&LF;</xsl:text>
                <xsl:text>&TAB;return readBackValue;&LF;</xsl:text>
            </xsl:with-param>
        </xsl:call-template>
        
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
