#pragma once


#include <Windows.h>
#include <string>

class SystemTrayIcon
{
private:
	bool bInitialized = false;
	bool bLastResult = false;
	UINT IdCallback;
	NOTIFYICONDATA NotifyIconData;
	static const int MAX_TIP = 255;
	static const int MAX_TITLE = 1;
	static const int MAX_MESSAGE = 2;

public:
	SystemTrayIcon();
	~SystemTrayIcon();
	bool Create(const std::wstring &Tip, HWND hWndParent, const GUID &Guid, UINT idCallback, HICON hIcon, bool bSharedIcon, bool bHidden);
	void Balloon(const std::wstring &Title, const std::wstring &Message, HICON hBalloonIcon, UINT IconType, UINT Timeout, bool bSound, bool bLargeIcon, bool bRespectQuiteTime);
	void Update();
};

