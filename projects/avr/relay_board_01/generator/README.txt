--------------------------------------------------------------------------------
Author: Andreas Merkle, http://www.blue-andi.de

--------------------------------------------------------------------------------

The user specific persistent storage is generated, as well as the application register access.
This is done by a XML transformation, which base is the app_register.ods file.

Follow these steps to generate the files:
1. Update the app_register.ods with e. g. LibreOffice for your needs.
   Don't change the order of the columns, otherwise the generation is broken.
   
2. Save the app_register.ods and save it again in the Microsoft Excel 2003 XML format as app_register.xml.

3. Call the generate_app_register.bat (windows only).

4. The files are generated and automatically copied to ../vscp_user

5. Finished.
