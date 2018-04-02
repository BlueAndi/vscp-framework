# VSCP core time handling

The following diagram shows how the vscp-framework core handles the internal time, which is represented in unix time format.

![vscp-core-time-diagram](https://github.com/BlueAndi/vscp-framework/blob/master/vscp/doc/vscp_core_time.png)

If a segment controller is available, the time can be synchronized, as shown in the sequence diagram above.

If a real time clock (RTC) is available, just use them after power-up to initialize the internal time, until a heartbeat event from the segment controller is received. Later the RTC can be synchronized according to the segment masters time.
