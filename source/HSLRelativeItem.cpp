#include "HSLRelativeItem.h"


HSLRelativeItem::HSLRelativeItem(string title, string saveId, int needleResource, int sItem) : HSLBase(title, saveId, needleResource)
{
	searchItem = sItem;
}

HSLRelativeItem::~HSLRelativeItem()
{

}

string HSLRelativeItem::getLearningRequiredText() {
	return "The Item 1 and Item 2 needs to be saved to autodetect this Element";
}

bool HSLRelativeItem::isLearningPossible() {
	return (ImageFilterMat::isValidRect(item1) && ImageFilterMat::isValidRect(item2));
}

void HSLRelativeItem::setItem1(cv::Rect input) {
	item1 = input;
}

void HSLRelativeItem::setItem2(cv::Rect input) {
	item2 = input;
}

bool HSLRelativeItem::findLocations(Mat* haystack, Mat* needle, int offsetX, int offsetY) {
	if (item1.x < 1 || item1.y < 1 || item1.width < 1 || item1.height < 1 ||
		item2.x < 1 || item2.y < 1 || item2.width < 1 || item2.height < 1) {
		NuLogger::getInstance()->log("Cant learn Relative item - i1 & i2 not saved!");
		return false;
	}
		
	int left = item1.x;
	int top = item1.y;
	int itemSpaceX = item2.x - (item1.x + item1.width);
	int itemSpaceY = 3;

	cv::Rect output(0, 0, item1.width, item1.height);
	switch (searchItem) {
	case 3:
		output.x = item2.x + (item2.width + itemSpaceX)*1;
		output.y = item1.y;
		break;
	case 4:
		output.x = item2.x + (item2.width + itemSpaceX)*2;
		output.y = item1.y;
		break;
	case 5:
		output.x = item1.x;
		output.y = item1.y + item1.height + itemSpaceY;
		break;
	case 6:
		output.x = item2.x;
		output.y = item1.y + item1.height + itemSpaceY;
		break;
	case 7:
		output.x = item2.x + (item2.width + itemSpaceX) * 1;
		output.y = item1.y + item1.height + itemSpaceY;
		break;
	}

	addFoundLocation(output);
	return true;
}