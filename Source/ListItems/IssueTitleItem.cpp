/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "IssueTitleItem.h"
#include "MultiLineTextDrawer.h"
#include "ColorManager.h"
#include <interface/ListView.h>
#include <stdlib.h>

IssueTitleItem::IssueTitleItem(const TitleSettings &titleSettings, bool isReplicant)
	:BListItem()
	,fDrawer(NULL)
	,fColorManager(NULL)
	,fTitleSettings(titleSettings)
	,fHeight(-1)
	,fPreviousHeight(0)
	,fIsReplicant(isReplicant)
{
	fColorManager = new ColorManager(fIsReplicant);
}

IssueTitleItem::~IssueTitleItem()
{
	delete fDrawer;
	delete fColorManager;
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
		parent->FillRoundRect(frame.InsetBySelf(0, 1), 3, 3);
	} else {
		parent->SetDrawingMode(B_OP_COPY);
		parent->FillRect(frame);
	}
}

void
IssueTitleItem::DrawRepository(BListView *parent, BRect frame, bool disableOutput)
{

	BRect r(frame);
	BFont font(be_plain_font);
	font.SetSize(10);

	const char *title = fTitleSettings.title.String();
	const char *subTitle = fTitleSettings.subTitle.ToUpper().String();

	fDrawer->SetTextColor(234,86,136);
	fHeight = fDrawer->DrawString(r, subTitle, &font, disableOutput);
	r = r.OffsetBySelf(0.0f, fHeight);

	font = be_bold_font;
	font.SetSize(17.0f);

	rgb_color textColor = fColorManager->TextColor();
	fDrawer->SetTextColor(textColor);
	fHeight += fDrawer->DrawString(r, title, &font, disableOutput);
	fHeight += 14;
}

void
IssueTitleItem::DrawItem(BView *view, BRect rect, bool complete)
{
	BListView *parent = dynamic_cast<BListView *>(view);
	const int32 index = parent->IndexOf(this);
	BRect frame = parent->ItemFrame(index);

	if (fDrawer == NULL) {
		fDrawer = new MultiLineTextDrawer(parent);
		fDrawer->SetInsets(BSize(10,5));
	}
	DrawBackground(parent);
	DrawRepository(parent, frame, true);
}

void
IssueTitleItem::Update(BView *view, const BFont *font)
{
	if (fPreviousHeight != fHeight) {
		fPreviousHeight = fHeight;

		BListView *parent = dynamic_cast<BListView *>(view);

		if (fDrawer == NULL && parent != NULL) {
			fDrawer = new MultiLineTextDrawer(parent);
			fDrawer->SetInsets(BSize(10,5));
			DrawRepository(parent, view->Bounds(), false);
		}
		SetHeight(fHeight);
	}
}

