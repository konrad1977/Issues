/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef REPOSITORYTYPEITEM_H
#define REPOSITORYTYPEITEM_H

#include <SupportDefs.h>
#include <interface/ListItem.h>
#include <support/String.h>
#include "ColorManager.h"

enum RepositoryType {
	FORK = 0,
	PUBLIC,
	PRIVATE,
	CUSTOM
};

class BOutlineListView;
class RepositoryTypeItem : public BListItem {
public:
	RepositoryTypeItem(RepositoryType type);
	~RepositoryTypeItem();

			void SetVisibleItems(uint8 visible, uint8 total);

	virtual void DrawItem(BView *view, BRect rect, bool complete = true);
	virtual void Update(BView *view, const BFont *font);

private:
			void DrawBackground(BOutlineListView *parent, BRect frame);
			float FontHeight(const BFont *font) const;

	ColorManager		*fColorManager;
	rgb_color			fIndicatorColor;
	RepositoryType		fType;
	BString				fTitle;
	BString 			fVisability;
	float 				fHeight;
	float				fPreviousHeight;
};

#endif // _H