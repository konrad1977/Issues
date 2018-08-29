/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "RepositoryTitleView.h"
#include "GithubRepository.h"

RepositoryTitleView::RepositoryTitleView()
	:BView("RepositoryTitleView", B_WILL_DRAW)
	,fRepository(NULL)
{

}
	
RepositoryTitleView::~RepositoryTitleView()
{

}

BSize	
RepositoryTitleView::MinSize()
{
	return BSize(B_SIZE_UNSET, 70);
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
	SetHighColor(255,0,0);
	FillRect(r);
}