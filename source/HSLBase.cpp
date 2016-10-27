#include "HSLBase.h"

HSLBase::HSLBase(string title, string saveId, int needleResource)
{
	setTitle(title);
	setSaveId(saveId);
	setNeedle(needleResource);
}

HSLBase::~HSLBase()
{
}

void HSLBase::releaseData() {
	haystackImage.release();
	originalImage.release(); 
}


void HSLBase::setNeedle(int templateResourceId) {
	setNeedle(ImageFilterMat::loadResourceAsMat(templateResourceId));
}

void HSLBase::setHaystack(Mat hs) {
	hs.copyTo(haystackImage);
}

void HSLBase::setOriginalImage(Mat hs) {
	hs.copyTo(originalImage);
}

Mat* HSLBase::getHaystack() {
	return &haystackImage;
}

void HSLBase::setNeedle(Mat n) {
	n.copyTo(needleImage);
}

Mat* HSLBase::getNeedle() {
	return &needleImage;
}

void HSLBase::resetLocations() {
	foundLocations = {};
}

vector<cv::Rect> HSLBase::getFoundLocations() {
	return foundLocations;
}
void HSLBase::addFoundLocation(cv::Rect foundRect) {
	foundLocations.push_back(foundRect);
}

cv::Rect HSLBase::getLocationRectByLocationIndex(int idx) {
	return foundLocations[idx];
}

bool HSLBase::isLearningPossible() {
	return true;
}

string HSLBase::getLearningRequiredText() {
	return "";
}

string HSLBase::getTitle() {
	return title;
}

void HSLBase::setTitle(string t) {
	title = t;
}

string HSLBase::getSaveId() {
	return saveId;
}

void HSLBase::setSaveId(string id) {
	saveId = id;
}

void HSLBase::orderLocations() {
	
	vector<cv::Rect> output = {};


	#ifdef _DEBUG
		Mat debugFoundRect(originalImage.rows, originalImage.cols, CV_8UC3);
		for (int i = 0; i < foundLocations.size(); i++) {
			cv::rectangle(debugFoundRect, foundLocations[i], Scalar(0, 0, 255));
		}
	#endif _DEBUG


	// Cluster with DBSCAN (Density-Based Spatial Clustering of Applications with Noise)
	DBScan dbScan(foundLocations, 1, 3);
	dbScan.run();
	
	// put clusters it in map!
	std::map<int, vector<cv::Rect>> map = {};
	for (int i = 0; i < dbScan.data.size(); i++)
	{
		map[dbScan.labels[i]].push_back(dbScan.data[i]);
	}

	// create order mapping
	std::vector<std::pair<int, int>> orderMap;
	for (std::map<int, vector<cv::Rect>>::iterator it = map.begin(); it != map.end(); it++) {
		orderMap.push_back(make_pair(it->first, (int)it->second.size()));
	}

	// sort...
	sort(orderMap.begin(), orderMap.end(), [=](std::pair<int, int>& a, std::pair<int, int>& b)
	{
		return a.second > b.second;
	});

	#ifdef _DEBUG
		Mat debugClustered(originalImage.rows, originalImage.cols, CV_8UC3);
		int score = 1;
		for (std::vector<std::pair<int, int>>::iterator it = orderMap.begin(); it != orderMap.end(); it++) {
			int mapKey = (*it).first;
			
			Scalar color;
			if (mapKey == -1) {
				color = Scalar(80, 80, 80);
			}
			else {
				color = ImageFilterMat::HSVtoRGBcvScalar(rand() % 255, 255, 255);
			}
			
			NuLogger::getInstance()->log("MAP: %i", mapKey);

			putText(debugClustered, to_string(score++) + " = "+to_string(mapKey) + " > " + to_string(map[mapKey].size()), cv::Point(map[mapKey][0].x, map[mapKey][0].y), FONT_HERSHEY_COMPLEX, .5, color, 1);

			for (vector<cv::Rect>::iterator vit = map[mapKey].begin(); vit != map[mapKey].end(); vit++) {

				cv::rectangle(debugClustered, *vit, color);
			}
		}

	#endif _DEBUG


	// popuplate flat map
	NuLogger::getInstance()->log("------------ MAP ORDER ------------");
	for (std::vector<std::pair<int, int>>::iterator it = orderMap.begin(); it != orderMap.end(); it++) {
		int mapKey = (*it).first;
		NuLogger::getInstance()->log("MAP: %i", mapKey);
		
		if (mapKey != -1) {
			output.push_back(medianRect(map[mapKey]));
		}
	}


#ifdef _DEBUG
	Mat debugOutput(originalImage.rows, originalImage.cols, CV_8UC3);
	originalImage.copyTo(debugOutput);
	for (int i = 0; i < output.size(); i++) {
		Scalar color = ImageFilterMat::HSVtoRGBcvScalar(rand() % 255, 255, 255);
		putText(debugOutput, to_string(i), cv::Point(output[i].x, output[i].y), FONT_HERSHEY_COMPLEX, .5, color, 1);
		cv::Rect overScanned(output[i].x - foundOverScan, output[i].y - foundOverScan, output[i].width + foundOverScan * 2, output[i].height + foundOverScan * 2);
		cv::rectangle(debugOutput, overScanned, color);
	}
#endif _DEBUG
	
	foundLocations.empty();
	foundLocations = output;
}




