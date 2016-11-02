#include "FPPatchClient.h"


FPPatchClient::FPPatchClient(HWND uiHWND) : FPScreenMirror(uiHWND, CLK_Escape, CLK_PauseBreak)
{
	NuLogger::getInstance()->log("Setup FrameProcessing PatchClient");
	setOffset(RECT{ 10, 10, 10, 10 });
	setOffsetScale(1, 1);
	setUIDimensions(202, 328, 370, 20);
	setCutRect(330, 45, 330, 750);
	
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
