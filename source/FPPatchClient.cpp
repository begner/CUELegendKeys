#include "FPPatchClient.h"


FPPatchClient::FPPatchClient(HWND uiHWND) : FPScreenMirror(uiHWND)
{
	setOffset(RECT{ 0, 10, 10, 5 });
	setOffsetScale(1, 1);
	setUIDimensions(209, 287, 98*4, 7*4*5);
	setCutRect(300, 0, 240, 730);
}

FPPatchClient::~FPPatchClient()
{

}


void FPPatchClient::setCaptureWindow(HWND currentProcess) {
	FPScreenMirror::setCaptureWindow(currentProcess);
}


int FPPatchClient::getWindowBackgroundResource() {
	return IDB_WINDOW_BACKGROUND_PATCH;
}

bool FPPatchClient::process() {
	FPScreenMirror::setFpsLimit(Settings::getInstance()->getValue("PatchClientMode", "FPSLimit", (int)15));
	return FPScreenMirror::process();
}
