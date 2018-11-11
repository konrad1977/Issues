/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "CListItem.h"
#include "GithubIssue.h"
#include "Constants.h"
#include "FileDownloader.h"

#include "ColorManager.h"
#include <Bitmap.h>
#include <TranslationUtils.h>

#include <interface/ListView.h>
#include <interface/Screen.h>
#include <posix/stdio.h>

CListItem::CListItem(CListModel model, bool isReplicant)
	:BListItem()
	,fMultiLineTextDrawer(nullptr)
	,fListColorManager(nullptr)
	,fModel(model)
	,fHeight(10.0f)
	,fPreviousHeight(0.0f)
	,fIsReplicant(isReplicant)
	,fIcon(nullptr)
{
	fListColorManager = new ColorManager(this, fIsReplicant);
}

CListItem::~CListItem()
{
	delete fMultiLineTextDrawer;
	delete fListColorManager;
}

CListModel
CListItem::CurrentModel() const
{
	return fModel;
}

void
CListItem::DrawBackground(BListView *parent, BRect frame, bool tint)
{
	rgb_color backgroundColor = fListColorManager->BackgroundColor();
	
	if (fIsReplicant || tint == false) {
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
CListItem::DrawItem(BView *view, BRect rect, bool complete)
{
	BListView *parent = dynamic_cast<BListView *>(view);
	const int32 index = parent->IndexOf(this);
	BRect frame = parent->ItemFrame(index);

	if (fMultiLineTextDrawer == nullptr) {
		fMultiLineTextDrawer = new MultiLineTextDrawer(parent);
		fMultiLineTextDrawer->SetInsets(BSize(10,5));
	}

	bool tint = index % 2 == 1;
	DrawBackground(parent, frame, tint);
	parent->SetDrawingMode(B_OP_OVER);
	DrawIssue(frame, true);
	parent->FrameResized(frame.Width(), frame.Height());

	DrawIcon(parent, frame);
}

void
CListItem::DrawIcon(BListView *parent, BRect rect)
{
	BRect r(rect);
	const float SIZE = 24;
	r.left = r.right - SIZE;
	r.bottom = r.top + SIZE;

	if (fIcon) {
		parent->SetDrawingMode(B_OP_ALPHA);
		parent->DrawBitmap(fIcon, fIcon->Bounds(), r.OffsetBySelf(-3.0f, 3.0f), B_FILTER_BITMAP_BILINEAR);
		parent->SetDrawingMode(B_OP_COPY);
		return;
	}

	const char *url = fModel.AuthorUrl().String();

	if (url == nullptr) {
		return;
	}

	BMallocIO buffer;
	FileDownloader downloader(fModel.AuthorUrl());

	if (downloader.Download(&buffer) != B_OK) {
		return;
	}

	fIcon = BTranslationUtils::GetBitmap(&buffer);
	if (fIcon == nullptr) {
		return;
	}

	parent->SetDrawingMode(B_OP_ALPHA);
	parent->DrawBitmap(fIcon, fIcon->Bounds(), r.OffsetBySelf(-3.0f, 3.0f), B_FILTER_BITMAP_BILINEAR);
	parent->SetDrawingMode(B_OP_COPY);
}

void
CListItem::DrawIssue(BRect rect, bool enableOutput)
{
	BRect frame = rect;
	BFont font(be_bold_font);
	font.SetSize(13.0);

	rgb_color textColor = fListColorManager->TextColor();
	fMultiLineTextDrawer->SetTextColor(textColor);

	const char *author = fModel.Author().String();
	float authorWidth = font.StringWidth(author);
	BRect titleFrame = frame;
	titleFrame.right -= authorWidth;
	float titleHeight = fMultiLineTextDrawer->DrawString(titleFrame, fModel.Title().String(), &font, enableOutput);

	font = be_plain_font;
	font.SetSize(10);

	fMultiLineTextDrawer->SetTextColor(tint_color(textColor, B_DARKEN_1_TINT));
	fMultiLineTextDrawer->SetAlignment(B_ALIGN_RIGHT);
	BRect authorFrame = frame;
	float authorHeight = fMultiLineTextDrawer->DrawString(authorFrame.OffsetBySelf(-26.0f, 0), author, &font, enableOutput);
	fMultiLineTextDrawer->SetTextColor(tint_color(textColor, B_DARKEN_1_TINT));

	fHeight = MAX(titleHeight, authorHeight);

	font = be_plain_font;
	font.SetSize(12);
	frame.OffsetBy(0, fHeight);

	fMultiLineTextDrawer->SetAlignment(B_ALIGN_LEFT);
	fHeight += fMultiLineTextDrawer->DrawString(frame, fModel.Body().String(), &font, enableOutput);
	fHeight += 20;
}

void
CListItem::SetTransparency(uint8 transparency)
{
	if (fListColorManager == nullptr) {
		return;
	}
	fListColorManager->SetTransparency(transparency);
}

void
CListItem::Update(BView *view, const BFont *font)
{
	if (fPreviousHeight != fHeight) {
		fPreviousHeight = fHeight;

		if (fMultiLineTextDrawer == nullptr) {
			BListView *parent = dynamic_cast<BListView *>(view);
			fMultiLineTextDrawer = new MultiLineTextDrawer(parent);
			fMultiLineTextDrawer->SetInsets(BSize(10,5));
		}
		DrawIssue(view->Bounds(), false);
		SetHeight(fPreviousHeight);
		printf("Update height: %f\n", fPreviousHeight);
	}
}
