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
	
	void DrawString(BRect frame, const char *text);
	void SetInsets(BSize insets);
	void SetTextColor(rgb_color color);
	void SetAligntment(alignment align);
	
	void Invalidate();
	const float GetFontHeight(BFont &font) const;
	
private:
	const char *GetStringFromWidth(const char *input, BFont font, float width, BString &output);
	uint32 CharactedFittedFor(BString text, BFont *font, float width) const;
	
	BView 			*fParent;
	BFont			*fFont;
	rgb_color		fTextColor;
	alignment 		fAlignment;
	BSize 			fInsets;
};


#endif // _H
