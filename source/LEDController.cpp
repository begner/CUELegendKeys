#include "LEDController.h"

LEDController* LEDController::theInstance = NULL;

LEDController* LEDController::getInstance()
{
	if (theInstance == NULL)
		theInstance = new LEDController();
	return theInstance;
}

void LEDController::release()
{
	if (theInstance != NULL)
		delete theInstance;
	theInstance = NULL;
}

LEDController::LEDController(void) {

}

LEDController::~LEDController(void) {
}

string LEDController::getErrorString(CorsairError error) {
	switch (error) {
	case CE_Success:
		return "CE_Success";
	case CE_ServerNotFound:
		return "CE_ServerNotFound";
	case CE_NoControl:
		return "CE_NoControl";
	case CE_ProtocolHandshakeMissing:
		return "CE_ProtocolHandshakeMissing";
	case CE_IncompatibleProtocol:
		return "CE_IncompatibleProtocol";
	case CE_InvalidArguments:
		return "CE_InvalidArguments";
	default:
		return "unknown error";
	}
}
/*
string LEDController::getKeyNameById(CorsairLedId id) {
	string keyname = ledIdToString(id);
	keyname = keyname.substr((keyname.find("_")+1));
	return keyname;
}
	*/
bool LEDController::checkCompatibleDevice() {
	CorsairPerformProtocolHandshake();
	if (const auto error = CorsairGetLastError()) {
		NuLogger::getInstance()->log("Handshake failed: " + getErrorString(error));
		//		NuLogger::getInstance()->log(std::endl);
		getchar();
		return false;
	}
	return true;
}

CorsairDeviceInfo * LEDController::getDeviceInfo() {
	CorsairDeviceInfo * info = CorsairGetDeviceInfo(0);
	return info;
}


CorsairLedPosition LEDController::getKeyPositionById(CorsairLedId ledId) {
	CorsairLedPosition ret;

	vector<CorsairLedPosition> pos = getKeyPositions();
	for (int idx = 0; idx < pos.size(); idx++) {
		if (pos[idx].ledId == ledId) {
			ret = pos[idx];
			break;
		}
	}
	return ret;
}

std::vector<CorsairLedPosition> LEDController::getKeyPositions() {
	if (cachedPositions.size() < 1) {
		
		auto positionVector = std::vector<CorsairLedPosition>();
		
		for (auto deviceIndex = 0; deviceIndex < CorsairGetDeviceCount(); deviceIndex++) {
			if (auto deviceInfo = CorsairGetDeviceInfo(deviceIndex)) {
				if (deviceInfo->type == CDT_Keyboard) {
					auto ledPositions = CorsairGetLedPositions();
					if (ledPositions) {
						for (auto i = 0; i < ledPositions->numberOfLed; i++) {
							auto ledId = ledPositions->pLedPosition[i].ledId;
							positionVector.push_back(ledPositions->pLedPosition[i]);
						}
					}
				}
			}
		}
		cachedPositions = positionVector;
	}
	return cachedPositions;
}

std::vector<CorsairLedColor> LEDController::getAvailableKeys()
{
	if (cachedColors.size() < 1) {
		auto colorsVector = std::vector<CorsairLedColor>();

		for (auto deviceIndex = 0; deviceIndex < CorsairGetDeviceCount(); deviceIndex++) {
			if (auto deviceInfo = CorsairGetDeviceInfo(deviceIndex)) {
				if (deviceInfo->type == CDT_Keyboard) {
					auto ledPositions = CorsairGetLedPositions();
					if (ledPositions) {
						for (auto i = 0; i < ledPositions->numberOfLed; i++) {
							auto ledId = ledPositions->pLedPosition[i].ledId;
							colorsVector.push_back(CorsairLedColor{ ledId, 0, 0, 0 });
						}
					}
				}
			}
		}
		cachedColors = colorsVector;
	}
	return cachedColors;
}

void LEDController::setKey(CorsairLedId ledId, int cR, int cG, int cB) {
	bool found = false;
	for (auto &ledColor : allKeys) {
		if (ledColor.ledId == ledId) {
			ledColor.r = cR;
			ledColor.g = cG;
			ledColor.b = cB;
			found = true;
			break;
		}
	}
	if (!found) {
		allKeys.push_back(CorsairLedColor{ ledId, cR, cG, cB });
	}
}

void LEDController::updateFrame() {
	CorsairSetLedsColors((int)allKeys.size(), allKeys.data());
}

void LEDController::initializeFrame(int r, int g, int b) {
	allKeys.empty();
	allKeys = getAvailableKeys();
	 
	for (auto &ledColor : allKeys) {
		ledColor.r = r;
		ledColor.g = g;
		ledColor.b = b;
	}
	
}

vector<CorsairLedPosition> LEDController::getAllKeysByRect(CorsairLedId topLeftKey, CorsairLedId bottomRightKey) {

	CorsairLedPosition posTopLeft = getKeyPositionById(topLeftKey);
	CorsairLedPosition posBottomRight = getKeyPositionById(bottomRightKey);

	// +/- 1 => be a litte fuzzy here...
	double top = posTopLeft.top - 1;
	if (top < 0) top = 0;
	double left = posTopLeft.left - 1;
	if (left < 0) left = 0;
	double bottom = posBottomRight.top + posBottomRight.height + 1; 
	double right = posBottomRight.left + posBottomRight.width + 1; 

	vector<CorsairLedPosition> kLayout = LEDController::getInstance()->getKeyPositions();

	vector<CorsairLedPosition>::iterator it = kLayout.begin();
	for (; it != kLayout.end(); ) {

		// filter keys
		if ((it->left >= left && it->left + it->width <= right) &&
			(it->top >= top && it->top + it->height <= bottom)) {
			// key is in range!

			// reposition it to 0, 0
			it->left = it->left - left;
			it->top = it->top - top;
			++it;
		}
		else {
			it = kLayout.erase(it);
		}
	}

	return kLayout;
}

RECT LEDController::getKeyBoardDimensions(vector<CorsairLedPosition> keys) {

	RECT myRect;
	myRect.top = 0;
	myRect.left = 0;

	int width = 0;
	int height = 0;
	// get dimension of keys

	vector<CorsairLedPosition>::iterator it;
	it = keys.begin();
	for (; it != keys.end(); ) {
		width = max(width, (int)ceil(it->left + it->width));
		height = max(height, (int)ceil(it->top + it->height));
		++it;
	}

	myRect.right = width;
	myRect.bottom = height;
	return myRect;
}
