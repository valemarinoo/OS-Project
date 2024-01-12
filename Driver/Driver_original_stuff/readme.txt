what does the test script do:

-open the module
-list all available commands the driver can perform
-try a bunch of combinations of them.

IOCTL domain
-SET and RESET performs writes to the 2 virtual registers ENABLE and BLINK_RATE of some STD values
-ENABLE and BLINK_RATE sets the current register to the command name, to allow for further R/W operations on that reg.

WRITE performs a (dummy) write to the selected registers (by IOCTL)
READ performs a dummy read, retrieving in each buffer byte the current register number.


**License: [CC BY-NC 4.0](https://creativecommons.org/licenses/by-nc/4.0/)**