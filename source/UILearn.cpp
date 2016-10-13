#include "UILearn.h"

UILearn* UILearn::theInstance = NULL;

// extern CUELegendKeys cueLegendKeys;


UILearn* UILearn::getInstance()
{
	if (theInstance == NULL)
		theInstance = new UILearn();
	return theInstance;
}

void UILearn::release()
{
	if (theInstance != NULL)
		delete theInstance;
	theInstance = NULL;
}

UILearn::UILearn()
{
}

UILearn::~UILearn()
{
}



void UILearn::onShow() {
	// cueLegendKeys.forceRefresh();

	int x = Settings::getInstance()->getValue("Main", "LearnWindowX", -1);
	int y = Settings::getInstance()->getValue("Main", "LearnWindowY", -1);
	if (x > -1 && y > -1) {
		SetWindowPos(hwnd, HWND_TOP, x, y, 0, 0, SWP_NOSIZE);
	}
	running = true;
}

void UILearn::onHide() {
	running = false;
}

void UILearn::createWindow() {
	HWND hwnd;
	hwnd = CreateDialog(getHInstance(), MAKEINTRESOURCE(IDD_LEARN), UIMainWindow::getInstance()->getHandle(), UILearn::MessageHandler);
	setHandle(hwnd);
	SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)LoadIcon(getHInstance(), MAKEINTRESOURCE(IDI_APP_ICON)));
	
	setBackgroundResource(IDB_WINDOW_BACKGROUND_LEARNEDITOR);
	initHost();
	


	UIButton* element;
	element = new UIButton(IDB_UI_BUTTON_BIG_LEFT_NORMAL, IDB_UI_BUTTON_BIG_LEFT_OVER, IDB_UI_BUTTON_BIG_LEFT_MASK, 0.3);
	element->setPos(50, 50);
	element->setOnClickId(IDE_CLICK_PREV_REFERENCE);
	addElement(element);

	element = new UIButton(IDB_UI_BUTTON_BIG_RIGHT_NORMAL, IDB_UI_BUTTON_BIG_RIGHT_OVER, IDB_UI_BUTTON_BIG_RIGHT_MASK, 0.3);
	element->setPos(100, 50);
	element->setOnClickId(IDE_CLICK_NEXT_REFERENCE);
	addElement(element);



	element = new UIButton(IDB_UI_BUTTON_BIG_LEFT_NORMAL, IDB_UI_BUTTON_BIG_LEFT_OVER, IDB_UI_BUTTON_BIG_LEFT_MASK, 0.3);
	element->setPos(50, 150);
	element->setOnClickId(IDE_CLICK_BORDER_POS_T_INC);
	addElement(element);

	element = new UIButton(IDB_UI_BUTTON_BIG_RIGHT_NORMAL, IDB_UI_BUTTON_BIG_RIGHT_OVER, IDB_UI_BUTTON_BIG_RIGHT_MASK, 0.3);
	element->setPos(80, 150);
	element->setOnClickId(IDE_CLICK_BORDER_POS_T_DEC);
	addElement(element);

	element = new UIButton(IDB_UI_BUTTON_BIG_LEFT_NORMAL, IDB_UI_BUTTON_BIG_LEFT_OVER, IDB_UI_BUTTON_BIG_LEFT_MASK, 0.3);
	element->setPos(120, 150);
	element->setOnClickId(IDE_CLICK_BORDER_POS_R_INC);
	addElement(element);

	element = new UIButton(IDB_UI_BUTTON_BIG_RIGHT_NORMAL, IDB_UI_BUTTON_BIG_RIGHT_OVER, IDB_UI_BUTTON_BIG_RIGHT_MASK, 0.3);
	element->setPos(150, 150);
	element->setOnClickId(IDE_CLICK_BORDER_POS_R_DEC);
	addElement(element);


	element = new UIButton(IDB_UI_BUTTON_BIG_LEFT_NORMAL, IDB_UI_BUTTON_BIG_LEFT_OVER, IDB_UI_BUTTON_BIG_LEFT_MASK, 0.3);
	element->setPos(50, 200);
	element->setOnClickId(IDE_CLICK_BORDER_POS_B_INC);
	addElement(element);

	element = new UIButton(IDB_UI_BUTTON_BIG_RIGHT_NORMAL, IDB_UI_BUTTON_BIG_RIGHT_OVER, IDB_UI_BUTTON_BIG_RIGHT_MASK, 0.3);
	element->setPos(80, 200);
	element->setOnClickId(IDE_CLICK_BORDER_POS_B_DEC);
	addElement(element);

	element = new UIButton(IDB_UI_BUTTON_BIG_LEFT_NORMAL, IDB_UI_BUTTON_BIG_LEFT_OVER, IDB_UI_BUTTON_BIG_LEFT_MASK, 0.3);
	element->setPos(120, 200);
	element->setOnClickId(IDE_CLICK_BORDER_POS_L_INC);
	addElement(element);

	element = new UIButton(IDB_UI_BUTTON_BIG_RIGHT_NORMAL, IDB_UI_BUTTON_BIG_RIGHT_OVER, IDB_UI_BUTTON_BIG_RIGHT_MASK, 0.3);
	element->setPos(150, 200);
	element->setOnClickId(IDE_CLICK_BORDER_POS_L_DEC);
	addElement(element);


	element = new UIButton(IDB_UI_BUTTON_BIG_LEFT_NORMAL, IDB_UI_BUTTON_BIG_LEFT_OVER, IDB_UI_BUTTON_BIG_LEFT_MASK, 0.3);
	element->setPos(50, 300);
	element->setOnClickId(IDE_CLICK_BORDER_SIZE_INC);
	addElement(element);

	element = new UIButton(IDB_UI_BUTTON_BIG_RIGHT_NORMAL, IDB_UI_BUTTON_BIG_RIGHT_OVER, IDB_UI_BUTTON_BIG_RIGHT_MASK, 0.3);
	element->setPos(80, 300);
	element->setOnClickId(IDE_CLICK_BORDER_SIZE_DEC);
	addElement(element);





	zoomedPreviewUI = new UIImage();
	zoomedPreviewUI->setImage(Mat(zpmHeight, zpmWidth, CV_8UC4, Scalar(255, 0, 0, 255)));
	zoomedPreviewUI->setPos(220, 125);
	addElement(zoomedPreviewUI);

	

	UIButton* eStartButton = new UIButton(IDB_UI_BUTTON_TEXT_NORMAL, IDB_UI_BUTTON_TEXT_OVER, IDB_UI_BUTTON_TEXT_MASK);
	eStartButton->setPos(10, 10);
	eStartButton->setLabel("START LEARNING");
	eStartButton->setOnClickId(IDE_CLICK_START);
	addElement(eStartButton);



	
}