void HSLBase::prepareHaystack(Mat* hay) {
	ImageFilterMat::incSaturation(*hay, 50, 2);
	ImageFilterMat::cannyThreshold(*hay, hay);
}

void HSLBase::prepareNeedle(Mat* needle) {
	ImageFilterMat::incSaturation(*needle, 50, 2);
	ImageFilterMat::cannyThreshold(*needle, needle);
}


bool HSLBase::findLocations() {
	return findLocations(getHaystack(), getNeedle());
}

bool HSLBase::findLocations(Mat* haystack, Mat* needle, int offsetX, int offsetY) {
	
	resetLocations();

	prepareHaystack(haystack);
	// ImageFilterMat::cannyThreshold(*getHaystack(), getHaystack());

	Mat needleItteration;

	int minDim = min(needle->cols, needle->rows);

	cv::Rect foundRect(0, 0, 0, 0);

	bool foundArea = false;

	for (int i = 0; i < minDim - 1; i++) {
		cv::Size newSize(needle->cols - i, needle->rows - i);
		resize(*needle, needleItteration, newSize, 0, 0, INTER_CUBIC);

		prepareNeedle(&needleItteration);
		// ImageFilterMat::cannyThreshold(needleItteration, &needleItteration);
		// CV_TM_SQDIFF = 0,
		// CV_TM_SQDIFF_NORMED = 1,
		// CV_TM_CCORR = 2,
		// CV_TM_CCORR_NORMED = 3,
		// CV_TM_CCOEFF = 4,
		// CV_TM_CCOEFF_NORMED = 5
		if (ImageFilterMat::getMatchingRect(CV_TM_CCORR_NORMED, haystack, &needleItteration, &foundRect)) {
			foundArea = true;
			foundRect.x += offsetX;
			foundRect.y += offsetY;
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

int HSLBase::medianInt(vector<int> vec) {
	if (vec.empty()) return 0;
	else {
		std::sort(vec.begin(), vec.end());
		if (vec.size() % 2 == 0)
			return (vec[vec.size() / 2 - 1] + vec[vec.size() / 2]) / 2;
		else
			return vec[vec.size() / 2];
	}
}

cv::Rect HSLBase::medianRect(vector<cv::Rect> vecRect) {
	cv::Rect output(0, 0, 0, 0);

	vector<int> vecX;
	vector<int> vecY;
	vector<int> vecWidth;
	vector<int> vecHeight;

	for (int i = 0; i < vecRect.size(); i++) {
		vecX.push_back(vecRect[i].x);
		vecY.push_back(vecRect[i].y);
		vecWidth.push_back(vecRect[i].width);
		vecHeight.push_back(vecRect[i].height);
	}

	output.x = medianInt(vecX);
	output.y = medianInt(vecY);
	output.width = medianInt(vecWidth);
	output.height = medianInt(vecHeight);

	return output;
}

bool HSLBase::isWide() {
	return false;
}

bool HSLBase::needsBorder() {
	return true;
}