#pragma once

// using namespace std;

#include <iostream>
#include <windows.h>
#include <iostream>
#include <string>

#include <thread>
#include <future>
#include <vector>

#include "CUESDK.h"
#include "NuLogger.h"



class LEDController
{
public:
	static LEDController * getInstance();
	static void release();
	bool checkCompatibleDevice();

	vector<CorsairLedPosition> getKeyPositions();
	vector<CorsairLedColor> getAvailableKeys();
	CorsairLedPosition getKeyPositionById(CorsairLedId ledId);
	CorsairDeviceInfo * getDeviceInfo();
	void initializeFrame(int r = 5, int g = 5, int b = 5);
	void setKey(CorsairLedId ledId, int cR, int cG, int cB);
	void updateFrame();
	vector<CorsairLedPosition> getAllKeysByRect(CorsairLedId topLeftKey, CorsairLedId bottomRightKey);
	RECT getKeyBoardDimensions(vector<CorsairLedPosition> keys);


	// string getKeyNameById(CorsairLedId id);
	// string ledIdToString(CorsairLedId id);
private:

	std::vector<CorsairLedPosition> cachedPositions;
	std::vector<CorsairLedColor> cachedColors;

	std::vector<CorsairLedColor> allKeys;
	
	LEDController(void);
	~LEDController(void);
	static LEDController * theInstance;

	string getErrorString(CorsairError error);

};


