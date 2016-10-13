#pragma once

#include "UIBaseWindow.h"
#include "UIMainWindow.h"
#include "CUELegendKeys.h"
#include "ImageFilterMat.h"

class UIMapView :
	public UIBaseWindow
{
private:
	static UIMapView * theInstance;
	UIMapView();
	~UIMapView();
	

	int mapHeight = 255;
	int mapWidth = 254;
	int mapBorder = 3;
	int mapRight = 4;
	int mapBottom = 5;


	HDC mapWindowHDC;
	HDC mapDrawHDC;
	HBITMAP mapDrawHBitmap;

public:
	static UIMapView * getInstance();
	static void release();
	void ShowIconMenu();

	void createWindow(); 
	void onHide();
	void onShow();
	static INT_PTR CALLBACK MessageHandler(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	
	void drawMap(Mat4b screenshotMat, int clientWidth, int clientHeight);
	void onResize();
	RECT getCurrentWindowSize();
};

