/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include "RepositoryTypeItem.h"
#include <interface/ListItem.h>
#include <interface/OutlineListView.h>
#include <stdio.h>
#include <Region.h>

RepositoryTypeItem::RepositoryTypeItem(RepositoryType type)
	:BListItem()
	,fColorManager(NULL)
	,fType(type)
{
	fColorManager = new ColorManager(this, false);
	
	switch (fType) {
		case FORK:
			fTitle = BString("Fork");
			fIndicatorColor = { 193, 188, 194 };
			break;
		case PRIVATE:
			fTitle = BString("Private");
			fIndicatorColor = { 255, 64, 80 };
			break;
		case PUBLIC: 
			fTitle = BString("Public");
			fIndicatorColor = { 38, 201, 158 };
			break;
		default:
			break;
	}
}

RepositoryTypeItem::~RepositoryTypeItem()
{

}	
	
float 
RepositoryTypeItem::FontHeight(const BFont *font) const
{
	font_height fh;
	font->GetHeight(&fh);
	return ceilf(fh.ascent + fh.descent + fh.leading);
}

void 
RepositoryTypeItem::DrawItem(BView *view, BRect rect, bool complete)
{
	BOutlineListView *parent = dynamic_cast<BOutlineListView *>(view);
	const int32 index = parent->IndexOf(this);
	BRect frame = parent->ItemFrame(index);
	
	DrawBackground(parent, frame);

	rgb_color textColor = fColorManager->TextColor();
	parent->SetHighColor(textColor);
	parent->SetDrawingMode(B_OP_OVER);		
	
	BFont font;
	parent->GetFont(&font);
	font_height fh;
	font.GetHeight(&fh);
	
	float fontHeight = fh.ascent + fh.descent + fh.leading;
	float horizontalCenter = ((frame.Height() - fontHeight) / 2.0) + fh.descent;
	parent->MovePenTo(BPoint(20, frame.LeftBottom().y - horizontalCenter));
	parent->DrawString(fTitle.String());
}
	
void 
RepositoryTypeItem::Update(BView *view, const BFont *font)
{	
	float height = FontHeight(font);
	SetHeight(height + 20);
}
	
void 
RepositoryTypeItem::DrawBackground(BOutlineListView *parent, BRect frame)
{	
	BRect r = frame;
	r.right = 14;
	BRegion reg(frame);
	reg.Exclude(r);
	
	parent->SetDrawingMode( B_OP_MIN );
	parent->SetLowColor(fIndicatorColor);
	parent->FillRect(r, B_SOLID_LOW);
	
	rgb_color backgroundColor = fColorManager->BackgroundColor();
	parent->SetHighColor(mix_color(fIndicatorColor, backgroundColor, 168));
	parent->SetDrawingMode(B_OP_COPY);
	parent->FillRect(reg.Frame());
}