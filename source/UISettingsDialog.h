#pragma once

#include "UIBase.h"
#include "UIMainWindow.h"
#include "CUELegendKeys.h"

class UISettingsDialog :
	public UIBase
{
private:
	static UISettingsDialog * theInstance;
	UISettingsDialog();
	~UISettingsDialog();

public:
	static UISettingsDialog * getInstance();
	static void release();
	void ShowIconMenu();

	void createWindow();
	void onHide();
	void onShow();
	static INT_PTR CALLBACK MessageHandler(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
};

