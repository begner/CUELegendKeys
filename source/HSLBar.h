#pragma once
#include "HSLBase.h"
class HSLBar :
	public HSLBase
{
private:
	cv::Rect abilityQRect = {0, 0, 0, 0};
	cv::Rect abilityFRect = { 0, 0, 0, 0 };
	cv::Rect passiveRect = { 0, 0, 0, 0 };

	int hueRangeL = 0;
	int hueRangeH = 0;
public:
	HSLBar(string title, string saveId, int needleResource, int hueRangeLow, int hueRangeHigh);
	~HSLBar();

	void setPassive(cv::Rect input);
	void setAbilityQ(cv::Rect input);
	void setAbilityF(cv::Rect input);

	bool findLocations(Mat* haystack, Mat* needle, int offsetX, int offsetY);
	bool isWide();
	bool needsBorder();
};

