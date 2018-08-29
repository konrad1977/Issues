/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "MultiLineStringView.h"
#include "MultiLineTextDrawer.h"

#include <posix/string.h>
#include <posix/stdlib.h>

MultiLineStringView::MultiLineStringView(BRect rect)
	:BView(rect, "MultiLine", B_FOLLOW_ALL, B_WILL_DRAW | B_FRAME_EVENTS )
	,fText(NULL)	
	,fMultiLineTextDrawer(NULL)
	,previousWidth(0.0)
{
	fMultiLineTextDrawer = new MultiLineTextDrawer(this);
}

MultiLineStringView::~MultiLineStringView()
{
	free(fText);
	delete fMultiLineTextDrawer;
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
	fMultiLineTextDrawer->SetTextColor(color);
}

void 
MultiLineStringView::SetAlignment(alignment align)
{
	fMultiLineTextDrawer->SetAlignment(align);
}

void 
MultiLineStringView::SetTextColor(uchar red, uchar green, uchar blue, uchar alpha) 
{
	rgb_color color = { red, green, blue, alpha };
	fMultiLineTextDrawer->SetTextColor(color);
}

void 
MultiLineStringView::SetInsets(BSize size)
{
	fMultiLineTextDrawer->SetInsets(size);
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
	BFont font;
	GetFont(&font);
	fMultiLineTextDrawer->DrawString(r, fText, &font);
}	
