#pragma once

#include "UIBaseElement.h"

class UIImage :
	public UIBaseElement
{
private:
	Mat currentImage;
public:
	UIImage();
	~UIImage();
	void processUI(Mat4b* drawUI);
	void setImage(Mat src);
	int getWidth();
	int getHeight();
};

