#include "UISettingsDialog.h"

UISettingsDialog* UISettingsDialog::theInstance = NULL;

// extern CUELegendKeys cueLegendKeys;


UISettingsDialog* UISettingsDialog::getInstance()
{
	if (theInstance == NULL)
		theInstance = new UISettingsDialog();
	return theInstance;
}

void UISettingsDialog::release()
{
	if (theInstance != NULL)
		delete theInstance;
	theInstance = NULL;
}

UISettingsDialog::UISettingsDialog()
{
}


UISettingsDialog::~UISettingsDialog()
{
}

void UISettingsDialog::onHide() {
	UIMainWindow::getInstance()->notify("I'm still running...", "Please Note: I'm still running and eating up some performance!", 2);
}

void UISettingsDialog::onShow() {
	// cueLegendKeys.forceRefresh();

	int x = Settings::getInstance()->getValue("main", "window.x", -1);
	int y = Settings::getInstance()->getValue("main", "window.y", -1);

	if (x > -1 && y > -1) {
		SetWindowPos(hwnd, HWND_TOP, x, y, 0, 0, SWP_NOSIZE);
	}


}


void UISettingsDialog::ShowIconMenu() {

	int position = 0; // position of menuitem
	HMENU hMenu = CreatePopupMenu(); // menu
	UIMainWindow::getInstance()->createMenuItems(hMenu);

	// Show that thing!
	SetForegroundWindow(hwnd);

	// get Mouse Position
	POINT pt;
	GetCursorPos(&pt);

	// Show Menu
	TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_VERNEGANIMATION, pt.x, pt.y, 0, hwnd, NULL);

}

void UISettingsDialog::createWindow() {

	HWND hwnd;
	hwnd = CreateDialog(getHInstance(), MAKEINTRESOURCE(IDD_MAINWIN), UIMainWindow::getInstance()->getHandle(), UISettingsDialog::MessageHandler);
	setHandle(hwnd);

	SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)LoadIcon(getHInstance(), MAKEINTRESOURCE(IDI_APP_ICON)));

}


INT_PTR CALLBACK UISettingsDialog::MessageHandler(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	// UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{

	case WM_CONTEXTMENU:
		UISettingsDialog::getInstance()->ShowIconMenu();
		return (INT_PTR)TRUE;
		break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		BeginPaint(hDlg, &ps);
		EndPaint(hDlg, &ps);
		return (INT_PTR)TRUE;
	}
	break;
	case WM_WINDOWPOSCHANGED:
	{
		LPWINDOWPOS lpwp = (LPWINDOWPOS)lParam; // points to size and position data
		if (!(lpwp->flags & SWP_NOMOVE)) {
			Settings::getInstance()->setAutoCommit(false);
			Settings::getInstance()->setValue("main", "window.x", lpwp->x);
			Settings::getInstance()->setValue("main", "window.y", lpwp->y);
			Settings::getInstance()->commit();
		}

	}
	break;
	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			UISettingsDialog::getInstance()->Hide();
			return (INT_PTR)TRUE;
		}

		// passthru menu events to main window :)
		return UIMainWindow::MessageHandler(UIMainWindow::getInstance()->getHandle(), message, wParam, lParam);
	}
	break;
	}
	return (INT_PTR)FALSE;
}
