/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef DRAWER_H
#define DRAWER_H


#include <SupportDefs.h>
#include <InterfaceDefs.h>
#include <View.h>
#include <Font.h>

class Drawer {
public:

	Drawer(BView *parent);
	~Drawer();
	
	void DrawString(BRect frame, const char *text);
	void SetInsets(BSize insets);
	
private:

	const char *GetFirstLineFromWidth(const char *text, BFont font, float width);
	inline const char *GetStringFromWidth(const char *text, BFont font, float width);
	
	BView 			*fParent;
	BSize 			fInsets;
};


#endif // _H
