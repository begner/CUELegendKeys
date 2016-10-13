#pragma once

#include "ImageFilterMat.h"

class HSLBase
{
private:
	vector<cv::Rect> foundLocations;
	
	Mat originalImage;
	Mat haystackImage;
	Mat needleImage;

public:
	HSLBase();
	virtual ~HSLBase();

	void setHaystack(Mat hs);
	Mat* getHaystack();

	void setNeedle(Mat n);
	void setNeedle(int templateResourceId);
	Mat* getNeedle();

	virtual boolean findLocations() = 0;

	void addFoundLocation(cv::Rect foundRect);
	vector<cv::Rect> getFoundLocations();
	Mat* getPreviewMatForLocationIndex(int idx);
};

