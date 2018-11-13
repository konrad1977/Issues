/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "RepositoryListItem.h"
#include "Repository.h"
#include "ColorManager.h"

#include <interface/ListView.h>
#include <posix/stdio.h>

RepositoryListItem::RepositoryListItem(Repository *repository)
	:BListItem()
	,fRepository(repository)
	,fMultiLineTextDrawer(NULL)
	,fHeight(0)
	,fPreviousHeight(-1)
{
	fListColorManager = new ColorManager(this, false);
}

RepositoryListItem::~RepositoryListItem()
{
	delete fListColorManager;
	delete fMultiLineTextDrawer;
}

Repository*
RepositoryListItem::CurrentRepository() const
{
	return fRepository;
}

void
RepositoryListItem::DrawBackground(BListView *parent, BRect frame, bool tint)
{
	rgb_color color = fListColorManager->BackgroundColor();
	rgb_color backgroundColor = tint ? tint_color(color, 1.02) : color;
	
	parent->SetHighColor(backgroundColor);
	parent->SetLowColor(backgroundColor);
	parent->SetDrawingMode(B_OP_COPY);
	parent->FillRect(frame);
}

void
RepositoryListItem::DrawItem(BView *view, BRect rect, bool complete)
{
	BListView *parent = dynamic_cast<BListView *>(view);
	const int32 index = parent->IndexOf(this);
	BRect frame = parent->ItemFrame(index);

	if (fMultiLineTextDrawer == NULL) {
		fMultiLineTextDrawer = new MultiLineTextDrawer(parent);
		fMultiLineTextDrawer->SetInsets(BSize(10,0));
	}

	bool tintItem = index % 2 == 1;
	DrawBackground(parent, frame, tintItem);
	DrawRepository(frame, true);
	
	parent->FrameResized(frame.Width(), frame.Height());
}

void
RepositoryListItem::DrawRepository(BRect rect, bool enableOutput)
{
	BRect frame = rect;
	BFont font(be_bold_font);

	rgb_color textColor = fListColorManager->TextColor();
	fMultiLineTextDrawer->SetTextColor(textColor);

	float height = fMultiLineTextDrawer->DrawString(frame, fRepository->Name().String(), &font, enableOutput);
	fHeight = height;

	font = be_plain_font;
	frame = frame.OffsetBySelf(0, height);

	fMultiLineTextDrawer->SetTextColor(tint_color(textColor, B_LIGHTEN_1_TINT));

	height = fMultiLineTextDrawer->DrawString(frame, fRepository->Description().Trim().String(), &font, enableOutput);
	fHeight += height;

	frame = frame.OffsetBySelf(0, height);
	fMultiLineTextDrawer->SetTextColor(ui_color(B_LINK_TEXT_COLOR));
	height = fMultiLineTextDrawer->DrawString(frame, fRepository->Url().String(), &font, enableOutput);
	fHeight += height + 10;
}

void
RepositoryListItem::Update(BView *view, const BFont *font)
{
	if (fPreviousHeight != fHeight) {
		fPreviousHeight = fHeight;

		if (fMultiLineTextDrawer == NULL) {
			BListView *parent = dynamic_cast<BListView *>(view);
			fMultiLineTextDrawer = new MultiLineTextDrawer(parent);
			fMultiLineTextDrawer->SetInsets(BSize(10,0));
		}
		DrawRepository(view->Bounds(), false);
		SetHeight(fHeight);
	}
}
