/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "ImageView.h"
#include <Bitmap.h>
#include "FileDownloader.h"
#include <TranslationUtils.h>
#include <stdio.h>

ImageView::ImageView(BRect rect, const BString &url)
	:BView(rect, "ImageView", B_FOLLOW_ALL_SIDES, B_WILL_DRAW)
	,fUrl(url)
	,fBitmap(NULL)
{

}

ImageView::~ImageView()
{

}

void
ImageView::Draw(BRect rect)
{
	if (fBitmap == NULL) {
		return;
	}

	BRect r(Bounds());
	SetDrawingMode(B_OP_ALPHA);
	DrawBitmap(fBitmap, fBitmap->Bounds(), r, B_FILTER_BITMAP_BILINEAR);
	SetDrawingMode(B_OP_COPY);

	BView::Draw(r);
}

void
ImageView::AttachedToWindow()
{
	BMallocIO buffer;
	FileDownloader downloader(fUrl);

	if (downloader.Download(&buffer) != B_OK) {
		return;
	}

	fBitmap = BTranslationUtils::GetBitmap(&buffer);
	if (fBitmap == nullptr) {
		return;
	}

	Invalidate();
	BView::AttachedToWindow();
}
