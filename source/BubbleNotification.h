#pragma once

using namespace std;

#include <Windows.h>
#include "convert.h"
#include <string>
#include <Uxtheme.h>
#include "NuLogger.h"

class BubbleNotification
{
private:
	
	string sTitle;
	string sContent;
	HWND hwnd;
	TOOLINFO ti;
	DWORD CreateTooltip(LPCTSTR pszText, LPCTSTR pszTitle, WORD x, WORD y, WORD nMaxWidth);
	void CloseTooptip();
	void AutoHide();
public:
	BubbleNotification(string title, string content);
	BubbleNotification();
	~BubbleNotification();
	
};

