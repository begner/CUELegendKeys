#include "UIBaseWindow.h"

UIBaseWindow::UIBaseWindow()
{

}

UIBaseWindow::~UIBaseWindow()
{

}


void UIBaseWindow::setHInstance(HINSTANCE hinst) {
	hInstance = hinst;
}


HWND UIBaseWindow::getHandle() {
	return hwnd;
}

void UIBaseWindow::setHandle(HWND h) {
	hwnd = h;
}

HINSTANCE UIBaseWindow::getHInstance() {
	return hInstance;
}

bool UIBaseWindow::isVisible() {
	if (IsWindowVisible(getHandle())) {
		return true;
	}
	else {
		return false;
	}
}

void UIBaseWindow::createWindow()
{
	
}

void UIBaseWindow::onHide() {

}

void UIBaseWindow::Hide() {
	// Create a toast notification from the XML, then create a ToastNotifier object
	// to send the toast.


	// Shell_NotifyIcon(NIM_ADD, notificationData);
	// notificationData->tooltip
	ShowWindow(hwnd, SW_HIDE);
	onHide();
}


void UIBaseWindow::onShow() {

}

void UIBaseWindow::Show(bool doRestore) {
	if (doRestore) {
		ShowWindow(hwnd, SW_RESTORE);
	}
	else {
		ShowWindow(hwnd, SW_SHOW);
	}
	onShow();
	// UpdateWindow(hwnd);
}


void UIBaseWindow::setAlwaysOnTop() {

	SetWindowPos(getHandle(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

}
