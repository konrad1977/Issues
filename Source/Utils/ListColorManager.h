/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef LISTCOLORMANAGER_H
#define LISTCOLORMANAGER_H

#include <SupportDefs.h>
#include <InterfaceDefs.h>
#include <ListItem.h>

class ListColorManager {
public:
	ListColorManager(BListItem *listItem, bool isFeplicant);
	~ListColorManager();

	bool IsDark();

	rgb_color BackgroundColor();
	rgb_color TextColor();
	
private:
	bool 		IsSelected() const;
	
	BListItem 	*fListItem;
	bool 		fIsReplicant;
};


#endif // _H
