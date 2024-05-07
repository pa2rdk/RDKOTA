#ifndef RDKOTA_h
#define RDKOTA_h

#include "Arduino.h"

class RDKOTA
{
	public:
		RDKOTA(char* OTA_HOST);
		void begin();
		bool checkForUpdate(char* actualVersion);
		void installUpdate();
		
	private:
		void updateFirmware(uint8_t *data, size_t len, int totalLength);
		int currentLength;
		String _OTA_HOST;
};

#endif