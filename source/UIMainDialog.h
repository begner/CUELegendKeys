#pragma once

#include "UIBaseWindow.h"
#include "UIMainWindow.h"
#include "CUELegendKeys.h"

class UIMainDialog :
	public UIBaseWindow
{
private:
	static UIMainDialog * theInstance;
	UIMainDialog();
	~UIMainDialog();
	
public:
	static UIMainDialog * getInstance();
	static void release();
	void ShowIconMenu();

	void createWindow(); 
	void onHide();
	void onShow();
	static INT_PTR CALLBACK MessageHandler(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
};

