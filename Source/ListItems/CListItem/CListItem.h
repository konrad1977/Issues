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
class BBitmap;
class CListItem : public BListItem {
public:
	CListItem(const CListModel *model, bool isReplicant);
	~CListItem();

	virtual void DrawItem(BView *view, BRect rect, bool complete = true);
	virtual void Update(BView *view, const BFont *font);
			void SetTransparency(uint8 value);
	const CListModel* CurrentModel() const;

private:
			void DrawBackground(BListView *parent, BRect frame, bool tint);
			void DrawIssue(BRect frame, bool disableOutput);
			void DrawIcon(BListView *parent, BRect rect);
			int32 IconSize(BListView* parent) const;

	MultiLineTextDrawer 	*fMultiLineTextDrawer;
	ColorManager			*fListColorManager;
	const CListModel		*fModel;
	float 					fHeight;
	float					fPreviousHeight;
	bool					fIsReplicant;
	bool					fSecondPass;
	BBitmap					*fIcon;
};

#endif // _H
