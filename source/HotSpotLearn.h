#pragma once

#include "Resource.h"
#include "ImageFilterMat.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

class HotSpotLearn
{
private:
	/*
	Mat originalImage;
	Mat haystackImage;
	Mat* screenshotRaw;
	
	int match_method;
	int max_Trackbar = 5;
	void cannyThreshold(Mat src, Mat* dest);
	
	boolean findAllItemHotSpots();
	boolean findResourceHotSpots();
	boolean findAllSkillHotSpots();

	boolean findTemplate(Mat4b sourceTempl, cv::Rect* outputRect, Mat* foundImage);
	boolean findMatching(int match_method, Mat* needleImage, cv::Rect* foundRect);
	

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
	
	*/
public:
	HotSpotLearn();
	~HotSpotLearn();
	
	void findAllHotSpots();
	
};

