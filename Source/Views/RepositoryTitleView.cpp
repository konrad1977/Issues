/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "RepositoryTitleView.h"
#include "GithubRepository.h"
#include "ColorManager.h"

RepositoryTitleView::RepositoryTitleView(bool isReplicant)
	:BView("RepositoryTitleView", B_WILL_DRAW)
	,fRepository(NULL)
	,fDrawer(NULL)
	,fColorManager(NULL)
	,fHeight(10)
	,fIsReplicant(isReplicant)
{	
	fDrawer = new MultiLineTextDrawer(this);
	fDrawer->SetInsets(BSize(10,0));

	if (isReplicant) {
		SetViewColor(B_TRANSPARENT_COLOR);
	} else {
		SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	}
	fColorManager = new ColorManager(fIsReplicant);
}
	
RepositoryTitleView::~RepositoryTitleView()
{

}

BSize
RepositoryTitleView::MinSize() 
{
	return BSize(B_SIZE_UNSET, fHeight);
}

BSize 
RepositoryTitleView::MaxSize()
{
	return BSize(B_SIZE_UNLIMITED, fHeight);
}

void
RepositoryTitleView::SetRepository(GithubRepository *repository)
{
	fRepository = repository;
	Invalidate();
}
	
void
RepositoryTitleView::Draw(BRect rect)
{
	BRect r = Bounds();

	if (fIsReplicant) {
		rgb_color backgroundColor = fColorManager->BackgroundColor();
		SetHighColor(backgroundColor);
		SetDrawingMode(B_OP_ALPHA);
		FillRoundRect(r.InsetBySelf(2,2), 3, 3);
	}
	
	if (fRepository == NULL) {
		return;
	}
	
	rgb_color textColor = fColorManager->TextColor();

	BFont font(be_bold_font);
	font.SetSize(16);
	fDrawer->SetTextColor(textColor);

	const char *title = fRepository->name.ToUpper().String();
	float strWidth = font.StringWidth(title);
	
	fHeight = fDrawer->DrawString(r, title, &font);
	r = r.OffsetBySelf(0, fHeight + 6);
	
	SetHighColor(255,64,80);
	SetPenSize(2.0f);
	SetDrawingMode(B_OP_COPY);
	StrokeLine(BPoint(r.left + 10, r.top), BPoint(r.left + strWidth + 10, r.top));

	fHeight += 16;	
	InvalidateLayout();
}