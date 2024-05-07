#include "Arduino.h"
#include "RDKOTA.h"
#include <HTTPClient.h>
#include <Update.h>

RDKOTA::RDKOTA(char* OTA_HOST){
	_OTA_HOST = OTA_HOST;
}

void RDKOTA::begin(){
	currentLength = 0;
}

bool RDKOTA::checkForUpdate(char* actualVersion){
  bool retVal = false;
  HTTPClient http;
  String newVersion = actualVersion;
  String HostURL(_OTA_HOST);
  HostURL += ".version";
  Serial.println("Check version at:" + HostURL);
  http.begin(HostURL);
  int resp = http.GET();
  Serial.printf("checkForUpdate response:%d\r\n",resp);

  if(resp == 200){
	newVersion = http.getString();
	newVersion.trim();
	if (newVersion>actualVersion){
  		Serial.printf("Actual version:%s, New version: %s\r\n", actualVersion, newVersion);
  		retVal = true;
  	} else {
  		Serial.printf("Running the latest version: %s\r\n", newVersion);
  	}
  }else{
    Serial.println("No new version info available");
  }

  http.end();
  return retVal;
}

void RDKOTA::installUpdate(){
  HTTPClient http;
  int totalLength;
  String HostURL(_OTA_HOST);
  HostURL += ".ino.bin";
  Serial.println("Get version at:" + HostURL);
  http.begin(HostURL);
  int resp = http.GET();
  Serial.printf("getUpdate response:%d\r\n",resp);
  if(resp == 200){
    totalLength = http.getSize();
    int len = totalLength;
    // this is required to start firmware update process
    Update.begin(UPDATE_SIZE_UNKNOWN);
    Serial.printf("FW Size: %u\n",totalLength);
    uint8_t buff[128] = { 0 };
    WiFiClient * stream = http.getStreamPtr();
    Serial.println("Updating firmware...");
    while(http.connected() && (len > 0 || len == -1)) {
      size_t size = stream->available();
      if(size) {
        int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));
        RDKOTA::updateFirmware(buff, c, totalLength);
        if(len > 0) {
          len -= c;
        }
      }
      delay(1);
    }
  }else{
    Serial.println("Download failed");
  }
  http.end();
}

void RDKOTA::updateFirmware(uint8_t *data, size_t len, int totalLength){
  Update.write(data, len);
  currentLength += len;
  Serial.print('.');
  // if current length of written firmware is not equal to total firmware size, repeat
  if(currentLength != totalLength) return;
  Update.end(true);
  Serial.printf("\r\nUpdate Success, Total Size: %u\r\nRebooting...\r\n", currentLength); 
  ESP.restart();
}