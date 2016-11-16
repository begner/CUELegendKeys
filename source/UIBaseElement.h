#pragma once

#include "ImageFilterMat.h"

class UIBaseElement
{
private:
	int posX = 0;
	int posY = 0;
	int offsetX = 0;
	int offsetY = 0;
	int onClickId = -1;
	bool needUpdate = false;
	bool visibility = true;
	bool disabled = false;
	bool captureMouseEvents = false;
public:
	UIBaseElement();
	~UIBaseElement();


	void show();
	void hide();
	void setVisibility(bool state);

	bool getVisibility();

	virtual bool isDisabled();

	virtual void setPos(int x, int y);
	void setOffset(int x, int y);

	virtual int getX();
	virtual int getY();

	void needsUpdate(bool state);

	virtual void processUI(Mat4b* drawUI) = 0;
	virtual int getWidth() = 0;
	virtual int getHeight() = 0;

	virtual bool onMouseOn(int x, int y);
	virtual bool onMouseOut();
	virtual bool needsUpdate();
	
	void setCaptureMouseEvents(bool state);
	bool getCaptureMouseEvents();

	void setOnClickId(int id);
	virtual int getOnClickId(int mouseX = 0, int mouseY = 0);

};

