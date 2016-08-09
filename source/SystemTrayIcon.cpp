#include "SystemTrayIcon.h"



SystemTrayIcon::SystemTrayIcon()
{
}


SystemTrayIcon::~SystemTrayIcon()
{
}

void SystemTrayIcon::Balloon(const std::wstring &Title, const std::wstring &Message, HICON hBalloonIcon, UINT IconType, UINT Timeout, bool bSound, bool bLargeIcon, bool bRespectQuiteTime)
{
	if (!bInitialized) return;

	if (Title.length() > MAX_TITLE)
	{
		wcsncpy_s(NotifyIconData.szInfoTitle, Title.c_str(), MAX_TITLE);
		NotifyIconData.szInfoTitle[MAX_TITLE] = wchar_t(0);
	}
	else
	{
		wcsncpy_s(NotifyIconData.szInfoTitle, Title.c_str(), Title.length() + 1);
	}

	if (Message.length() > MAX_MESSAGE)
	{
		wcsncpy_s(NotifyIconData.szInfo, Message.c_str(), MAX_MESSAGE);
		NotifyIconData.szInfo[MAX_MESSAGE] = wchar_t(0);
	}
	else
	{
		wcsncpy_s(NotifyIconData.szInfo, Message.c_str(), Message.length() + 1);
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

	Update();
}


bool SystemTrayIcon::Create(const std::wstring &Tip, HWND hWndParent, const GUID &Guid, UINT idCallback, HICON hIcon, bool bSharedIcon, bool bHidden)
{
	HICON hIcon = (HICON)LoadImage(*getHinstance(),
		MAKEINTRESOURCE(IDI_TRAY_ICON),
		IMAGE_ICON,
		GetSystemMetrics(SM_CXSMICON),
		GetSystemMetrics(SM_CYSMICON),
		LR_DEFAULTCOLOR);

	if (bInitialized) return false;

	ZeroMemory(&NotifyIconData, sizeof(NOTIFYICONDATA));
	NotifyIconData.cbSize = sizeof(NotifyIconData);
	NotifyIconData.hWnd = hWndParent;
	NotifyIconData.uID = 0;    // Use GUID instead. (NIF_GUID)
	NotifyIconData.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP | NIF_STATE | NIF_SHOWTIP;
	NotifyIconData.uCallbackMessage = IdCallback;
	NotifyIconData.hIcon = hIcon;

	if (Tip.length() > MAX_TIP)
	{
		wcsncpy_s(NotifyIconData.szTip, Tip.c_str(), MAX_TIP);
		NotifyIconData.szTip[MAX_TIP] = wchar_t(0);
	}
	else
	{
		wcsncpy_s(NotifyIconData.szTip, Tip.c_str(), Tip.length() + 1);
	}

	NotifyIconData.dwState = (bSharedIcon ? NIS_SHAREDICON : 0) | (bHidden ? NIS_HIDDEN : 0);
	NotifyIconData.dwStateMask = NIS_SHAREDICON | NIS_HIDDEN;
	/// NotifyIconData.guidItem = Guid;
	NotifyIconData.uCallbackMessage = IdCallback;
	bInitialized = Shell_NotifyIconW(NIM_ADD, &NotifyIconData);

	NotifyIconData.uVersion = NOTIFYICON_VERSION_4;
	bLastResult = Shell_NotifyIconW(NIM_SETVERSION, &NotifyIconData);

	

	return bInitialized;
}


void createMenu() {

}


void SystemTrayIcon::Update()
{
	bLastResult = Shell_NotifyIconW(NIM_MODIFY, &NotifyIconData);
}