/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef CLISTITEM_H
#define CLISTITEM_H

#include <SupportDefs.h>
#include <interface/ListItem.h>
#include "GithubIssue.h"
#include "MultiLineTextDrawer.h"
#include "ColorManager.h"
#include "CListModel.h"

class BListView;
class CListItem : public BListItem {
public:
	CListItem(CListModel model, bool isReplicant);
	~CListItem();	
	
	virtual void DrawItem(BView *view, BRect rect, bool complete = true);	
	virtual void Update(BView *view, const BFont *font);	
			
	CListModel CurrentModel() const;
	
private:
			void DrawBackground(BListView *parent);
			void DrawIssue(BRect frame, bool disableOutput);
			
	MultiLineTextDrawer 	*fMultiLineTextDrawer;
	ColorManager			*fListColorManager;
	CListModel				fModel;	
	float 					fHeight;
	float					fPreviousHeight;
	bool					fIsReplicant;
	bool					fSecondPass;
};

#endif // _H
