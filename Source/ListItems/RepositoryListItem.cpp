/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "RepositoryListItem.h"
#include "GithubRepository.h"

#include <interface/ListView.h>
#include <posix/stdio.h>

RepositoryListItem::RepositoryListItem(GithubRepository *repository)
	:BListItem()
	,fRepository(repository)
	,fMultiLineTextDrawer(NULL)
	,fHeight(30)
{

}
	
RepositoryListItem::~RepositoryListItem()
{
	delete fMultiLineTextDrawer;
}

void 
RepositoryListItem::DrawItem(BView *view, BRect rect, bool complete)
{
	BListView *parent = dynamic_cast<BListView *>(view);
	const int32 index = parent->IndexOf(this);
	BRect frame = parent->ItemFrame(index);
	
	if (fMultiLineTextDrawer == NULL) {
		fMultiLineTextDrawer = new MultiLineTextDrawer(parent);
	//	fDrawer->SetTransparency(255);
		fMultiLineTextDrawer->SetInsets(BSize(10,0));
	}
	
	rgb_color backgroundColor = ui_color(B_LIST_BACKGROUND_COLOR);
	
	if (IsSelected()) {
		parent->SetHighColor(ui_color(B_LIST_SELECTED_BACKGROUND_COLOR));
	} else if (index % 2 == 0) {
		parent->SetHighColor(backgroundColor);
	} else {
		parent->SetHighColor(tint_color(backgroundColor, 1.02));
	}
	
	parent->SetDrawingMode(B_OP_COPY);
	parent->FillRect(frame);
	parent->SetDrawingMode(B_OP_OVER);
	
	DrawRepository(frame);
	parent->FrameResized(rect.Width(), rect.Height());	
}

void 
RepositoryListItem::DrawRepository(BRect rect)
{
	BRect frame = rect;
	BFont font(be_bold_font);
	font.SetSize(13.0);
	fMultiLineTextDrawer->SetTextColor( 0, 0, 0);

	fMultiLineTextDrawer->SetFont(&font);
	float height = fMultiLineTextDrawer->DrawString(frame, fRepository->name.String());
	fHeight = height;

	font = be_plain_font;
	font.SetSize(12);
	frame = frame.OffsetBySelf(0, height);
	fMultiLineTextDrawer->SetTextColor( 70, 72, 71);
	fMultiLineTextDrawer->SetFont(&font);
	height = fMultiLineTextDrawer->DrawString(frame, fRepository->description.Trim().String());
	fHeight += height;
	
	frame = frame.OffsetBySelf(0, height);
	fMultiLineTextDrawer->SetFont(&font);
	fMultiLineTextDrawer->SetTextColor( 40, 42, 154);
	height = fMultiLineTextDrawer->DrawString(frame, fRepository->url.String());
	fHeight += height + 10;
}

void 
RepositoryListItem::Update(BView *view, const BFont *font)
{
	if (fPreviousHeight != fHeight) {
		fPreviousHeight = fHeight;
		SetHeight(fHeight);
	}
}