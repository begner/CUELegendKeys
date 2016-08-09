#include "BubbleNotification.h"

BubbleNotification::BubbleNotification() {

}

BubbleNotification::BubbleNotification(string title, string content)
{
	sTitle = title;
	sContent = content;

	std::wstring sContentTemp = s2ws(sTitle); 
	std::wstring sTitleTemp = s2ws(sContent); 
	LPCTSTR pszTitle = sTitleTemp.c_str();
	LPCTSTR pszText = sContentTemp.c_str();
	

	ZeroMemory(&ti, sizeof(TOOLINFO));
	ti.cbSize = sizeof(TOOLINFO);
	#if (_WIN32_WINNT >= 0x0501)
		ti.cbSize -= sizeof(void*);
	#endif
	ti.uFlags = TTF_TRACK | TTF_ABSOLUTE;
	ti.lpszText = const_cast<LPTSTR>(pszText);

	DWORD dwRet = CreateTooltip(pszText, pszTitle, 150, 100, 200);
	if (NO_ERROR != dwRet)
	{
		
		return;
	}

	AutoHide();
}


BubbleNotification::~BubbleNotification()
{
}


void BubbleNotification::AutoHide() {
	CloseTooptip();
	return;
	
	HANDLE hTimer = NULL;
	LARGE_INTEGER liDueTime;

	liDueTime.QuadPart = -100000000LL;

	// Create an unnamed waitable timer.
	hTimer = CreateWaitableTimer(NULL, TRUE, NULL);
	if (NULL == hTimer)
	{
		// printf("CreateWaitableTimer failed (%d)\n", GetLastError());
		return;
	}
	
	if (!SetWaitableTimer(hTimer, &liDueTime, 0, NULL, NULL, 0))
	{
		// printf("SetWaitableTimer failed (%d)\n", GetLastError());
		return;
	}

	if (WaitForSingleObject(hTimer, INFINITE) != WAIT_OBJECT_0) {

	}
	else {
		CloseTooptip();
	}

}

void BubbleNotification::CloseTooptip() {
	BOOL bRet = AnimateWindow(hwnd, 200, AW_BLEND);
	// handle WM_PRINT!!! 
	if (!bRet)
	{
		NuLogger::getInstance()->log("AWBLEND Failed!");
	}
	

	// DestroyWindow(hwnd);
}

DWORD BubbleNotification::CreateTooltip(LPCTSTR pszText, LPCTSTR pszTitle, WORD x, WORD y, WORD nMaxWidth)
{


	DWORD dwRet = NO_ERROR;
	hwnd = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL, TTS_BALLOON | WS_POPUP, 0, 0, 0, 0, NULL, NULL, GetModuleHandle(NULL), NULL);
	// hwnd = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL, TTS_ALWAYSTIP | TTS_BALLOON | WS_POPUP, 0, 0, 0, 0, NULL, NULL, GetModuleHandle(0), 0);
	if (NULL != hwnd)
	{
		SendMessage(hwnd, TTM_ADDTOOL, 0, (LPARAM)&ti);
		SendMessage(hwnd, TTM_SETMAXTIPWIDTH, 0, nMaxWidth);
		SendMessage(hwnd, TTM_SETTITLE, TTI_INFO, (LPARAM)pszTitle);
		SendMessage(hwnd, TTM_TRACKACTIVATE, TRUE, (LPARAM)&ti);
		SendMessage(hwnd, TTM_TRACKPOSITION, 0, MAKELPARAM(x, y));
	}
	else
	{
		dwRet = ::GetLastError();
	}
	return dwRet;
}

