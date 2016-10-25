#pragma once
#include "HSLBase.h"


class HSLRelativeItem :
	public HSLBase
{
private:
	cv::Rect item1 = { 0, 0, 0, 0 };
	cv::Rect item2 = { 0, 0, 0, 0 };
	int searchItem;
public:
	HSLRelativeItem(string title, string saveId, int needleResource, int searchItem);
	~HSLRelativeItem();

	void setItem1(cv::Rect input);
	void setItem2(cv::Rect input);
	
	bool findLocations(Mat* haystack, Mat* needle, int offsetX, int offsetY);
};

