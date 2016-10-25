#include "HotSpotGroup.h"

HotSpotGroup::HotSpotGroup()
{
}


HotSpotGroup::~HotSpotGroup()
{
}

ScreenHotSpot* HotSpotGroup::getHotSpotAt(int index) {
	return hotSpots[index];
}

void HotSpotGroup::addHotSpot(ScreenHotSpot* hs) {
	hotSpots.push_back(hs);
	hs->initialize();
}

vector<ScreenHotSpot *> HotSpotGroup::getHotSpots() {
	return hotSpots;
}

