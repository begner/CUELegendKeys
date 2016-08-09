#include "HotSpotLearn.h"



HotSpotLearn::HotSpotLearn()
{
}


HotSpotLearn::~HotSpotLearn()
{
}


void HotSpotLearn::findAllHotSpots() {
	/*
	HWND desktop = GetDesktopWindow();
	HDC desktopDC = GetDC(desktop);

	RECT targetRect;
	GetWindowRect(desktop, &targetRect);

	int clientWidth = targetRect.right - targetRect.left;
	int clientHeight = targetRect.bottom - targetRect.top;

	Mat* screenshotRaw = ImageFilterMat::hdc2mat(desktopDC, 0, 0, clientWidth, clientHeight);
	if (screenshotRaw == NULL) {
		return;
	}

	originalImage = (Mat4b)*screenshotRaw;
	haystackImage = (Mat4b)*screenshotRaw;
	cannyThreshold(haystackImage, &haystackImage);

	vector<int> allSkills;
	allSkills.push_back(IDB_AUTO_LEARN_SKILL_Q);
	allSkills.push_back(IDB_AUTO_LEARN_SKILL_W);
	allSkills.push_back(IDB_AUTO_LEARN_SKILL_E);
	allSkills.push_back(IDB_AUTO_LEARN_SKILL_R);
	allSkills.push_back(IDB_AUTO_LEARN_SKILL_D);
	allSkills.push_back(IDB_AUTO_LEARN_SKILL_F);

	for (vector<int>::iterator it = allSkills.begin(); it != allSkills.end(); ++it) {
		findTemplate(ImageFilterMat::loadResourceAsMat(*it));
	}
	*/
}


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


bool HotSpotLearn::findMatching(int match_method, Mat* needleImage, cv::Rect* foundRect)
{
	Mat result;

	/// Create the result matrix
	int result_cols = haystackImage.cols - needleImage->cols + 1;
	int result_rows = haystackImage.rows - needleImage->rows + 1;

	result.create(result_rows, result_cols, CV_32FC1);

	/// Do the Matching and Normalize
	matchTemplate(haystackImage, *needleImage, result, match_method);
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

	/// Localizing the best match with minMaxLoc
	double minVal; double maxVal;
	cv::Point minLoc;
	cv::Point maxLoc;
	cv::Point matchLoc;

	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

	/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
	if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED)
	{
		matchLoc = minLoc;
	}
	else
	{
		matchLoc = maxLoc;
	}

	/// Show me what you got
	// rectangle(img_display, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
	// rectangle(result, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);

	// imshow(image_window, img_display);
	// imshow(result_window, result);

	if (matchLoc.x > 0 && matchLoc.y > 0) {
		foundRect->x = matchLoc.x;
		foundRect->y = matchLoc.y;
		foundRect->width = needleImage->cols;
		foundRect->height = needleImage->rows;
		return true;
	}

	return false;

}

*/