/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef ISSUELISTITEM_H
#define ISSUELISTITEM_H

#include <SupportDefs.h>
#include <interface/ListItem.h>

#include "MultiLineTextDrawer.h"

class GithubIssue;
class IssueListItem : public BListItem {
public:
	IssueListItem(GithubIssue *issue);
	~IssueListItem();	

	virtual void DrawItem(BView *view, BRect rect, bool complete = false);
	virtual void Update(BView *view, const BFont *font);	
		
private:
			void DrawIssue(BRect frame);
			
	GithubIssue				*fIssue;
	MultiLineTextDrawer 	*fMultiLineTextDrawer;
	float 					fHeight;
	float					fPreviousHeight;
};

#endif // _H
