/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "RepositoryListItem.h"
#include "GithubRepository.h"
#include "ColorManager.h"

#include <interface/ListView.h>
#include <posix/stdio.h>

RepositoryListItem::RepositoryListItem(GithubRepository *repository)
	:BListItem()
	,fRepository(repository)
	,fMultiLineTextDrawer(NULL)
	,fHeight(30)
{
	fListColorManager = new ColorManager(this, false);
}
	
RepositoryListItem::~RepositoryListItem()
{
	delete fListColorManager;
	delete fMultiLineTextDrawer;
}

GithubRepository *
RepositoryListItem::CurrentRepository() const
{
	return fRepository;
}

void 
RepositoryListItem::DrawBackground(BListView *parent)
{
	const int32 index = parent->IndexOf(this);
	BRect frame = parent->ItemFrame(index);

	rgb_color backgroundColor = fListColorManager->BackgroundColor();

	if (fRepository->IsPrivate()) {
		parent->SetHighColor(255,243,244);
	} else if (fRepository->IsFork()) {
		parent->SetHighColor(255,250,234);
	} else if (index % 2 == 0) {
		parent->SetHighColor(backgroundColor);
	} else {
		parent->SetHighColor(tint_color(backgroundColor, 1.05));
	}

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
		
	DrawBackground(parent);	
	parent->SetDrawingMode(B_OP_OVER);
	DrawRepository(frame, true);
	parent->FrameResized(frame.Width(), frame.Height());
}

void 
RepositoryListItem::DrawRepository(BRect rect, bool enableOutput)
{
	BRect frame = rect;
	BFont font(be_bold_font);
	font.SetSize(14.0);
	
	rgb_color textColor = fListColorManager->TextColor();
	fMultiLineTextDrawer->SetTextColor(textColor);
	
	float height = fMultiLineTextDrawer->DrawString(frame, fRepository->name.String(), &font, enableOutput);
	fHeight = height;

	font = be_plain_font;
	font.SetSize(13);
	frame = frame.OffsetBySelf(0, height);

	fMultiLineTextDrawer->SetTextColor(tint_color(textColor, B_LIGHTEN_1_TINT));
	
	height = fMultiLineTextDrawer->DrawString(frame, fRepository->description.Trim().String(), &font, enableOutput);
	fHeight += height;
	
	frame = frame.OffsetBySelf(0, height);
	fMultiLineTextDrawer->SetTextColor(ui_color(B_LINK_TEXT_COLOR));

	height = fMultiLineTextDrawer->DrawString(frame, fRepository->url.String(), &font, enableOutput);
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
