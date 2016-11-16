#include "HSCastable.h"



HSCastable::HSCastable()
{
}


HSCastable::~HSCastable()
{
}

int HSCastable::getType() {
	return ScreenHotSpot::TYPE_CASTABLE;
}

bool HSCastable::isCastable() {
	if (!isCacheDirty()) {
		return isCastableCache;
	}
	
	bool isCastable;
	
	cv::Rect castDetectionRect(0, 0, getBorder(), getBorder());
	Mat4b myMat = getOriginalMat()->clone();
	if (!ImageFilterMat::isValidRect(&myMat, castDetectionRect)) {
		return false;
	}
	Mat4b castDetectionMat(myMat, castDetectionRect);

	ImageFilterMat::colorReduce(castDetectionMat, 64);
	isCastable = ImageFilterMat::colorIsPresent(castDetectionMat, 96, 224, 224) ||
				ImageFilterMat::colorIsPresent(castDetectionMat, 96, 160, 224) || 
				ImageFilterMat::colorIsPresent(castDetectionMat, 96, 160, 160);
	castDetectionMat.release();
	
	isCastableCache = isCastable;
	return isCastable;
}

void HSCastable::filterMat() {
	// getFilteredMat();
	if (!getFilteredMat()->empty()) {
		ImageFilterMat::killDarkPixel(*getFilteredMat(), 60);
		ImageFilterMat::killGrayPixel(*getFilteredMat(), 60);

		// ImageFilterMat::colorReduce(*filteredMat, 64);
		blur(*getFilteredMat(), *getFilteredMat(), cv::Size(5, 5), cv::Point(-1, -1));
		ImageFilterMat::saturation(*getFilteredMat(), 0, 255, 1.5);
	}
}



Vec4b HSCastable::getCurrentColor(int index) {
	if (!isCacheDirty()) {
		return currentColorCache;
	}
		

	Vec4b color(0, 0, 0, 255);
	int brightnessTreshold = 50;
	int r, g, b = 0;

	if (isCastable()) {
		int count = 0;
		int max = (getFilteredMat()->cols * getFilteredMat()->rows);

		while (count < max) {
			color = getColorByTick(count);
			r = color[2];
			g = color[1];
			b = color[0];
			
			if (r > brightnessTreshold || g > brightnessTreshold || b > brightnessTreshold) {
				
				cv::Point curPoint = getCoordsByTick(count);
				setCurrentColorCoord(curPoint.x, curPoint.y);
				break;
			}
			else {
				tick();
			}
			count++;
		}

	}

	currentColorCache = color;
	return color;
}

cv::Point HSCastable::getCoordsByTick(int offset) {
	Mat4b *fMat = getFilteredMat();
	int pos = getCurrentTick() + offset;

	// overflow...
	int max = getMaxTick();
	if (pos > max - 1) {
		pos = pos - max;
	}

	int x = (pos % fMat->cols);
	int y = (int)floor(pos / (fMat->cols));
	
	return cv::Point(x, y);
}


int HSCastable::getMaxTick() {
	return getFilteredMat()->cols*getFilteredMat()->rows;
}


Vec4b HSCastable::getColorByTick(int offset) {
	Mat4b *fMat = getFilteredMat();

	cv::Point curPoint = getCoordsByTick(offset);

	// TODO: if y , x is out of range...
	Vec4b color = fMat->at<Vec4b>(curPoint.y, curPoint.x);
	return color;
}



/*
// ImageFilterMat::saturation(hs->getMatRespectBorders(), 180, 255, 1.5);
// blur(hotspotMat, hotspotMat, cv::Size(5, 5), cv::Point(-1, -1));
// ImageFilterMat::colorReduce(hotspotMat, 64);


// ImageFilterMat::saturation(hotspotMat, 100, 255, 1);
// ImageFilterMat::DrawToHDC(hdc, hotspotMat, hs->getBitmapX(), hs->getBitmapY(), hs->getWidth(true), hs->getHeight(true));


// check if spell is castable...

// ImageFilterMat::saturation(castDetectionMat, 0, 255, 1);
// ImageFilterMat::colorReduce(castDetectionMat, 128);

// ImageFilterMat::DrawToHDC(hdc, castDetectionMat, hs->getBitmapX(), hs->getBitmapY()+100, hs->getBorder(), it->getBorder());


}
*/