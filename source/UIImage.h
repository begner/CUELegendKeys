#pragma once

#include "UIBaseElement.h"

class UIImage :
	public UIBaseElement
{
private:
	bool useNextImage = false;
	Mat currentImage;
public:
	UIImage();
	UIImage(int resourceNormal, int resourceMask);
	~UIImage();
	void processUI(Mat4b* drawUI);
	void setImage(Mat* src);
	int getWidth();
	int getHeight();
};

