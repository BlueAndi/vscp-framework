#!/bin/bash

# ********************************************************************************
# Author: Andreas Merkle, http://www.blue-andi.de
# ********************************************************************************

echo "Generate VSCP events ..."
java -jar ../xslt/saxon/saxon9he.jar -s:../vscp-protocol/vscp-protocol.xml -xsl:vscp-protocol2vscp-events.xsl
if [ $? -ne 0 ]; then
    echo "Error: XSLT transformation failed."
    exit 1
fi

# Convert unix to windows line endings
echo "Convert line endings ..."
sed -i 's/$/\r/' vscp_evt_*

# Convert tabs to spaces
echo "Convert tabs to spaces ..."
for f in vscp_evt_*.h vscp_evt_*.c; do
    [ -f "$f" ] || continue
    expand -t 4 "$f" > temp.tmp
    mv temp.tmp "$f"
done

# Copy generated files to project
echo "Copy generated files ..."
cp -f vscp_evt_* ../../src/events

# Remove generated files
rm -f vscp_evt_*
