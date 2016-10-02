#pragma once

#include <vector>
#include "ScreenHotspot.h"


class HotSpotGroup
{

private:
	vector<ScreenHotSpot*> hotSpots;
	int lolUIScale = 0;
	int referenceY = 0;
	int referenceX = 0;

public:
	static const int TYPE_CASTABLE;
	static const int TYPE_BAR;

	HotSpotGroup();
	~HotSpotGroup();
	void addHotSpot(ScreenHotSpot *hs);
	ScreenHotSpot* getHotSpotAt(int index);
	vector<ScreenHotSpot*> getHotSpots();

	void setReferenceX(int x);
	void setReferenceY(int y);
	void setLolUIScale(int scale);




};

