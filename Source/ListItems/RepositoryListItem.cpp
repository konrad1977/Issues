/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "RepositoryListItem.h"
#include "GithubRepository.h"

#include <interface/ListView.h>
#include <posix/stdio.h>

RepositoryListItem::RepositoryListItem(GithubRepository *repository)
	:BListItem()
	,fRepository(repository)
	,fMultiLineTextDrawer(NULL)
	,fHeight(30)
{

}
	
RepositoryListItem::~RepositoryListItem()
{
	delete fMultiLineTextDrawer;
}

void 
RepositoryListItem::DrawItem(BView *view, BRect rect, bool complete)
{
	BListView *parent = dynamic_cast<BListView *>(view);
	const int32 index = parent->IndexOf(this);
	BRect frame = parent->ItemFrame(index);
	
	if (fMultiLineTextDrawer == NULL) {
		fMultiLineTextDrawer = new MultiLineTextDrawer(parent);
	//	fDrawer->SetTransparency(255);
		fMultiLineTextDrawer->SetInsets(BSize(10,0));
	}
	
	rgb_color backgroundColor = ui_color(B_LIST_BACKGROUND_COLOR);
	
	if (IsSelected()) {
		parent->SetHighColor(ui_color(B_LIST_SELECTED_BACKGROUND_COLOR));
	} else if (index % 2 == 0) {
		parent->SetHighColor(backgroundColor);
	} else {
		parent->SetHighColor(tint_color(backgroundColor, 1.02));
	}
	
	parent->SetDrawingMode(B_OP_COPY);
	parent->FillRect(frame);
	parent->SetDrawingMode(B_OP_OVER);
	
	DrawRepository(frame);
	Update(parent, NULL);
}

void 
RepositoryListItem::DrawRepository(BRect rect)
{
	BRect frame = rect;
	fHeight = fMultiLineTextDrawer->DrawString(frame, fRepository->name.String());

	frame = frame.OffsetBySelf(0, fHeight);
	fHeight += fMultiLineTextDrawer->DrawString(frame, fRepository->description.Trim().String());
	fHeight += 100;
}

/*void 
RepositoryListItem::DrawPortfolioInfo(BRect rect)
{
	BRect frame = rect.InsetBySelf(0,5);
	frame.bottom = frame.top + frame.Height() / 2;
	
	BFont font(be_plain_font);
	font.SetSize(11);
	rgb_color titleColor = fDrawer->TextColor(IsSelected());
	rgb_color textColor = tint_color(titleColor, 0.7);
	DrawItemSettings settings = { frame, &font, &textColor, B_ALIGN_LEFT };

	BString stocks;
	stocks << "Stocks: " << fPortfolio->CurrentSymbols()->CountItems();
	DrawText(stocks.String(), settings);	
	
	font = be_bold_font;
	font.SetSize(14);	
	frame = frame.OffsetBySelf(0, frame.Height());
	settings = { frame, &font, &titleColor, B_ALIGN_LEFT };
	DrawText(fPortfolio->Name().String(), settings);
}
*/
void 
RepositoryListItem::Update(BView *view, const BFont *font)
{
	SetHeight(fHeight);	
}