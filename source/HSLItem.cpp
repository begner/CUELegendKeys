#include "HSLItem.h"



HSLItem::HSLItem(string title, string saveId, int needleResource) : HSLBase(title, saveId, needleResource)
{
}

HSLItem::~HSLItem()
{

}

void HSLItem::prepareHaystack(Mat* hay) {
	ImageFilterMat::incSaturation(*hay, 0, 2);
	HSLBase::prepareHaystack(hay);
}

void HSLItem::prepareNeedle(Mat* needle) {
	ImageFilterMat::incSaturation(*needle, 0, 2);
	HSLBase::prepareNeedle(needle);
}



