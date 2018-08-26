/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "ListColorManager.h"
#include <interface/Screen.h>
#include "Constants.h"

ListColorManager::ListColorManager(BListItem *listItem, bool isreplicant)
	:fListItem(listItem)
	,fIsReplicant(isreplicant)
{

}
	
ListColorManager::~ListColorManager()
{

}

bool 		
ListColorManager::IsSelected() const
{
	return fListItem->IsSelected();
}

bool
ListColorManager::IsDark()
{
	rgb_color backgroundColor = BackgroundColor();
	return (backgroundColor.red + backgroundColor.green + backgroundColor.blue) < 128 * 3;
}

rgb_color
ListColorManager::BackgroundColor()
{
	if (fIsReplicant && IsSelected() == false) {
		BScreen screen;
		rgb_color color = screen.DesktopColor();
		color.alpha = 200;
		return color;
	}
	return IsSelected() ? ui_color(B_LIST_SELECTED_BACKGROUND_COLOR) : ui_color(B_LIST_BACKGROUND_COLOR);
}

rgb_color	
ListColorManager::TextColor()
{
	if (fIsReplicant) {
		if (IsDark()) {
			return kLightTextColor;
		} else {
			return kDarkTextColor;
		}
	}
	return ui_color(IsSelected() ? B_LIST_SELECTED_ITEM_TEXT_COLOR : B_LIST_ITEM_TEXT_COLOR);
}

