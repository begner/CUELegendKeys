#pragma once

#include "ImageFilterMat.h"


class HotSpotLearn 
{
private:
	Mat* screenshotRaw;

	int clientWidth = 0;
	int clientHeight = 0;

	// Found Areas
	cv::Rect HSRectSkillQ;
	cv::Rect HSRectSkillW;
	cv::Rect HSRectSkillE;
	cv::Rect HSRectSkillR;
	cv::Rect HSRectSkillD;
	cv::Rect HSRectSkillF;

	cv::Rect HSRectItem1;
	cv::Rect HSRectItem2;
	cv::Rect HSRectItem3;
	cv::Rect HSRectItem4;
	cv::Rect HSRectItem5;
	cv::Rect HSRectItem6;
	cv::Rect HSRectItem7;

	cv::Rect HSRectBarHeal;
	cv::Rect HSRectBarMana;

	/*
	Mat haystackImage;
	int match_method;
	int max_Trackbar = 5;
	
	
	boolean findAllItemHotSpots();
	boolean findResourceHotSpots();
	boolean findAllSkillHotSpots();

	boolean findTemplate(Mat4b sourceTempl, cv::Rect* outputRect, Mat* foundImage);
	boolean findMatching(int match_method, Mat* needleImage, cv::Rect* foundRect);
	*/
	void findAllHotSpots();
	boolean takeScreenshot();

public:
	HotSpotLearn();
	~HotSpotLearn();
	
	
	void start();
	
};

