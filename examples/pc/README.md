# VSCP L1 PC simulation node

## Manual

The PC simulation node is a good start point to check out the features and possibilities of VSCP and
the VSCP framework.

If you start it the first time, nothing will happen except that the status lamp will blink (simulated by text :-) ).
That means the node requests the user to start the segment initialization, which means to discover its node
id. Press 'i' to start it and look whats happen.

If the node discovered its id, it will go to active state and starts sending its own heartbeat every second.
You are able to switch that off and on via 'h'.

Use the keys '1' to '5' to send some simulated events to the node.
If nothing happens, it looks like the decision matrix is empty, because the program didn't find the appropriate
decision matrix xml files. They have to be in the working directory, where the program is called from!

If the standard decision matrix is enabled and the extension is disabled, the "dmStd.xml" is used.
If the standard decision matrix is enabled and the extension is enabled, the "dmExt.xml" is used.

Check them to find out how to configure it by using your favorite editor.

If this sounds boring, because you like to connect more nodes together. No problem, just start a VSCP daemon
and connect to it. Start the program with '-h' as argument and you see the possible options, which are necessary
to connect to a VSCP daemon.

BTW if you start the PC simulation node again, you will notice that it starts up immediatelly without waiting for the user.
Thats because the nodes persistency is saved to the file "eeprom.asc". Check it out via editor, its human readable.
Or use the EEPROM dump mechanism 'e', which additional shows in color which VSCP related data is where.

If you want to restore the factory settings, remove the "eeprom.asc" and start the program.

Have fun!

## Issues, Ideas and bugs

If you have further ideas or you found some bugs, great! Create a [issue](https://github.com/BlueAndi/vscp-framework/issues) or if
you are able and willing to fix it by yourself, clone the repository and create a pull request.

## License
The whole source code is published under the [MIT license](http://choosealicense.com/licenses/mit/).
