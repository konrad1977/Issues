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
	DrawIssue(frame, false);
	parent->FrameResized(frame.Width(), frame.Height());
}

void
IssueListItem::DrawIssue(BRect rect, bool disableOutput)
{
	BRect frame = rect;
	BFont font(be_bold_font);
	font.SetSize(12.0);
	
	fMultiLineTextDrawer->SetTextColor(fListColorManager->TextColor());
	fMultiLineTextDrawer->SetFont(&font);

	fHeight = fMultiLineTextDrawer->DrawString(frame, fIssue.title.String(), disableOutput);

	font = be_plain_font;
	frame.OffsetBy(0, fHeight);

	if (disableOutput == false) {
		fMultiLineTextDrawer->SetFont(&font);
	}

	fHeight += fMultiLineTextDrawer->DrawString(frame, fIssue.body.Trim().String(), disableOutput);
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
		DrawIssue(view->Bounds(), true);
		SetHeight(fHeight);
	}
}
