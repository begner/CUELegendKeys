#include "UIMainWindow.h"

UIMainWindow* UIMainWindow::theInstance = NULL;

UIMainWindow* UIMainWindow::getInstance()
{
	if (theInstance == NULL)
		theInstance = new UIMainWindow();
	return theInstance;
}

void UIMainWindow::release()
{
	if (theInstance != NULL)
		delete theInstance;
	theInstance = NULL;
}

UIMainWindow::UIMainWindow() {
	
}


UIMainWindow::~UIMainWindow()
{

}

void UIMainWindow::notify(string message) {
	notify("CUE Legend Keys", message);
}

void UIMainWindow::notify(string title, string message, UINT IconType)
{

	HICON hBalloonIcon = (HICON)LoadImage(getHInstance(), MAKEINTRESOURCE(IDI_TRAY_ICON), IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	UINT Timeout = 10000U;
	bool bSound = false;
	bool bLargeIcon = true;
	bool bRespectQuiteTime = false;

	wstring wTitle = s2ws(title).c_str();
	wstring wMessage = s2ws(message).c_str();

	if (wTitle.length() > MAX_TITLE)
	{
		wcsncpy_s(NotifyIconData.szInfoTitle, wTitle.c_str(), MAX_TITLE);
		NotifyIconData.szInfoTitle[MAX_TITLE] = wchar_t(0);
	}
	else
	{
		wcsncpy_s(NotifyIconData.szInfoTitle, wTitle.c_str(), wTitle.length() + 1);
	}

	if (wMessage.length() > MAX_MESSAGE)
	{
		wcsncpy_s(NotifyIconData.szInfo, wMessage.c_str(), MAX_MESSAGE);
		NotifyIconData.szInfo[MAX_MESSAGE] = wchar_t(0);
	}
	else
	{
		wcsncpy_s(NotifyIconData.szInfo, wMessage.c_str(), wMessage.length() + 1);
	}

	DWORD dwType;
	switch (IconType)
	{
	case 0:
		dwType = 0;
		break;
	case 1:
		dwType = NIIF_INFO;
		break;
	case 2:
		dwType = NIIF_WARNING;
		break;
	case 3:
		dwType = NIIF_ERROR;
		break;
	case 4:
		dwType = NIIF_USER; // Use the "hBalloonIcon" parameter.
		break;
	}

	NotifyIconData.dwInfoFlags = dwType
		| (bSound ? 0 : NIIF_NOSOUND)
		| (bLargeIcon ? NIIF_LARGE_ICON : 0)
		| (bRespectQuiteTime ? NIIF_RESPECT_QUIET_TIME : 0);

	NotifyIconData.uTimeout = Timeout;
	NotifyIconData.hBalloonIcon = hBalloonIcon;
	NotifyIconData.uFlags = NIF_INFO | NIF_GUID;

	updateTrayIcon();
}


void UIMainWindow::createTrayIcon() {

	const std::wstring Tip = L"CUELegendKeys";
	NotificationCallbackId = SYTEMTRAY_NOTIFYICON;

	bool bSharedIcon = true;
	bool bHidden = false;

	GUID gidReference;
	HRESULT hCreateGuid = CoCreateGuid(&gidReference);


	ZeroMemory(&NotifyIconData, sizeof(NOTIFYICONDATA));

	NotifyIconData.cbSize = sizeof(NotifyIconData);

	NotifyIconData.uID = SYTEMTRAY_ICON_ID;    // Use GUID instead. (NIF_GUID)

	NotifyIconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_SHOWTIP; // | NIF_SHOWTIP; // NIF_STATE | 
	NotifyIconData.hIcon = (HICON)LoadImage(getHInstance(), MAKEINTRESOURCE(IDI_TRAY_ICON), IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);

	NotifyIconData.hWnd = hwnd;
	NotifyIconData.uCallbackMessage = SYTEMTRAY_NOTIFYICON;
	
	if (Tip.length() > MAX_TIP)
	{
		wcsncpy_s(NotifyIconData.szTip, Tip.c_str(), MAX_TIP);
		NotifyIconData.szTip[MAX_TIP] = wchar_t(0);
	}
	else
	{
		wcsncpy_s(NotifyIconData.szTip, Tip.c_str(), Tip.length() + 1);
	}

	// NotifyIconData.dwState = (bSharedIcon ? NIS_SHAREDICON : 0) | (bHidden ? NIS_HIDDEN : 0);
	// NotifyIconData.dwStateMask = NIS_SHAREDICON | NIS_HIDDEN;
	// NotifyIconData.guidItem = gidReference;
	Shell_NotifyIconW(NIM_ADD, &NotifyIconData);

	NotifyIconData.uVersion = NOTIFYICON_VERSION_4;
	Shell_NotifyIconW(NIM_SETVERSION, &NotifyIconData);
}

void UIMainWindow::updateTrayIcon()
{
	Shell_NotifyIconW(NIM_MODIFY, &NotifyIconData);
}

void UIMainWindow::deleteTrayIcon()
{
	Shell_NotifyIconW(NIM_DELETE, &NotifyIconData);
}


void UIMainWindow::createMenuItems(HMENU hMenu) {
	
	int position = 0; // position of menuitem

	// Open Close Main Dlg
	if (!IsWindowVisible(UIMainDialog::getInstance()->getHandle()))
		InsertMenu(hMenu, position++, MF_BYPOSITION | MF_STRING, IDM_SHOW_MAINDLG, (LPCWSTR)L"Open");
	else
		InsertMenu(hMenu, position++, MF_BYPOSITION | MF_STRING, IDM_HIDE_MAINDLG, (LPCWSTR)L"Hide to Tray");
	
	InsertMenu(hMenu, position++, MF_MENUBARBREAK, 0, NULL);
	
	InsertMenu(hMenu, position++, MF_BYPOSITION | MF_STRING, IDM_MODE_LEARN, (LPCWSTR)L"Learn...");

	// Force Modes
	if (!CUELegendKeys::getInstance()->getForceInGameClient())
		InsertMenu(hMenu, position++, MF_BYPOSITION | MF_STRING, IDM_MODE_FORCE_INGAME, (LPCWSTR)L"Force In-Game Mode");
	else
		InsertMenu(hMenu, position++, MF_BYPOSITION | MF_STRING, IDM_MODE_NORMAL, (LPCWSTR)L"Switch to Normal Mode");
		
	InsertMenu(hMenu, position++, MF_MENUBARBREAK, 0, NULL);
	
	

	InsertMenu(hMenu, position++, MF_BYPOSITION | MF_STRING, IDM_SHOW_ABOUT, (LPCWSTR)L"About");
	InsertMenu(hMenu, position++, MF_BYPOSITION | MF_STRING, IDM_QUIT, (LPCWSTR)L"Quit");
}

void UIMainWindow::ShowTrayMenu() {
	
	
	HMENU hMenu = CreatePopupMenu(); // menu

	createMenuItems(hMenu);

	// Show that thing!
	SetForegroundWindow(hwnd);

	// get Mouse Position
	POINT pt;
	GetCursorPos(&pt);

	// Show Menu
	TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_VERNEGANIMATION, pt.x, pt.y, 0, hwnd, NULL);
	
}

