#pragma once

#include "ImageFilterMat.h"
#include "HSLBase.h"
#include "HSLSkill.h"


class HotSpotLearn 
{
private:
	Mat* screenshotRaw;

	int clientWidth = 0;
	int clientHeight = 0;

	Mat haystackImage;
	int match_method;
	int max_Trackbar = 5;
	
	
	/*
	boolean findAllItemHotSpots();
	boolean findResourceHotSpots();
	boolean findAllSkillHotSpots();

	boolean findTemplate(Mat4b sourceTempl, cv::Rect* outputRect, Mat* foundImage);
	boolean findMatching(int match_method, Mat* needleImage, cv::Rect* foundRect);
	*/

public:
	HotSpotLearn();
	~HotSpotLearn();
	
	void findSkill(int learnSkillIDB);
	// void findAllHotSpots();
	// boolean takeScreenshot();

	
};

