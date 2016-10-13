#pragma once

#include "Resource.h"
#include "ImageFilterMat.h"
#include "HSLSkill.h"

#include <math.h>

class LearnController
{
private:
	vector<pair<int, string>> allSkills;
	int currentSkillIDX = 0;
	int currentHSLfoundIDX = 0;
	HSLBase* currentHsl;
	Mat* getScreenshot();
	int borderPosL = 0;
	int borderPosT = 0;
	int borderPosR = 0;
	int borderPosB = 0;
	
	int borderSize = 1;

	void resetBorders();

public:

	static CONST int BORDER_POS_T = 0;
	static CONST int BORDER_POS_R = 1;
	static CONST int BORDER_POS_B = 2;
	static CONST int BORDER_POS_L = 3;

	LearnController();
	~LearnController();
	void next();
	void prev();
	void start();
	Mat* getUIPreview(int width, int height);

	void setBorderPosDelta(int position, int diff);
	void setBorderSizeDelta(int diff);

};

