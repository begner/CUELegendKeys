#include "HotSpotLearn.h"

HotSpotLearn::HotSpotLearn()
{

}


HotSpotLearn::~HotSpotLearn()
{
}

void HotSpotLearn::findSkill(int learnSkillIDB) {
	// HSLSkill skill = HSLSkill();
	/*
	hsl.setNeedle(learnSkillIDB);
	hsl.setHaystack(*screenshotRaw);
	hsl.findHotSpot(learnSkillIDB);
		
	
	}
	*/

	
}

// void HotSpotLearn::findHotSpot(int learnSkillIDB) {
	
	// HSLSkill mySkill = HSLSkill();
	/*
	skillQ.setTemplateResourceId(IDB_AUTO_LEARN_SKILL_Q);
	skillQ.setHaystack(&haystackImage);
	
	// prepare Images
	Mat originalImage = (Mat4b)*screenshotRaw;
	
	vector<HSLSkill> allSkills;

	


	allSkills.push_back(make_pair(IDB_AUTO_LEARN_SKILL_Q, HSRectSkillQ));
	allSkills.push_back(make_pair(IDB_AUTO_LEARN_SKILL_W, HSRectSkillW));
	allSkills.push_back(make_pair(IDB_AUTO_LEARN_SKILL_E, HSRectSkillE));
	allSkills.push_back(make_pair(IDB_AUTO_LEARN_SKILL_R, HSRectSkillR));
	allSkills.push_back(make_pair(IDB_AUTO_LEARN_SKILL_D, HSRectSkillD));
	allSkills.push_back(make_pair(IDB_AUTO_LEARN_SKILL_F, HSRectSkillF));

	for (vector<pair<int, cv::Rect*>>::iterator it = allSkills.begin(); it != allSkills.end(); ++it) {
		int resourceID = it->first;
		cv::Rect* targetRect = it->second;

		findTemplate(ImageFilterMat::loadResourceAsMat(*it));
	}
	*/
// }


/*

void HotSpotLearn::findTemplate(Mat4b sourceTempl) {
	NuLogger::getInstance()->log("-------");
	Mat needleImage;

	int minDim = 42; // <-- thats no joke ;)
					 // minDim = min(sourceTempl.cols, sourceTempl.rows);

	cv::Rect foundRect(0, 0, 0, 0);
	bool foundArea = false;

	for (int i = 0; i < minDim - 1; i++) {
		cv::Size newSize(sourceTempl.cols - i, sourceTempl.rows - i);
		resize(sourceTempl, needleImage, newSize, 0, 0, INTER_CUBIC);
		cannyThreshold(needleImage, &needleImage);
		
		// CV_TM_SQDIFF = 0,
		// CV_TM_SQDIFF_NORMED = 1,
		// CV_TM_CCORR = 2,
		// CV_TM_CCORR_NORMED = 3,
		// CV_TM_CCOEFF = 4,
		// CV_TM_CCOEFF_NORMED = 5
		
		if (findMatching(CV_TM_SQDIFF_NORMED, &needleImage, &foundRect)) {
			foundArea = true;
			rectangle(originalImage, foundRect, Scalar(0, 0, 255, 128), 1, 8, 0);
			NuLogger::getInstance()->log("Found at %i", i);
			break;
		}
		else {

		}
	}

	if (foundArea) {
		NuLogger::getInstance()->log("FOUND!");
	}
	else {
		NuLogger::getInstance()->log("NOT FOUND!");
	}
}

void HotSpotLearn::cannyThreshold(Mat src, Mat* dest)
{
	Mat src_gray;

	int lowThreshold = 50; // max_lowThreshold = 100;
	int kernel_size = 3;
	int ratio = 3;

	cvtColor(src, src_gray, CV_BGR2GRAY);

	/// Reduce noise with a kernel 3x3
	blur(src_gray, *dest, Size(3, 3));

	/// Canny detector
	Canny(*dest, *dest, lowThreshold, lowThreshold*ratio, kernel_size);
}



*/