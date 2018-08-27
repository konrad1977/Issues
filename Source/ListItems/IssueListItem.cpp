/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "IssueListItem.h"
#include "GithubIssue.h"
#include "Constants.h"
#include "ListColorManager.h"
#include <interface/ListView.h>
#include <interface/Screen.h>
#include <posix/stdio.h>

IssueListItem::IssueListItem(GithubIssue issue, bool isReplicant)
	:BListItem()
	,fMultiLineTextDrawer(NULL)
	,fListColorManager(NULL)	
	,fIssue(issue)
	,fHeight(10)
	,fIsReplicant(isReplicant)
{
	fListColorManager = new ListColorManager(this, fIsReplicant);
}

IssueListItem::~IssueListItem()
{
	delete fMultiLineTextDrawer;
	delete fListColorManager;
}

GithubIssue
IssueListItem::CurrentIssue() const
{
	return fIssue;
}

void
IssueListItem::DrawBackground(BListView *parent)
{
	const int32 index = parent->IndexOf(this);
	BRect frame = parent->ItemFrame(index);

	rgb_color backgroundColor = fListColorManager->BackgroundColor();

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

	DrawBackground(parent);
	parent->SetDrawingMode(B_OP_OVER);
	DrawIssue(frame, true);
	parent->FrameResized(frame.Width(), frame.Height());
}

void
IssueListItem::DrawIssue(BRect rect, bool enableOutput)
{
	BRect frame = rect;
	BFont font(be_bold_font);
	font.SetSize(13.0);
	
	rgb_color textColor = fListColorManager->TextColor();
	fMultiLineTextDrawer->SetTextColor(textColor);

	fHeight = fMultiLineTextDrawer->DrawString(frame, fIssue.title.String(), &font, enableOutput);
	fMultiLineTextDrawer->SetTextColor(tint_color(textColor, B_DARKEN_1_TINT));

	font = be_plain_font;
	frame.OffsetBy(0, fHeight);
	
	fHeight += fMultiLineTextDrawer->DrawString(frame, fIssue.body.Trim().String(), &font, enableOutput);
	fHeight += 10;
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
		DrawIssue(view->Bounds(), false);
		SetHeight(fHeight);
	}
}
