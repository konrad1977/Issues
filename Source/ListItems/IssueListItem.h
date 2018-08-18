/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef ISSUELISTITEM_H
#define ISSUELISTITEM_H

#include <SupportDefs.h>
#include <interface/ListItem.h>

#include "MultiLineTextDrawer.h"

class BListView;
class GithubIssue;
class IssueListItem : public BListItem {
public:
	IssueListItem(GithubIssue *issue, bool isReplicant);
	~IssueListItem();	

	virtual void DrawItem(BView *view, BRect rect, bool complete = true);
	virtual void Update(BView *view, const BFont *font);	
		
private:
			bool IsDark();
		rgb_color BackgroundColor(bool isSelected);
		rgb_color TextColor(bool isSelected);
		
			void DrawBackground(BListView *parent);
			void DrawIssue(BRect frame, bool disableOutput);
			
	GithubIssue				*fIssue;
	MultiLineTextDrawer 	*fMultiLineTextDrawer;
	float 					fHeight;
	float					fPreviousHeight;
	bool					fIsReplicant;
	bool					fSecondPass;
};

#endif // _H
