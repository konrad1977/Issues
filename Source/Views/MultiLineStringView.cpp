/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "MultiLineStringView.h"
#include "Drawer.h"

#include <posix/string.h>
#include <posix/stdlib.h>

MultiLineStringView::MultiLineStringView(BRect rect)
	:BView(rect, "MultiLine", B_FOLLOW_ALL, B_WILL_DRAW | B_FRAME_EVENTS )
	,fText(NULL)	
	,fDrawer(NULL)
	,previousWidth(0.0)
{
	fDrawer = new Drawer(this);
}

MultiLineStringView::~MultiLineStringView()
{
	free(fText);
}

void 
MultiLineStringView::SetFont(BFont *font)
{
	BView::SetFont(font);
	Invalidate();
}

void 
MultiLineStringView::SetTextColor(rgb_color color)
{
	fDrawer->SetTextColor(color);
}

void 
MultiLineStringView::SetAligntment(alignment align)
{
	fDrawer->SetAligntment(align);
}

void 
MultiLineStringView::SetTextColor(uchar red, uchar green, uchar blue, uchar alpha = 255) 
{
	rgb_color color = { red, green, blue, alpha };
	fDrawer->SetTextColor(color);
}

void 
MultiLineStringView::SetInsets(BSize size)
{
	fDrawer->SetInsets(size);
}
			
void 
MultiLineStringView::SetInsets(float left, float top, float right, float bottom)
{
	
}

void
MultiLineStringView::SetText(const char *text)
{
	free(fText);
	fText = strdup(text);
	Invalidate();
}

void
MultiLineStringView::FrameResized(float width, float height) 
{
	if (previousWidth != width) {
		Invalidate();
		previousWidth = width;
	}
	
	BView::FrameResized(width, height);
}
	
void 
MultiLineStringView::Draw(BRect rect)
{
	BRect r(Bounds());
	fDrawer->DrawString(r, fText);
}	
