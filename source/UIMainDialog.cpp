#include "UIMainDialog.h"

UIMainDialog* UIMainDialog::theInstance = NULL;

// extern CUELegendKeys cueLegendKeys;


UIMainDialog* UIMainDialog::getInstance()
{
	if (theInstance == NULL)
		theInstance = new UIMainDialog();
	return theInstance;
}

void UIMainDialog::release()
{
	if (theInstance != NULL)
		delete theInstance;
	theInstance = NULL;
}

UIMainDialog::UIMainDialog()
{
}


UIMainDialog::~UIMainDialog()
{
}

void UIMainDialog::onHide() {
	
}

void UIMainDialog::onShow() {
	// cueLegendKeys.forceRefresh();

	int x = Settings::getInstance()->getValue("Main", "mainWindowX", -1);
	int y = Settings::getInstance()->getValue("Main", "mainWindowY", -1);

	if (x > -1 && y > -1) {
		SetWindowPos(hwnd, HWND_TOP, x, y, 0, 0, SWP_NOSIZE);
	}


}


void UIMainDialog::ShowIconMenu() {

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

void UIMainDialog::createWindow() {
	
	HWND hwnd;
	hwnd = CreateDialog(getHInstance(), MAKEINTRESOURCE(IDD_MAINWIN), UIMainWindow::getInstance()->getHandle(), UIMainDialog::MessageHandler);
	setHandle(hwnd);

	SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)LoadIcon(getHInstance(), MAKEINTRESOURCE(IDI_APP_ICON)));

}


INT_PTR CALLBACK UIMainDialog::MessageHandler(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	// UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
		
		case WM_CONTEXTMENU:
			UIMainDialog::getInstance()->ShowIconMenu();
			return (INT_PTR)TRUE;
			break;
		case WM_ERASEBKGND:
			{
				RECT rc;
				GetClientRect(hDlg, &rc);
				SetBkColor((HDC)wParam, 0x001f1a06);
				ExtTextOut((HDC)wParam, 0, 0, ETO_OPAQUE, &rc, 0, 0, 0);
				return (INT_PTR)TRUE;
			}
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
					Settings::getInstance()->setValue("main", "mainWindowX", lpwp->x);
					Settings::getInstance()->setValue("main", "mainWindowY", lpwp->y);
					Settings::getInstance()->commit();
				}

			}
			break;
		case WM_COMMAND:
			{
				if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
				{
					UIMainDialog::getInstance()->Hide();
					UIMainWindow::getInstance()->notify("I'm still running...", "Please Note: I'm still running and eating up some performance!\n\nShutdown CUELegendKeys with right click -> Quit", 2);
					return (INT_PTR)TRUE;
				}

				// passthru menu events to main window :)
				if (LOWORD(wParam) >= IDM_CONST_START_RANGE && LOWORD(wParam) <= IDM_CONST_END_RANGE) {
					NuLogger::getInstance()->log("passthru menu events to main window");
					return (INT_PTR)UIMainWindow::MessageHandler(UIMainWindow::getInstance()->getHandle(), message, wParam, lParam);
				}
				
			}
			break;
	}
	return (INT_PTR)FALSE;
}
