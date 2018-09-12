/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef ISSUETITLEITEM_H
#define ISSUETITLEITEM_H

#include <SupportDefs.h>
#include <interface/ListItem.h>
#include <support/String.h>

struct TitleSettings {
	BString title;
	BString subTitle;
	BString url;
};

class ColorManager;
class MultiLineTextDrawer;
class BListView;
class IssueTitleItem : public BListItem {
public:
	IssueTitleItem(const TitleSettings &titleSettings, bool isReplicant);
	~IssueTitleItem();

	virtual void DrawItem(BView *view, BRect rect, bool complete = true);
	virtual void Update(BView *view, const BFont *font);


private:
			void DrawBackground(BListView *parent);
			void DrawRepository(BListView *parent, BRect frame, bool disableOutput);

	MultiLineTextDrawer 	*fDrawer;
	ColorManager			*fColorManager;
	TitleSettings			fTitleSettings;
	float 					fHeight;
	float 					fPreviousHeight;
	bool 					fIsReplicant;
};

#endif // _H
