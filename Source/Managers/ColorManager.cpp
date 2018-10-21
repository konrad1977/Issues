/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "ColorManager.h"
#include <interface/Screen.h>
#include "Constants.h"

ColorManager::ColorManager(BListItem *listItem, bool isreplicant)
	:fListItem(listItem)
	,fIsReplicant(isreplicant)
	,fTransparency(127)
{

}

ColorManager::ColorManager(bool isreplicant)
	:fListItem(NULL)
	,fIsReplicant(isreplicant)
	,fTransparency(127)
{

}

ColorManager::~ColorManager()
{

}

void
ColorManager::SetTransparency(uint8 transparency)
{
	fTransparency = transparency;
}

const uint8
ColorManager::Transparency() const
{
	return fTransparency;
}

bool
ColorManager::IsSelected() const
{
	return fListItem == NULL ? false : fListItem->IsSelected();
}

bool
ColorManager::IsDark()
{
	rgb_color backgroundColor = BackgroundColor();
	return (backgroundColor.red + backgroundColor.green + backgroundColor.blue) < 128 * 3;
}

rgb_color
ColorManager::BackgroundColor()
{
	if (fIsReplicant && IsSelected() == false) {
		BScreen screen;
		rgb_color color = screen.DesktopColor();
		color.alpha = fTransparency;
		return color;
	}
	return IsSelected() ? ui_color(B_LIST_SELECTED_BACKGROUND_COLOR) : ui_color(B_LIST_BACKGROUND_COLOR);
}

rgb_color
ColorManager::TextColor()
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

