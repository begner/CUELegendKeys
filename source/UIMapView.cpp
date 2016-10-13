#include "UIMapView.h"

UIMapView* UIMapView::theInstance = NULL;

// extern CUELegendKeys cueLegendKeys;


UIMapView* UIMapView::getInstance()
{
	if (theInstance == NULL)
		theInstance = new UIMapView();
	return theInstance;
}

void UIMapView::release()
{
	if (theInstance != NULL)
		delete theInstance;
	theInstance = NULL;
}

UIMapView::UIMapView()
{
	
}


UIMapView::~UIMapView()
{
}

void UIMapView::onHide() {
	
}

RECT UIMapView::getCurrentWindowSize() {
	RECT windowSize;
	GetClientRect(getHandle(), &windowSize);
	return windowSize;
}

void UIMapView::drawMap(Mat4b screenshotMat, int clientWidth, int clientHeight) {
	if (IsWindowVisible(getHandle())) {

		RECT targetRect = getCurrentWindowSize();
		int mapWindowWidth = targetRect.right - targetRect.left;
		int mapWindowHeight = targetRect.bottom - targetRect.top;

		if (mapWindowWidth < 1 || mapWindowHeight < 1) {
			return;
		}

		cv::Rect mapRect(clientWidth - mapRight - mapWidth - mapBorder * 2, clientHeight - mapBottom - mapHeight - mapBorder * 2,
			mapWidth - mapBorder - mapRight, mapHeight - mapBorder - mapBottom);

		if (screenshotMat.cols < (mapWidth + mapBorder * 2 + mapRight) || screenshotMat.rows < (mapHeight + mapBorder * 2 + mapBottom)) {
			return;
		}

		Mat4b map;
		map = Mat4b(screenshotMat, mapRect);

		resize(map, map, cv::Size(mapWindowWidth, mapWindowHeight), 0, 0, INTER_NEAREST);
		ImageFilterMat::DrawToHDC(mapDrawHDC, map, 0, 0, map.cols, map.rows);

		BitBlt(mapWindowHDC, 0, 0, mapWindowWidth, mapWindowHeight, mapDrawHDC, 0, 0, SRCCOPY);

	}
}

void UIMapView::onResize() {
	// Map Window HDC
	RECT targetRect = getCurrentWindowSize();
	int mapWindowWidth = targetRect.right - targetRect.left;
	int mapWindowHeight = targetRect.bottom - targetRect.top;

	mapDrawHBitmap = CreateCompatibleBitmap(mapWindowHDC, mapWindowWidth, mapWindowHeight);
	SelectObject(mapDrawHDC, mapDrawHBitmap);
}

void UIMapView::onShow() {
	// cueLegendKeys.forceRefresh();

	int x = Settings::getInstance()->getValue("Main", "MapWindowX", -1);
	int y = Settings::getInstance()->getValue("Main", "MapWindowY", -1);
	int w = Settings::getInstance()->getValue("Main", "MapWindowW", -1);
	int h = Settings::getInstance()->getValue("Main", "MapWindowH", -1);

	if (x > -1 && y > -1 && w > -1 && h > -1) {
		SetWindowPos(hwnd, HWND_TOP, x, y, w, h, NULL);
	}


}

void UIMapView::createWindow() {
	
	HWND hwnd;
	hwnd = CreateDialog(getHInstance(), MAKEINTRESOURCE(IDD_MAP), UIMainWindow::getInstance()->getHandle(), UIMapView::MessageHandler);
	setHandle(hwnd);
	SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)LoadIcon(getHInstance(), MAKEINTRESOURCE(IDI_APP_ICON)));

	mapWindowHDC = GetDC(getHandle());
	mapDrawHDC = CreateCompatibleDC(mapWindowHDC);
}


INT_PTR CALLBACK UIMapView::MessageHandler(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	// UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
		
		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				BeginPaint(hDlg, &ps);
				EndPaint(hDlg, &ps);
				return (INT_PTR)TRUE;
			}
			break;
		case WM_SIZING:
			{
				Rect* windowRect = (Rect*)lParam;
				Settings::getInstance()->setValue("Main", "MapWindowW", windowRect->width - windowRect->x);
				Settings::getInstance()->setValue("Main", "MapWindowH", windowRect->height - windowRect->y);
				Settings::getInstance()->commit();
			}
			break;
		case WM_SIZE: 
			{
				UIMapView::getInstance()->onResize();
			}
			break;
		case WM_WINDOWPOSCHANGED:
			{
				LPWINDOWPOS lpwp = (LPWINDOWPOS)lParam; // points to size and position data
				if (!(lpwp->flags & SWP_NOMOVE)) {
					Settings::getInstance()->setAutoCommit(false);
					Settings::getInstance()->setValue("Main", "MapWindowX", lpwp->x);
					Settings::getInstance()->setValue("Main", "MapWindowY", lpwp->y);
					Settings::getInstance()->commit();
				}

			}
			break;
		case WM_COMMAND:
		{
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				UIMapView::getInstance()->Hide();
				return (INT_PTR)TRUE;
			}
		}
	}
	return (INT_PTR)FALSE;
}
