# RDKOTA
 RDKOTA
 
 
This Arduino OTA library is meant to make an 'Over The Air' update for the ESP32 very simple.
With just a few lines of code you add OTA to your project.

This library needs 2 files somewhere on the internet.
In this example OTAExample.version, holding just the version number of the latest version as defined in 'OTAVERSION'
This file only holds a string with the actual version, in this example v2.8
And OTAExample.ino.bin, the compiled ino file.
You can create a compiled ino file from the main menu of the Arduino IDE. Select Sketch and then select Export Compiled Binary.
The bin file can be found in your project folder in the Build folder. 
I put both files in "https://www.rjdekok.nl/Updates/", but you can put them anywhere as defined in 'OTAHOST'.
If you run this example, you will see in the serial monitor the check for update. 
As the version is equal to the version on the internet, nothing will happen.
By changing the version to something lower (fi. v2.7), you will see in the serial monitor the update process en the reboot of the ESP32 with the new version.

To update the ESP, a connection with the WiFi is necessary, therefor it needs your SSID and WiFi password.
This library uses HTTPClient.h and Update.h which are part of the Espressif ESP32 board manager