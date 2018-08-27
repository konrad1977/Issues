/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef MULTILINETEXTDRAWER_H
#define MULTILINETEXTDRAWER_H


#include <SupportDefs.h>
#include <InterfaceDefs.h>
#include <View.h>
#include <Font.h>

#include <stdlib.h>
#include <String.h>

class MultiLineTextDrawer {
public:

	MultiLineTextDrawer(BView *parent);
	~MultiLineTextDrawer();
	
	float DrawString(BRect frame, const char *text, BFont *font, bool enableOutput = true);
	void SetInsets(BSize insets);
	const BSize Insets() const;

	void SetTextColor(rgb_color color);
	void SetTextColor(uchar red, uchar green, uchar blue, uchar alpha = 255);
	
	void SetAligntment(alignment align);
		
private:
	
	const int32 	FindLineBreak(BString &text, uint32 offset) const;
	const float 	GetFontHeight(BFont &font) const;	
	const char 		*GetStringFromWidth(const char *input, BFont *font, float width, BString &output);
	const uint32 	CharactedFittedFor(BString text, BFont *font, float width) const;
	
	BView 			*fParent;
	rgb_color		fTextColor;
	alignment 		fAlignment;
	BSize 			fInsets;
};


#endif // _H
