# BT-RangeLab

# How to run Bluetooth
1. Compile code in Arduino IDE and upload it on ESP32
2. Bluetooth slave device on ESP32 should now be activated. 
3. Activate Bluetooth connection on the PC (host). Check MAC adress of the slave (ESP32)
3. If you are on Ubuntu, run command line: 'sudo rfcomm connect /dev/rfcomm0 XX:XX:XX:XX:XX:XX 1'. It creates virtual serial port and connect it to device.
XX:XX:XX:XX:XX:XX is your MAC adres. 
4. You should see that ESP32 is connected. Something like this:
'Connected /dev/rfcomm0 to 08:D1:F9:D1:1E:2E on channel 1
Press CTRL-C for hangup'
5. ESP32 transfers text from USB-UART to Bluetooth and from Bluetooth to UART and USB. To see data received from USB, you must check to which USB serial port ESP32 is connected (for example tty/USB0) and type: "screen <your port> 115200", to see your port on the terminal (for example: /dev/ttyUSB0).
6. To send data through Bluetooth you must type 'echo "Hello World!" | sudo tee <your virtual port> /dev/null', for example 'echo "Hello World!" | sudo tee /dev/rfcomm0 > /dev/null'.

