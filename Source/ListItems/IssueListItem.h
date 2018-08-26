/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef ISSUELISTITEM_H
#define ISSUELISTITEM_H

#include <SupportDefs.h>
#include <interface/ListItem.h>
#include "GithubIssue.h"
#include "MultiLineTextDrawer.h"
#include "ListColorManager.h"

class BListView;
class IssueListItem : public BListItem {
public:
	IssueListItem(GithubIssue issue, bool isReplicant);
	~IssueListItem();	
	
	virtual void DrawItem(BView *view, BRect rect, bool complete = true);	
	virtual void Update(BView *view, const BFont *font);	
			
	GithubIssue CurrentIssue() const;
	
private:
			void DrawBackground(BListView *parent);
			void DrawIssue(BRect frame, bool disableOutput);
			
	MultiLineTextDrawer 	*fMultiLineTextDrawer;
	ListColorManager		*fListColorManager;
	GithubIssue				fIssue;	
	float 					fHeight;
	float					fPreviousHeight;
	bool					fIsReplicant;
	bool					fSecondPass;
};

#endif // _H
