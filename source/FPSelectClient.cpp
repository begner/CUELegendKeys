#include "FPSelectClient.h"


FPSelectClient::FPSelectClient(HWND uiHWND) : FPScreenMirror(uiHWND) 
{
	setNewClient(false, true);
}

FPSelectClient::~FPSelectClient()
{

}


void FPSelectClient::setCaptureWindow(HWND currentProcess) {
	FPScreenMirror::setCaptureWindow(currentProcess);
}


int FPSelectClient::getWindowBackgroundResource() {
	return IDB_WINDOW_BACKGROUND_SELECT;
}

bool FPSelectClient::process() {
	FPScreenMirror::setFpsLimit(Settings::getInstance()->getValue("SelectClientMode", "FPSLimit", (int)15));
	return FPScreenMirror::process();
}

void FPSelectClient::setNewClient(bool state, bool forceSet) {
	if (isNewClient != state || forceSet) {
		isNewClient = state;
		if (isNewClient) {
			NuLogger::getInstance()->log("New SelectClient");
			setOffset(RECT{ 40, 140, 320, 0});
			setOffsetScale(1, 1.25f);
			setUIDimensions(209, 287, 363, 204);
		}
		else {
			NuLogger::getInstance()->log("old SelectClient");
			setOffset(RECT{ 10, 80, 10, 10 });
			setOffsetScale(1, 1.17f);
			setUIDimensions(209, 287, 363, 204);
		}
	}
}