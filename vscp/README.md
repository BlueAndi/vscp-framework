#VSCP framework detail

###Persistent memory layout

Attention, the persistent memory contains all data in LSB first!

| Id ( != address) | Size in byte | Enable flag | Description |
| ---------------: | -----------: | :---------- | :---------- |
|                1 |            1 | VSCP\_CONFIG\_BOOT\_LOADER\_SUPPORTED | Boot flag (jump to application or stay in bootloader) |
|                2 |            1 | - | VSCP nickname id |
|                3 |            1 | - | Segment controller CRC |
|                4 |            1 | - | Node control flags |
|                5 |            5 | - | User id |
|                6 |           16 | VSCP\_DEV\_DATA\_CONFIG\_ENABLE\_GUID\_STORAGE\_PS | GUID |
|                7 |            1 | VSCP\_DEV\_DATA\_CONFIG\_ENABLE\_NODE\_ZONE\_STORAGE\_PS | Node zone |
|                8 |            1 | VSCP\_DEV\_DATA\_CONFIG\_ENABLE\_NODE\_SUB\_ZONE\_STORAGE\_PS | Node sub zone |
|                9 |            4 | VSCP\_DEV\_DATA\_CONFIG\_ENABLE\_MANUFACTURER\_DEV\_ID\_STORAGE\_PS | Manufacturer device id |
|               10 |            4 | VSCP\_DEV\_DATA\_CONFIG\_ENABLE\_MANUFACTURER\_SUB\_DEV\_ID\_STORAGE\_PS | Manufacturer sub device id |
|               11 |           32 | VSCP\_DEV\_DATA\_CONFIG\_ENABLE\_MDF\_URL\_STORAGE\_PS | MDF URL |
|               12 |            4 | VSCP\_DEV\_DATA\_CONFIG\_ENABLE\_STD\_DEV\_FAMILY\_CODE\_STORAGE\_PS | Family code |
|               13 |            4 | VSCP\_DEV\_DATA\_CONFIG\_ENABLE\_STD\_DEV\_TYPE\_STORAGE\_PS | Device type |
|               14 | VSCP\_CONFIG\_DM\_ROWS * VSCP\_DM\_ROW\_SIZE | VSCP\_CONFIG\_ENABLE\_DM | Standard decision matrix |
|               15 | VSCP\_CONFIG\_DM\_ROWS * VSCP\_DM\_ROW\_SIZE | VSCP\_CONFIG\_ENABLE\_DM\_EXTENSION | Extended decision matrix |
|               16 | VSCP\_CONFIG\_DM\_NG\_RULE\_SET\_SIZE | VSCP\_CONFIG\_ENABLE\_DM\_NEXT\_GENERATION | Decision matrix next generation |