void UIMainWindow::onDestroy() {
	deleteTrayIcon();
}

void UIMainWindow::destroy() {
	DestroyWindow(hwnd);
	onDestroy();
}

void UIMainWindow::createWindow() {
	HWND hwnd;

	wchar_t *windowClass = TEXT("cuelegendkeystray");
	wchar_t *windowTitle = TEXT("CUELegendKeys Tray");

	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = UIMainWindow::MessageHandler;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = (HICON)::LoadImage(hInstance, MAKEINTRESOURCE(IDI_TRAY_ICON), IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = MAKEINTRESOURCE(IDC_TRAYMENU);
	wc.lpszClassName = windowClass;
	wc.hIconSm = (HICON)::LoadImage(hInstance, MAKEINTRESOURCE(IDI_TRAY_ICON), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);

	

	RegisterClassEx(&wc);

	hwnd = CreateWindow(windowClass, windowTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
	setHandle(hwnd);
}


INT_PTR CALLBACK UIMainWindow::MessageHandler(HWND _hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	auto xlParam = GET_X_LPARAM(wParam);

	switch (message)
	{
		case SYTEMTRAY_NOTIFYICON:
			
			
			if (xlParam == NIN_SELECT) {
				NuLogger::getInstance()->log("NIN_SELECT");
			}
			else if (xlParam == NIN_KEYSELECT) {
			}
			
			switch (LOWORD(lParam))
			{

				case WM_LBUTTONDBLCLK:
					UIMainDialog::getInstance()->Show();
					return (INT_PTR)TRUE;
					break;
				case NIN_KEYSELECT:
					// NuLogger::getInstance()->log("NIN_KEYSELECT");
					break;
				/*
				case NIN_BALLOONSHOW:
					NuLogger::getInstance()->log("NIN_BALLOONSHOW");
					break;
					*/
				case NIN_POPUPOPEN: // mouse over...
					// NuLogger::getInstance()->log("NIN_POPUPOPEN");
					// UIMainDialog::getInstance()->Show();
					break;
				case WM_CONTEXTMENU:
					UIMainWindow::getInstance()->ShowTrayMenu();
					return (INT_PTR)TRUE;
					break;
			}
			break;
		case WM_CREATE:
			return (INT_PTR)TRUE;
			break;
		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				BeginPaint(_hwnd, &ps);
				EndPaint(_hwnd, &ps);
				return (INT_PTR)TRUE;
			}
			break;
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case IDM_SHOW_MAINDLG:
					UIMainDialog::getInstance()->Show();
					return (INT_PTR)TRUE;
					break;
				case IDM_HIDE_MAINDLG:
					UIMainDialog::getInstance()->Hide();
					return (INT_PTR)TRUE;
					break;
				case IDM_SHOW_ABOUT:
					// DialogBox(ghInst, MAKEINTRESOURCE(IDD_ABOUTBOX), ghWnd, About);
					return (INT_PTR)TRUE;
					break;
				case IDM_SHOW_SETTINGS:
					// DialogBox(ghInst, MAKEINTRESOURCE(IDD_ABOUTBOX), ghWnd, About);
					return (INT_PTR)TRUE;
					break;
				case IDM_MODE_LEARN:
					CUELegendKeys::getInstance()->learnHotSpots();
					return (INT_PTR)TRUE;
					break;
				case IDM_MODE_NORMAL:
					CUELegendKeys::getInstance()->setForceInGameClient(false);
					return (INT_PTR)TRUE;
					break;
				case IDM_MODE_FORCE_INGAME:
					CUELegendKeys::getInstance()->setForceInGameClient(true);
					return (INT_PTR)TRUE;
					break;
				case IDM_QUIT:
					CUELegendKeys::getInstance()->quit();
					return (INT_PTR)TRUE;
					break;
			}
			break;
		case WM_DESTROY:
			CUELegendKeys::getInstance()->quit();
			return (INT_PTR)TRUE;
			break;
	}

	return DefWindowProc(_hwnd, message, wParam, lParam);
}

