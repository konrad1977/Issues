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
	ColorManager(bool isFeplicant);
	~ColorManager();

	bool IsDark();

	rgb_color BackgroundColor();
	rgb_color TextColor();
	
private:
	bool 		IsSelected() const;
	
	BListItem 	*fListItem;
	bool 		fIsReplicant;
};


#endif // _H
