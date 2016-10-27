#pragma once

#include "Resource.h"
#include "ImageFilterMat.h"
#include "HSLSkill.h"
#include "HSLItem.h"
#include "HSLRelativeItem.h"
#include "HSLBar.h"
#include <math.h>
#include "Settings.h"
#include <thread>
#include <future>


class LearnController
{
private:
	static LearnController * theInstance;
	LearnController();
	~LearnController();

	bool jobInProgress = false;

	vector<HSLBase*> allSkills;
	int currentSkillIDX = 0;
	int currentHSLfoundIDX = 0;
	HSLBase* currentHsl;
	Mat* getScreenshot();
	int borderPosL = 0;
	int borderPosT = 0;
	int borderPosR = 0;
	int borderPosB = 0;
	

	bool dataChanged = false;
	bool thereAreSavedChanges = false;

	int borderSize = 1;
	Mat screenShotRaw;

	Mat wideMask;

	static bool threadLearn();

	int overScan = 5;
	void resetBorders();
	cv::Rect* currentLocation;
	Mat getPreviewMatForCurrentLocation();
public:
	static LearnController * getInstance();

	static CONST int BORDER_POS_T = 0;
	static CONST int BORDER_POS_R = 1;
	static CONST int BORDER_POS_B = 2;
	static CONST int BORDER_POS_L = 3;

	cv::Rect getSavedLocation(string saveId);
	int getBorderSize(string saveId);
	bool loadDataExists(string saveId);
	bool areThereSavedChanges();

	bool hasDataChanged();
	void setScreenShot(Mat* screenShot);
	void nextReference();
	void prevReference();
	void nextSkill();
	void prevSkill();
	void gotoSkill(int idx);
	void unloadCurrentHSL();
	void save();
	
	void load();
	void learn();
	bool isLearningInProgress();

	Mat getUIPreview(int width, int height, bool wideMode = false);
	Mat getUINeedle(int width, int height);
	
	void LearnController::moveCurrentLocation(int dir, int diff);
	
	HSLBase* getHSLbyIndex(int idx);
	HSLBase* getCurrentHSL();
	int getCurrentHSLfoundIDX();
	int getFoundSkillReferences();
	int getCurrentSkillIDX();
	void setBorderPosDelta(int position, int diff);
	void setBorderSizeDelta(int diff);

};

