/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "Drawer.h"
#include <String.h>

#include <posix/stdio.h>
#include <posix/stdlib.h>
#include <posix/string.h>


Drawer::Drawer(BView *parent)
	:fParent(parent)
{
	SetInsets(BSize(2,2));
}

Drawer::~Drawer()
{
	
}

void
Drawer::SetInsets(BSize size) 
{
	fInsets = size;
}

const char *
Drawer::GetFirstLineFromWidth(const char *text, BFont font, float width)
{
	BString buffer(text);
	int32 position = buffer.Length();
	int32 originLength = position;
	
	while (width < font.StringWidth(buffer.String())) {
		position = buffer.Length();
		buffer = buffer.Remove(position -1, 1);
	}
	return BString(text).Remove(position, originLength);
}

inline
const char *
Drawer::GetStringFromWidth(const char *text, BFont font, float width)
{
	BString buffer(text);
	
	int32 position = buffer.Length();
	int32 originalLength = position;
	
	while (width < font.StringWidth(buffer.String())) {
		position = buffer.Length();
		buffer = buffer.Remove(position -1, 1);
	}
	return BString(text).Remove(0, position);
}

void 
Drawer::DrawString(BRect frame, const char *text)
{
	BFont font(be_plain_font);
	fParent->SetFont(&font);
	
	font_height fh;
	font.GetHeight(&fh);
	
	const float fontHeight = fh.ascent + fh.descent + fh.leading;
	const float horizontalCenter = ((frame.Height() - fontHeight) / 2) + fh.descent;
	//const float width = font.StringWidth(text);	

	fParent->MovePenTo( fInsets.width, frame.LeftBottom().y - horizontalCenter);	
	fParent->SetHighColor(0,0,0);
	fParent->SetDrawingMode( B_OP_COPY );
	fParent->SetLowColor(fParent->ViewColor());	

	const float textWidth = frame.Width() - fInsets.width;
	
	BString string(text);
	fParent->DrawString(GetFirstLineFromWidth(text, font, textWidth));
	
	int32 lines = 1;
	const float linePosition = frame.LeftBottom().y - horizontalCenter;
	
	while( string.Length() > 0 ) {
		fParent->MovePenTo( fInsets.width, linePosition + fontHeight * lines);
		string = GetStringFromWidth(string.String(), font, textWidth);
		fParent->DrawString(string.String());
		lines++;
	}

	//printf("%s\n", GetStringFromWidth(text, font, frame.Width() - fInsets.width));	

}