/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef REPOSITORYLISTITEM_H
#define REPOSITORYLISTITEM_H


#include <SupportDefs.h>
#include <interface/ListItem.h>
#include <interface/ListView.h>

#include "MultiLineTextDrawer.h"

class ColorManager;
class Repository;
class RepositoryListItem : public BListItem {
public:
	RepositoryListItem(Repository *repository);
	~RepositoryListItem();

	virtual void DrawItem(BView *view, BRect rect, bool complete = false);
	virtual void Update(BView *view, const BFont *font);

	Repository *CurrentRepository() const;

private:
			void DrawRepository(BRect frame, bool disableOutput);
			void DrawBackground(BListView *parent, BRect frame, bool tint);

	Repository				*fRepository;
	MultiLineTextDrawer 	*fMultiLineTextDrawer;
	ColorManager			*fListColorManager;

	float 					fHeight;
	float					fPreviousHeight;
};

#endif // _H
