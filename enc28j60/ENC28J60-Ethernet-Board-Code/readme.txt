======================================
Programming Methods
======================================
* SWD JTAG


======================================
Demo Function
======================================
* ENC28J60 Ethernet Board Example


======================================
Hardware Connection
======================================
* Connect an ENC28J60 Ethernet Board to the SPI interface of STM32 MCU.
* Connect the ENC28J60 Ethernet Board and a PC by an ethernet cable.


======================================
Software Settings
======================================
* Set the IP of your PC to 192.168.0.x (0 <= x <= 255)
* Confirm the Serial(COM) number by Resource Manager then set the COM monitor software as:
+-------------------+
|Baud rate | 115200 |
+-------------------+
|Data bit  |   8    |
+-------------------+
|Stop bit  |   1    |
+-------------------+
|Parity    |  None  |
+-------------------+

======================================
Expected result
======================================
Enter the address 192.168.0.100/888 in your browser and refer to the page information.