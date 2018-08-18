/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "IssueListItem.h"
#include "GithubIssue.h"
#include "Constants.h"
#include <interface/ListView.h>
#include <interface/Screen.h>
#include <posix/stdio.h>

IssueListItem::IssueListItem(GithubIssue *issue, bool isReplicant)
	:BListItem()
	,fIssue(issue)
	,fMultiLineTextDrawer(NULL)
	,fHeight(30)
	,fIsReplicant(isReplicant)
{

}
	
IssueListItem::~IssueListItem()
{
	delete fMultiLineTextDrawer;
}

GithubIssue*
IssueListItem::CurrentIssue() const 
{
	return fIssue;
}

void 
IssueListItem::DrawBackground(BListView *parent)
{
	const int32 index = parent->IndexOf(this);
	BRect frame = parent->ItemFrame(index);
	
	rgb_color backgroundColor = BackgroundColor(IsSelected());
	backgroundColor.alpha = 100;
	
	if (fIsReplicant || index % 2 == 0) {
		parent->SetHighColor(backgroundColor);
	} else {
		parent->SetHighColor(tint_color(backgroundColor, 1.05));
	}	
	
	parent->SetDrawingMode(fIsReplicant ? B_OP_ALPHA: B_OP_COPY);	
	if (fIsReplicant) {
		parent->FillRoundRect(frame.InsetBySelf(2,2), 3, 3);
	} else {
		parent->FillRect(frame);
	}
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
	
	rgb_color backgroundColor = BackgroundColor(IsSelected());
	
	parent->SetDrawingMode(B_OP_OVER);	
	DrawIssue(frame, true);
	DrawBackground(parent);	
	DrawIssue(frame, false);
	parent->FrameResized(frame.Width(), frame.Height());		
}

void 
IssueListItem::DrawIssue(BRect rect, bool disableOutput)
{
	BRect frame = rect;
	BFont font(be_bold_font);
	font.SetSize(12.0);
	fMultiLineTextDrawer->SetTextColor(TextColor(IsSelected()));

	fMultiLineTextDrawer->SetFont(&font);
	float height = fMultiLineTextDrawer->DrawString(frame, fIssue->title.String(), disableOutput);
	fHeight = height;

	font = be_plain_font;
	font.SetSize(12);
	frame = frame.OffsetBySelf(0, height);
	fMultiLineTextDrawer->SetTextColor( TextColor(IsSelected()));
	fMultiLineTextDrawer->SetFont(&font);
	height = fMultiLineTextDrawer->DrawString(frame, fIssue->body.Trim().String(), disableOutput);
	fHeight += height + 10;
	SetHeight(fHeight);
}

rgb_color
IssueListItem::BackgroundColor(bool isSelected)
{
	if (fIsReplicant && isSelected == false) {
		BScreen screen;		
		rgb_color color = screen.DesktopColor();
		color.alpha = 127;
		return color;	
	}
	return isSelected ? ui_color(B_LIST_SELECTED_BACKGROUND_COLOR) : ui_color(B_LIST_BACKGROUND_COLOR);
}

rgb_color
IssueListItem::TextColor(bool isSelected)
{
	if (fIsReplicant) {
		if (IsDark()) {
			return kLightTextColor;
		} else {
			return kDarkTextColor;
		}		
	}
	return ui_color( isSelected ? B_LIST_SELECTED_ITEM_TEXT_COLOR : B_LIST_ITEM_TEXT_COLOR);
}

bool 
IssueListItem::IsDark() 
{
	rgb_color backgroundColor = BackgroundColor(false);
	return backgroundColor.red < 127 || backgroundColor.green < 127 ||  backgroundColor.blue < 127;
}

void 
IssueListItem::Update(BView *view, const BFont *font)
{
	if (fPreviousHeight != fHeight) {
		fPreviousHeight = fHeight;
		SetHeight(fHeight);
	}
}
