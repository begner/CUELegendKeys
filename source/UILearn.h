#pragma once

#include "UIBaseWindowGUIHost.h"
#include "UIMainWindow.h"
#include "CUELegendKeys.h"
#include "LearnController.h"
#include "ImageFilterMat.h"



class UILearn :
	public UIBaseWindowGUIHost
{
private:
	static UILearn * theInstance;
	UILearn();
	~UILearn();

	bool running = false;
	int zpmWidth = 350;
	int zpmHeight = 350;
	
	void updateLcUI();

	LearnController lc;
	
	UIImage* zoomedPreviewUI;

public:
	static UILearn * getInstance();
	static void release();
	void ShowIconMenu();

	void createWindow(); 
	void onHide();
	void onShow();
	static INT_PTR CALLBACK MessageHandler(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void handleClickEvents(int xPos, int yPos);

};

