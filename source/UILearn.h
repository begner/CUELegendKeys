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
	bool workerIsRunning = false;
	static UILearn * theInstance;
	UILearn();
	~UILearn();

	bool oldLearningState = false;
	bool oldInGameState = false;
	UIButton* createKeyButton(string label, int onClickId, int posX, int posY, int iconResource = -1);
	void setGroupVisibility(vector<UIBaseElement*> elementList, bool state);
	void setGroupVisibility(vector<UIButton*> elementList, bool state);
	void setGroupOffset(vector<UIBaseElement*> elementList, int offsetX, int offsetY);
	
	UIText* hoverText;

	vector<UIButton*> keyBar;

	vector<UIBaseElement*> borderPosControl;
	int borderPosControlOffsetX = 0;
	int borderPosControlOffsetY = 0;
	int borderPosControlWideOffsetX = 500;
	int borderPosControlWideOffsetY = -180;
	
	vector<UIBaseElement*> borderSizeControl;
	int borderSizeControlOffsetX = 0;
	int borderSizeControlOffsetY = 0;
	int borderSizeControlWideOffsetX = 500;
	int borderSizeControlWideOffsetY = -180;

	UIText* referenceInfoText;
	int referenceInfoOffsetX = 0;
	int referenceInfoOffsetY = 0;
	int referenceInfoWideOffsetX = 0;
	int referenceInfoWideOffsetY = 0;

	vector<UIBaseElement*> referenceControl;
	UIText* referenceNumberText;
	UIButton* ePrevReference;
	UIButton* eNextReference;
	int referenceControlOffsetX = 0;
	int referenceControlOffsetY = 0;
	int referenceControlWideOffsetX = 0;
	int referenceControlWideOffsetY = 0;

	UIButton* eLearnButton;
	int eLearnButtonOffsetX = 0;
	int eLearnButtonOffsetY = 0;
	int eLearnButtonWideOffsetX = 0;
	int eLearnButtonWideOffsetY = 0;

	UIButton* eSaveButton;
	int eSaveButtonOffsetX = 0;
	int eSaveButtonOffsetY = 0;
	int eSaveButtonWideOffsetX = 10;
	int eSaveButtonWideOffsetY = 40;

	UIText* eLearnButtonRequireText;
	int eLearnButtonRequireTextOffsetX = 0;
	int eLearnButtonRequireTextOffsetY = 0;
	int eLearnButtonRequireTextWideOffsetX = 10;
	int eLearnButtonRequireTextWideOffsetY = 40;
		
	UIText* skillNameText;
	
	UIImage* uiLearningProgressBlocker;
	
	UIImage* zoomedPreviewUI;
	int zpmX = 390;
	int zpmY = 168;
	int zpmWidth = 350;
	int zpmHeight = 350;
	int zpmWideX = 23;
	int zpmWideY = 323;
	int zpmWideWidth = 727;
	int zpmWideHeight = 210;

	UIImage* needlePreviewUI;
	int needleX = 289;
	int needleY = 166;
	int needleWidth = 64;
	int needleHeight = 64;
	int needleWideX = 22;
	int needleWideY = 274;
	int needleWideWidth = 424;
	int needleWideHeight = 20;
	
public:

	static CONST int IDE_CLICK_NEXT_REFERENCE = 1;
	static CONST int IDE_CLICK_PREV_REFERENCE = 2;

	static CONST int IDE_CLICK_LEARN = 3;
	static CONST int IDE_CLICK_SAVE = 4;

	static CONST int IDE_CLICK_BORDER_POS_T_INC = 10;
	static CONST int IDE_CLICK_BORDER_POS_T_DEC = 11;
	static CONST int IDE_CLICK_BORDER_POS_R_INC = 12;
	static CONST int IDE_CLICK_BORDER_POS_R_DEC = 13;
	static CONST int IDE_CLICK_BORDER_POS_B_INC = 14;
	static CONST int IDE_CLICK_BORDER_POS_B_DEC = 15;
	static CONST int IDE_CLICK_BORDER_POS_L_INC = 16;
	static CONST int IDE_CLICK_BORDER_POS_L_DEC = 17;
	static CONST int IDE_CLICK_BORDER_SIZE_INC = 18;
	static CONST int IDE_CLICK_BORDER_SIZE_DEC = 19;

	static CONST int IDE_CLICK_KEY_PASSIVE = 100;
	static CONST int IDE_CLICK_KEY_Q = 101;
	static CONST int IDE_CLICK_KEY_W = 102;
	static CONST int IDE_CLICK_KEY_E = 103;
	static CONST int IDE_CLICK_KEY_R = 104;
	static CONST int IDE_CLICK_KEY_D = 105;
	static CONST int IDE_CLICK_KEY_F = 106;
	static CONST int IDE_CLICK_KEY_B = 107;
	static CONST int IDE_CLICK_KEY_1 = 110;
	static CONST int IDE_CLICK_KEY_2 = 111;
	static CONST int IDE_CLICK_KEY_3 = 112;
	static CONST int IDE_CLICK_KEY_4 = 113;
	static CONST int IDE_CLICK_KEY_5 = 114;
	static CONST int IDE_CLICK_KEY_6 = 115;
	static CONST int IDE_CLICK_KEY_7 = 116;
	static CONST int IDE_CLICK_KEY_CHAMP = 117;
	static CONST int IDE_CLICK_KEY_HEAL = 118;
	static CONST int IDE_CLICK_KEY_MANA = 119;

	static UILearn * getInstance();
	static void release();
	
	static void threadWorker();
	void setThreadStarted();
	void processUI();
	void ShowIconMenu();
	void updateLcUI();

	LearnController* getLearnController();
	void createWindow(); 
	void onHide();
	void onShow();
	void onBeforeShow();
	static INT_PTR CALLBACK MessageHandler(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void handleClickEvents(int xPos, int yPos);

};

