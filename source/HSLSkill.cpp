#include "HSLSkill.h"



HSLSkill::HSLSkill()
{
}


HSLSkill::~HSLSkill()
{
}

boolean HSLSkill::findLocations() {

	ImageFilterMat::cannyThreshold(*getHaystack(), getHaystack());

	Mat needleItteration;

	int minDim = 42; // <-- thats no joke ;)
					 // minDim = min(sourceTempl.cols, sourceTempl.rows);

	cv::Rect foundRect(0, 0, 0, 0);
	
	bool foundArea = false;

	for (int i = 0; i < minDim - 1; i++) {
		cv::Size newSize(getNeedle()->cols - i, getNeedle()->rows - i);
		resize(*getNeedle(), needleItteration, newSize, 0, 0, INTER_CUBIC);
		ImageFilterMat::cannyThreshold(needleItteration, &needleItteration);

		// CV_TM_SQDIFF = 0,
		// CV_TM_SQDIFF_NORMED = 1,
		// CV_TM_CCORR = 2,
		// CV_TM_CCORR_NORMED = 3,
		// CV_TM_CCOEFF = 4,
		// CV_TM_CCOEFF_NORMED = 5
		if (ImageFilterMat::getMatchingRect(CV_TM_SQDIFF_NORMED, getHaystack(), &needleItteration, &foundRect)) {
			foundArea = true;
			addFoundLocation(foundRect);
			NuLogger::getInstance()->log("Found at %i", i);
		//	break;
		}
	}

	if (foundArea) {
		NuLogger::getInstance()->log("FOUND!");
	}
	else {
		NuLogger::getInstance()->log("NOT FOUND!");
	}
	return foundArea;
}
