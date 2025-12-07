# BT-RangeLab

# How to run Bluetooth (Ubuntu)
1. **Compile code** in Arduino IDE and **upload it on ESP32**
2. **Bluetooth** slave device on **ESP32** should now be **activated**. 
3. Activate Bluetooth connection on the PC (host). Check MAC adress of the slave (ESP32)
## How to send data by Bluetooth from Master (PC) to Slave (and by USB serial port from Slave to Master)
3. Run command line: `sudo rfcomm connect /dev/rfcomm0 XX:XX:XX:XX:XX:XX 1`. It creates virtual serial port and connect it to device.
'XX:XX:XX:XX:XX:XX' is your MAC adres. 
4. You should see that ESP32 is connected. Something like this:
```
Connected /dev/rfcomm0 to 08:D1:F9:D1:1E:2E on channel 1
Press CTRL-C for hangup
```
5. ESP32 transfers text from USB-UART to Bluetooth and from Bluetooth to UART and USB. To see data received from USB, you must check to which USB serial port ESP32 is connected (for example `tty/USB0`). Type: `screen <your port> 115200` to see your port on the terminal (for example: `/dev/ttyUSB0`).
6. To send data through Bluetooth you must type `echo "Hello World!" | sudo tee <your virtual port> /dev/null`, for example `echo "Hello World!" | sudo tee /dev/rfcomm0 > /dev/nul`'.
## How to send data by Bluetooth from Slave (ESP32) to Master (and by USB serial port from Master to Slave)
7. Run command line `sudo rfcomm bind /dev/rfcomm0 XX:XX:XX:XX:XX:XX 1` and `sudo cat /dev/rfcomm0` . It allows you to see incoming data
8. To send data through Serial Port, you must check to which USB serial port ESP32 is connected (for example `tty/USB0`). Type: `echo "Hello Word!" | sudo tee /dev/ttyUSB0 > /dev/null` to send data. 
