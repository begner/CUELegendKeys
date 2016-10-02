#include "HSBackport.h"


HSBackport::HSBackport()
{

}


HSBackport::~HSBackport()
{
}

void HSBackport::initialize() {
	
	// create mask
	
	int cW = getCaptureWidth(true);
	int cH = getCaptureHeight(true);
	int radius = cW - iconRadius;

	mask = Mat4b(cW, cH, CV_8UC4);
	cv::rectangle(mask, cv::Point(0, 0), cv::Point(cW, cH), Scalar(0, 0, 0, 255), CV_FILLED, 8, 0);
	cv::circle(mask, cv::Point((int)floor(cW / 2), (int)floor(cH / 2)), (int)floor(radius / 2), Scalar(255, 255, 255, 255), CV_FILLED, CV_AA, 0);
	
	// create effect animation mat
	int matW = 0;
	int matH = 0;

	allKeys = LEDController::getInstance()->getKeyPositions();
	vector<CorsairLedPosition>::iterator it = allKeys.begin();
	for (; it != allKeys.end(); ) {
		matW = max(matW, (int)ceil(it->left + it->width));
		matH = max(matH, (int)ceil(it->top + it->height));
		++it;
	}
	
	CorsairLedPosition kPos = LEDController::getInstance()->getKeyPositionById(getKey());
	bCenterPoint = cv::Point((int)floor(kPos.left + kPos.width / 2), (int)floor(kPos.top + kPos.height / 2));
	
	effectMat = Mat4b(matH, matW, CV_8UC4);

	effectColorsMat = ImageFilterMat::loadResourceAsMat(IDB_BACKPORTCOLORS);
	// effectColorsMat = Mat4b(90, 840, Vec4b(255, 125, 0, 255));
		
}

void HSBackport::setBackportDetectionCaptureCoordinates(int x, int y, int width, int height) {
	backPortDetectionX = x;
	backPortDetectionY = y;
	backPortDetectionW = width;
	backPortDetectionH = height;
}

int HSBackport::getType() {
	return ScreenHotSpot::TYPE_BACKPORT;
}

bool HSBackport::isCastable() {
	return true;
}


void HSBackport::initializeFrame() {
	HSCastable::copyMats();
	HSCastable::filterMat();
	ImageFilterMat::addAlphaMask(getOriginalMatRespectBorders(), &mask);
}


void HSBackport::filterMat() {
	
	ImageFilterMat::killGrayPixel(*getFilteredMat(), 127);

	blur(*getFilteredMat(), *getFilteredMat(), cv::Size(5, 5), cv::Point(-1, -1));
	ImageFilterMat::saturation(*getFilteredMat(), 0, 255, 1.5);
	
}

bool HSBackport::hasExclusiveEffect() {
	return (isBackportActive()); // || backportFinishedReleaseFrames < backportFinishedReleaseTreshold);
}

void HSBackport::doExclusiveEffect() {
	
	int percent = 100 - getBackportProgress(); // + backportFinishedReleaseFrames;

	// int nc = effectColorsMat->cols*effectColorsMat->channels();					// number of elements per line
	// get the address of row 0
	// uchar* data = effectColorsMat.ptr<uchar>(0);

	int halfWidth = (int)floor(effectMat.cols / 2);
	// int rowSize = effectColorsMat.cols * effectColorsMat.channels();

	for (int c = halfWidth; c > 0; c--) {
		for (int d = 0; d < 90; d++) {
			
			int colorAddressX = halfWidth - c + (int)floor((effectColorsMat.cols - halfWidth) / 100 * percent);
			if (colorAddressX >= effectColorsMat.cols) {
				colorAddressX = effectColorsMat.cols - 1;
			}
			int colorAddressY = d;

			// int colorAddressj = ((halfWidth - c + (int)floor((effectColorsMat.cols - halfWidth) / 100 * percent)) * effectColorsMat.channels()) + (rowSize * d);
			// Scalar sColor(data[colorAddressj], data[colorAddressj + 1], data[colorAddressj + 2], 255);
			
			Vec4b myColor = effectColorsMat.at<Vec4b>(colorAddressY, colorAddressX);
			Scalar sColor(myColor[0], myColor[1], myColor[2], 255);
			
			// cv::circle(effectMat, bCenterPoint, c*2, sColor, CV_FILLED, 8, 0);
			cv::ellipse(effectMat,
				bCenterPoint,
				Size(c, c),
				0, 
				d*4, 
				(d+1)*4,
				sColor, CV_FILLED, 8, 0);
		}
	}
	
	int test = 1;
	// effectMat
	
	for (vector<CorsairLedPosition>::iterator it = allKeys.begin(); it != allKeys.end(); ++it) {

		cv::Rect testRect((int)it->left, (int)it->top, (int)it->width, (int)it->height);
		Mat4b testMat = Mat(effectMat, testRect);
		resize(testMat, testMat, cv::Size(1, 1), 0, 0, INTER_CUBIC);

		Vec4b color = testMat.at<Vec4b>(0, 0);
		LEDController::getInstance()->setKey(it->ledId, color[2], color[1], color[0]);
		testMat.release();
	}

//	backportFinishedReleaseFrames++;

}


bool HSBackport::isBackportActive() {

	// Backport Detection
	cv::Rect bar(backPortDetectionX, backPortDetectionY, backPortDetectionW, backPortDetectionH);
	backPortMat = Mat4b(*getScreenshotMat(), bar);

	// ImageFilterMat::whiteToDarkPixel(backPortMat, 150);
	
	ImageFilterMat::saturation(backPortMat, 50, 255, 2);
	ImageFilterMat::colorReduce(backPortMat, 128);
	ImageFilterMat::killDarkPixel(backPortMat, 65);

	Vec3b borderColor1(64, 192, 192);
	Vec3b borderColor2(64, 64, 192);
	bool isBackporting = ImageFilterMat::hasPixelBorder(backPortMat, 1, 1, 1, 1, borderColor1, borderColor2);

	if (isBackporting) {
		cv::Rect barFill(getBorder()+2, getBorder()+1, backPortDetectionW - (getBorder()+1)*2, backPortDetectionH - (getBorder()+1)*2);
		backPortMeasureMat = Mat(backPortMat, barFill);
		backportFinishedReleaseFrames = 0;
	}
	
	return isBackporting;
	

}

int HSBackport::getBackportProgress() {

	int percent = ImageFilterMat::getBarPercentage(backPortMeasureMat);
	return percent;
	

}

