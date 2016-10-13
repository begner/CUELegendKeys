#pragma once

#include "ImageFilterMat.h"

class UIBaseElement
{
private:
	int posX = 0;
	int posY = 0;

	int onClickId;
	bool needUpdate = false;
public:
	UIBaseElement();
	~UIBaseElement();


	virtual void setPos(int x, int y);
	virtual int getX();
	virtual int getY();
	void needsUpdate(bool state);

	virtual void processUI(Mat4b* drawUI) = 0;
	virtual int getWidth() = 0;
	virtual int getHeight() = 0;

	virtual bool onMouseOn(int x, int y);
	virtual bool onMouseOut();
	virtual bool needsUpdate();
	
	
	void setOnClickId(int id);
	int getOnClickId();

};

