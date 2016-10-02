#pragma once

#include "ImageFilterMat.h"

class HSLBase
{
private:
	cv::Rect* targetRect;
	int templateResourceId;
	
	Mat haystackImage;
	Mat needleImage;

public:
	HSLBase();
	virtual ~HSLBase();

	void setTargetRect(cv::Rect* tRect);
	cv::Rect* getTargetRect();

	void setTemplateResourceId(int rid);

	void setHaystack(Mat hs);
	Mat* getHaystack();

	void setNeedle(Mat n);
	Mat* getNeedle();

	virtual boolean findLocation() = 0;

};

