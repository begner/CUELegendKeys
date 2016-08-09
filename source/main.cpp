#include <Windows.h>
#include <WinUser.h>
#include <WinBase.h>
#include <strsafe.h>
#include <array>

#include "Constants.h"
#include "Resource.h"

#include "UIMainWindow.h"
#include "UIMainDialog.h"

#include "CUELegendKeys.h"


void startWorker() {
	
	if (CUELegendKeys::getInstance()->AppStartupCheck()) {
		CUELegendKeys::getInstance()->AppInit();
		CUELegendKeys::getInstance()->AppStart();
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
	// createSteathWindow(&hInstance);

	UIMainWindow::getInstance()->setHInstance(hInstance);
	UIMainWindow::getInstance()->createWindow();
	UIMainWindow::getInstance()->createTrayIcon();

	UIMainDialog::getInstance()->setHInstance(hInstance);
	UIMainDialog::getInstance()->createWindow();
	
	CUELegendKeys::getInstance();
	


	// Boot
	// *****************************************
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)startWorker, NULL, NULL, NULL);
	
	
	// Message Loop
	MSG msg;
	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TRAYMENU));
	
	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	
	return (int)msg.wParam;
	
}
