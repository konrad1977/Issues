/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
 
#ifndef MULTILINETEXTVIEW_H
#define MULTILINETEXTVIEW_H

#include <SupportDefs.h>
#include <View.h>

class Drawer;
class MultiLineStringView : public BView {
public:
	MultiLineStringView(BRect rect);
	~MultiLineStringView();
	
	virtual void FrameResized(float width, float height);
	virtual void Draw(BRect rect);
	
			void SetText(const char *text);
			void SetTextColor(rgb_color color);
			void SetTextColor(uchar red, uchar green, uchar blue, uchar alpha = 255);
private:
	char *fText;
	Drawer *fDrawer;
	float previousWidth;
};

#endif // _H
