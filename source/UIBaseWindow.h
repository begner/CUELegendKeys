#pragma once

#include "Resource.h"
#include <Windows.h>
#include "NuLogger.h"

class UIBaseWindow
{
private:
	bool visibilityCache;

public:
	HWND hwnd;
	HINSTANCE hInstance;

	UIBaseWindow();
	virtual ~UIBaseWindow();

	virtual void createWindow();

	void setHInstance(HINSTANCE hinst);
	HINSTANCE getHInstance();
	
	void setHandle(HWND h);
	HWND getHandle();
	
	bool isVisible(bool dontUseCache = false);

	void Hide();
	virtual void onHide();
	
	void Show(bool doRestore = false);
	virtual void onBeforeShow();
	virtual void onShow();
	void setAlwaysOnTop();


};