void UILearn::updateLcUI() {

	Mat* test = lc.getUIPreview(zpmHeight, zpmWidth);
 	zoomedPreviewUI->setImage(*test);
}

void UILearn::handleClickEvents(int xPos, int yPos) {
	int eventId = onClick(xPos, yPos);
	switch (eventId) {
		case IDE_CLICK_PREV_REFERENCE:
			lc.prev();
			updateLcUI();
			break;
		case IDE_CLICK_NEXT_REFERENCE:
			lc.next();
			updateLcUI();
			break;
		case IDE_CLICK_START:
			lc.start();
			updateLcUI();
			break;

		case IDE_CLICK_BORDER_POS_T_INC:
			lc.setBorderPosDelta(LearnController::BORDER_POS_T, 1);
			updateLcUI();
			break;
		case IDE_CLICK_BORDER_POS_T_DEC:
			lc.setBorderPosDelta(LearnController::BORDER_POS_T, -1);
			updateLcUI();
			break;
		case IDE_CLICK_BORDER_POS_R_INC:
			lc.setBorderPosDelta(LearnController::BORDER_POS_R, 1);
			updateLcUI();
			break;
		case IDE_CLICK_BORDER_POS_R_DEC:
			lc.setBorderPosDelta(LearnController::BORDER_POS_R, -1);
			updateLcUI();
			break;
		case IDE_CLICK_BORDER_POS_B_INC:
			lc.setBorderPosDelta(LearnController::BORDER_POS_B, 1);
			updateLcUI();
			break;
		case IDE_CLICK_BORDER_POS_B_DEC:
			lc.setBorderPosDelta(LearnController::BORDER_POS_B, -1);
			updateLcUI();
			break;
		case IDE_CLICK_BORDER_POS_L_INC:
			lc.setBorderPosDelta(LearnController::BORDER_POS_L, 1);
			updateLcUI();
			break;
		case IDE_CLICK_BORDER_POS_L_DEC:
			lc.setBorderPosDelta(LearnController::BORDER_POS_L, -1);
			updateLcUI();
			break;

		case IDE_CLICK_BORDER_SIZE_INC:
			lc.setBorderSizeDelta(1);
			updateLcUI();
			break;
		case IDE_CLICK_BORDER_SIZE_DEC:
			lc.setBorderSizeDelta(-1);
			updateLcUI();
			break;


	}	
}


INT_PTR CALLBACK UILearn::MessageHandler(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	// UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				BeginPaint(hDlg, &ps);
				UILearn::getInstance()->forceRefresh();
				EndPaint(hDlg, &ps);
				return (INT_PTR)TRUE;
			}
			break;
		case WM_LBUTTONUP:
			{
				int xPos = GET_X_LPARAM(lParam);
				int yPos = GET_Y_LPARAM(lParam);
				UILearn::getInstance()->handleClickEvents(xPos, yPos);
			}
			break;
		case WM_MOUSEMOVE:
			{
				int xPos = GET_X_LPARAM(lParam);
				int yPos = GET_Y_LPARAM(lParam);
				UILearn::getInstance()->onMouseMove(xPos, yPos);
			}
			break;
		case WM_WINDOWPOSCHANGED:
			{
				LPWINDOWPOS lpwp = (LPWINDOWPOS)lParam; // points to size and position data
				if (!(lpwp->flags & SWP_NOMOVE)) {
					Settings::getInstance()->setAutoCommit(false);
					Settings::getInstance()->setValue("Main", "LearnWindowX", lpwp->x);
					Settings::getInstance()->setValue("Main", "LearnWindowY", lpwp->y);
					Settings::getInstance()->commit();
				}

			}
			break;
		case WM_COMMAND:
		{
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				UILearn::getInstance()->Hide();
				return (INT_PTR)TRUE;
			}
		}
	}
	return (INT_PTR)FALSE;
}
