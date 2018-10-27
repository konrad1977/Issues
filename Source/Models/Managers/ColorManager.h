/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef COLORMANAGER_H
#define COLORMANAGER_H

#include <SupportDefs.h>
#include <InterfaceDefs.h>
#include <ListItem.h>

class ColorManager {
public:
	ColorManager(BListItem *listItem, bool isFeplicant);
	~ColorManager();

	bool IsDark();

	void SetTransparency(uint8 transparency);

	rgb_color BackgroundColor();
	rgb_color TextColor();

private:

	bool 		IsSelected() const;
	const uint8 Transparency() const;

	BListItem 	*fListItem;
	bool 		fIsReplicant;
	uint8 		fTransparency;
};


#endif // _H
