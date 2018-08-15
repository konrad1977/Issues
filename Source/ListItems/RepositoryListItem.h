/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef REPOSITORYLISTITEM_H
#define REPOSITORYLISTITEM_H


#include <SupportDefs.h>
#include <interface/ListItem.h>

#include "MultiLineTextDrawer.h"

class GithubRepository;
class RepositoryListItem : public BListItem {
public:
	RepositoryListItem(GithubRepository *repository);
	~RepositoryListItem();	

	virtual void DrawItem(BView *view, BRect rect, bool complete = false);
	virtual void Update(BView *view, const BFont *font);	
	
	GithubRepository *CurrentRepository() const;
	
private:
			void DrawRepository(BRect frame);
			
	GithubRepository		*fRepository;
	MultiLineTextDrawer 	*fMultiLineTextDrawer;
	float 					fHeight;
	float					fPreviousHeight;
};

#endif // _H
