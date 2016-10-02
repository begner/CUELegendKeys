#include "HotSpotGroup.h"

HotSpotGroup::HotSpotGroup()
{
}


HotSpotGroup::~HotSpotGroup()
{
}

void HotSpotGroup::setReferenceX(int x) {
	referenceX = x;
}

void HotSpotGroup::setReferenceY(int y) {
	referenceY = y;
}

void HotSpotGroup::setLolUIScale(int scale) {
	lolUIScale = scale;
}


/*int HotSpotGroup::translateCoords(x) {
	int minSize = 650;


}
*/

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

