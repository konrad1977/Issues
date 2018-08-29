/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef ISSUETITLEITEM_H
#define ISSUETITLEITEM_H


#include <SupportDefs.h>
#include <interface/ListItem.h>
#include <interface/ListView.h>
#include "GithubRepository.h"

class MultiLineTextDrawer;
class ListColorManager;
class IssueTitleItem : public BListItem {
public:

	IssueTitleItem(GithubRepository repository, bool isreplicant);
	~IssueTitleItem();
	
	virtual void DrawItem(BView *view, BRect rect, bool complete = true);	
	virtual void Update(BView *view, const BFont *font);	
	
private:

			void DrawBackground(BListView *parent);
			void DrawTitle(BRect frame, bool disableOutput);

	GithubRepository	fRepository;
	MultiLineTextDrawer *fDrawer;
	ListColorManager 	*fColorManager;
	
	bool 				fIsReplicant;
	float				fHeight;
	float				fPreviousHeight;
};


#endif // _H
