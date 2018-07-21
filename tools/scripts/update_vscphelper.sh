#!/bin/sh

if [ -z "$1" ]; then
    echo VSCP path parameter is missing.
    exit 1
fi

if [ -z "$1" ]; then
    echo Variant is missing. Please specify release or debug.
    exit 1
fi

VSCPHELPER_PATH=../../common/pc/vscphelper/

echo Updating the vscphelper ...

cp $1/src/common/crc.h $VSCPHELPER_PATH
cp $1/src/vscp/common/canal.h $VSCPHELPER_PATH
cp $1/src/vscp/common/canal_macro.h $VSCPHELPER_PATH
cp $1/src/vscp/common/canaldlldef.h $VSCPHELPER_PATH
cp $1/src/vscp/common/vscp.h $VSCPHELPER_PATH
cp $1/src/vscp/common/vscp_class.h $VSCPHELPER_PATH
cp $1/src/vscp/common/vscp_type.h $VSCPHELPER_PATH
cp $1/src/vscp/helperlib/vscphelperlib.h $VSCPHELPER_PATH

cp $1/src/vscp/helperlib/linux/libvscphelper.so $VSCPHELPER_PATH/lib/linux/$2/
cp $1/src/vscp/helperlib/linux/libvscphelper.a $VSCPHELPER_PATH/lib/linux/$2/

echo Finished.
