/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "IssueTitleItem.h"
#include "ListColorManager.h"
#include "MultiLineTextDrawer.h"

#include <interface/ListView.h>

#include <posix/stdlib.h>
#include <posix/string.h>

IssueTitleItem::IssueTitleItem(const char *title, bool isReplicant)
	:BListItem()
	,fTitle(NULL)
	,fDrawer(NULL)
	,fColorManager(NULL)
	,fIsReplicant(isReplicant)
	,fHeight(10)
	,fPreviousHeight(0)
{
	fTitle = strdup(title);
	fColorManager = new ListColorManager(this, isReplicant);
}

IssueTitleItem::~IssueTitleItem()
{
	free(fTitle);
	delete fColorManager;
	delete fDrawer;
}

void
IssueTitleItem::DrawBackground(BListView *parent)
{
	const int32 index = parent->IndexOf(this);
	BRect frame = parent->ItemFrame(index);

	rgb_color backgroundColor = fColorManager->BackgroundColor();

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
IssueTitleItem::DrawItem(BView *view, BRect rect, bool complete)
{
	BListView *parent = dynamic_cast<BListView *>(view);
	const int32 index = parent->IndexOf(this);
	BRect frame = parent->ItemFrame(index);

	if (fDrawer == NULL) {
		fDrawer = new MultiLineTextDrawer(parent);
		fDrawer->SetInsets(BSize(10,0));
	}

	DrawBackground(parent);
	parent->SetDrawingMode(B_OP_OVER);
	DrawTitle(frame, false);
	parent->FrameResized(frame.Width(), frame.Height());
}

void 
IssueTitleItem::DrawTitle(BRect frame, bool disableOutput)
{
	BFont font(be_bold_font);
	font.SetSize(17.0);
	
	fDrawer->SetTextColor(fColorManager->TextColor());
	fDrawer->SetFont(&font);
	fDrawer->SetAligntment(B_ALIGN_CENTER);
	
	fHeight = fDrawer->DrawString(frame, fTitle, disableOutput);
	fHeight += 10;
}

void 
IssueTitleItem::Update(BView *view, const BFont *font)
{
	if (fPreviousHeight != fHeight) {
		fPreviousHeight = fHeight;

		if (fDrawer == NULL) {
			BListView *parent = dynamic_cast<BListView *>(view);
			fDrawer = new MultiLineTextDrawer(parent);
			fDrawer->SetInsets(BSize(10,0));
		}
		
		DrawTitle(view->Bounds(), true);
		SetHeight(fHeight);
	}
}	