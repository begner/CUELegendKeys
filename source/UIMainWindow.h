#pragma once

#include "version.h"
#include "Constants.h"
#include <Windows.h>
#include <windowsx.h>
#include "UIBaseWindow.h"
#include "UIMainDialog.h"
#include "UILearn.h"

// #include <shellapi.h>
#include <string>

class UIMainWindow :
	public UIBaseWindow
{
private:
	static UIMainWindow * theInstance;
	UIMainWindow();
	~UIMainWindow();

	UINT NotificationCallbackId;
	NOTIFYICONDATA NotifyIconData;

	static const int MAX_TIP = 127;
	static const int MAX_TITLE = 48;
	static const int MAX_MESSAGE = 255;
	void createMenuItem(HMENU hMenu, int position, UINT flags, UINT_PTR dlgIdent, string title);
	void createMenuDivider(HMENU hMenu, int position);
	void deleteTrayIcon();
	void onDestroy();
public:
	static UIMainWindow * getInstance();
	static void release();

	void createMenuItems(HMENU hmenu);
	void createWindow();
	void createTrayIcon();
	void updateTrayIcon();
	void notify(string message);
	void notify(string title, string message, UINT IconType = 0);
	void ShowTrayMenu();
	void destroy();
	
	static INT_PTR CALLBACK MessageHandler(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
};

