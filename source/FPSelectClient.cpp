#include "FPSelectClient.h"


FPSelectClient::FPSelectClient(HWND uiHWND) : FPScreenMirror(uiHWND) 
{
	NuLogger::getInstance()->log("Setup FrameProcessing SelectClient");
	setOffset(RECT {10, 80, 10, 10});
	setOffsetScale(1, 1.17f);
	setUIDimensions(209, 287, 363, 204);
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
