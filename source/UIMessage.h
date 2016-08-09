#pragma once

using namespace std;

#include <string>
#include <windows.h>
#include "UIMainWindow.h"

class UIMessage
{
public:
	static UIMessage * getInstance();
	static void release();
		
	int DisplayError(string title, string message, int uType = MB_ICONWARNING);
	void DisplayErrorAndQuit(string title, string message, int uType = MB_ICONWARNING);
private:
	UIMessage(void);
	~UIMessage(void);
	static UIMessage * theInstance;
};


