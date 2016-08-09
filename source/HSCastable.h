#pragma once
#include "ScreenHotSpot.h"

class HSCastable :
	public ScreenHotSpot

{
private:
	Vec4b getColorByTick(int offset = 0);

public:
	HSCastable();
	~HSCastable();
	int getType();
	
	virtual bool isCastable();
	void filterMat();
	Vec4b getCurrentColor(int index = 0);
	int getMaxTick();
};

