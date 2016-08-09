#include "UIBase.h"

UIBase::UIBase()
{

}

UIBase::~UIBase()
{

}


void UIBase::setHInstance(HINSTANCE hinst) {
	hInstance = hinst;
}


HWND UIBase::getHandle() {
	return hwnd;
}

void UIBase::setHandle(HWND h) {
	hwnd = h;
}

HINSTANCE UIBase::getHInstance() {
	return hInstance;
}

void UIBase::createWindow()
{
	
}

void UIBase::onHide() {

}

void UIBase::Hide() {
	// Create a toast notification from the XML, then create a ToastNotifier object
	// to send the toast.


	// Shell_NotifyIcon(NIM_ADD, notificationData);
	// notificationData->tooltip
	ShowWindow(hwnd, SW_HIDE);
	onHide();
}


void UIBase::onShow() {

}

void UIBase::Show(bool doRestore) {
	if (doRestore) {
		ShowWindow(hwnd, SW_RESTORE);
	}
	else {
		ShowWindow(hwnd, SW_SHOW);
	}
	onShow();
	// UpdateWindow(hwnd);
}

