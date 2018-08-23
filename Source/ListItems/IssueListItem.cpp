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
	,fHeight(10)
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

	if (fIsReplicant || index % 2 == 0) {
		parent->SetHighColor(backgroundColor);
	} else {
		parent->SetHighColor(tint_color(backgroundColor, 1.05));
	}

	if (fIsReplicant) {
		parent->SetDrawingMode(B_OP_ALPHA);
		parent->FillRoundRect(frame.InsetBySelf(2,2), 3, 3);
	} else {
		parent->SetDrawingMode(B_OP_COPY);
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

	parent->SetDrawingMode(B_OP_OVER);
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

	fHeight = fMultiLineTextDrawer->DrawString(frame, fIssue->title.String(), disableOutput);

	font = be_plain_font;
	frame = frame.OffsetBySelf(0, fHeight);
	fMultiLineTextDrawer->SetTextColor( TextColor(IsSelected()));

	if (disableOutput == false) {
		fMultiLineTextDrawer->SetFont(&font);
	}

	fHeight += fMultiLineTextDrawer->DrawString(frame, fIssue->body.Trim().String(), disableOutput);
	fHeight += 10;
}

rgb_color
IssueListItem::BackgroundColor(bool isSelected)
{
	if (fIsReplicant && isSelected == false) {
		BScreen screen;
		rgb_color color = screen.DesktopColor();
		color.alpha = 200;
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
	return (backgroundColor.red + backgroundColor.green + backgroundColor.blue) < 128 * 3;
}

void
IssueListItem::Update(BView *view, const BFont *font)
{
	if (fPreviousHeight != fHeight) {
		fPreviousHeight = fHeight;

		if (fMultiLineTextDrawer == NULL) {
			BListView *parent = dynamic_cast<BListView *>(view);
			fMultiLineTextDrawer = new MultiLineTextDrawer(parent);
			fMultiLineTextDrawer->SetInsets(BSize(10,0));
		}
		DrawIssue(view->Bounds(), true);
		SetHeight(fHeight);
	}
}
