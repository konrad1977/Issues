/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "IssueListItem.h"
#include "GithubIssue.h"

#include <interface/ListView.h>
#include <posix/stdio.h>

IssueListItem::IssueListItem(GithubIssue *issue)
	:BListItem()
	,fIssue(issue)
	,fMultiLineTextDrawer(NULL)
	,fHeight(30)
{

}
	
IssueListItem::~IssueListItem()
{
	delete fMultiLineTextDrawer;
}

void 
IssueListItem::DrawItem(BView *view, BRect rect, bool complete)
{
	BListView *parent = dynamic_cast<BListView *>(view);
	const int32 index = parent->IndexOf(this);
	BRect frame = parent->ItemFrame(index);
	
	if (fMultiLineTextDrawer == NULL) {
		fMultiLineTextDrawer = new MultiLineTextDrawer(parent);
		fMultiLineTextDrawer->SetInsets(BSize(10,0));
	}
	
	rgb_color backgroundColor = ui_color(B_LIST_BACKGROUND_COLOR);
	
	if (IsSelected()) {
		parent->SetHighColor(ui_color(B_LIST_SELECTED_BACKGROUND_COLOR));
	} else if (index % 2 == 0) {
		parent->SetHighColor(backgroundColor);
	} else {
		parent->SetHighColor(tint_color(backgroundColor, 1.05));
	}
	
	parent->SetDrawingMode(B_OP_COPY);
	parent->FillRect(frame);
	parent->SetDrawingMode(B_OP_OVER);
	
	DrawIssue(frame);
	parent->FrameResized(rect.Width(), rect.Height());	
}

void 
IssueListItem::DrawIssue(BRect rect)
{
	BRect frame = rect;
	BFont font(be_bold_font);
	font.SetSize(12.0);
	fMultiLineTextDrawer->SetTextColor( 0, 0, 0);

	fMultiLineTextDrawer->SetFont(&font);
	float height = fMultiLineTextDrawer->DrawString(frame, fIssue->title.String());
	fHeight = height;

	font = be_plain_font;
	font.SetSize(12);
	frame = frame.OffsetBySelf(0, height);
	fMultiLineTextDrawer->SetTextColor( 70, 72, 71);
	fMultiLineTextDrawer->SetFont(&font);
	height = fMultiLineTextDrawer->DrawString(frame, fIssue->body.Trim().String());
	fHeight += height + 10;
}

void 
IssueListItem::Update(BView *view, const BFont *font)
{
	if (fPreviousHeight != fHeight) {
		fPreviousHeight = fHeight;
		SetHeight(fHeight);
	}
}
