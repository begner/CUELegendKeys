#pragma once
#include "HSLBase.h"

class HSLItem :
	public HSLBase
{
private:
public:
	HSLItem::HSLItem(string title, string saveId, int needleResource);
	~HSLItem();
	
	void prepareHaystack(Mat* hay);
	void prepareNeedle(Mat* needle);
};

