#pragma once

#include "ImageFilterMat.h"
#include "opencv2/features2d/features2d.hpp"
#include "DBScan.h"

class HSLBase
{
private:
	vector<cv::Rect> foundLocations = {};
	string title = "";
	string saveId = "";
	Mat originalImage;
	Mat haystackImage;
	Mat needleImage;
	
	int foundOverScan = 5;
	int medianInt(vector<int> vec);
	cv::Rect medianRect(vector<cv::Rect> vec);
	void resetLocations();
public:
	HSLBase(string title, string saveId, int needleResource);
	virtual ~HSLBase();
	void releaseData();

	string getTitle();
	void setTitle(string t);

	string getSaveId();
	void setSaveId(string t);

	virtual void setHaystack(Mat hs);
	virtual void setOriginalImage(Mat hs);
	Mat* getHaystack();

	void setNeedle(Mat n);
	void setNeedle(int templateResourceId);
	Mat* getNeedle();

	virtual bool findLocations();
	virtual bool findLocations(Mat* hayStack, Mat* needle, int offsetX = 0, int offsetY = 0);
	void orderLocations();
	cv::Rect getLocationRectByLocationIndex(int idx);


	void addFoundLocation(cv::Rect foundRect);
	vector<cv::Rect> getFoundLocations();
	
	virtual void prepareHaystack(Mat* hay);
	virtual void prepareNeedle(Mat* needle);
	
	virtual bool isWide();
	virtual bool needsBorder();
};

