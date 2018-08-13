/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "Drawer.h"
#include <String.h>

#include <posix/stdio.h>
#include <posix/stdlib.h>
#include <string>

Drawer::Drawer(BView *parent)
	:fParent(parent)
{
	SetInsets(BSize(2,2));
	BFont font(be_plain_font);
	fParent->SetFont(&font);
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
Drawer::GetStringFromWidth(const char *input, BFont font, float width, BString &output)
{
	BString buffer(input);
	size_t position = buffer.CountChars();
	int32 fittableSize = CharactedFittedFor(buffer, &font, width);
	if (fittableSize > position) {
		output.RemoveChars(0, position);
		return buffer.String();
	}
	
	output.RemoveChars(0, fittableSize);
	return buffer.RemoveChars(fittableSize, position).String();
}
	
int32 
Drawer::CharactedFittedFor(BString text, BFont *font, float width) const
{
	const float textWidth = font->StringWidth(text.String());
	const float sizePerChar = textWidth / text.CountChars();
	return int32(width / sizePerChar);
}

void 
Drawer::DrawString(BRect frame, const char *text)
{
	BFont font(be_plain_font);
	font_height fh;
	font.GetHeight(&fh);
		
	const float fontHeight = fh.ascent + fh.descent + fh.leading;
	const float textWidth = frame.Width() - fInsets.width;

	fParent->MovePenTo( fInsets.width, frame.LeftTop().y + fontHeight);	
	fParent->SetLowColor(fParent->ViewColor());	
	fParent->SetHighColor(0,0,0);
	fParent->SetDrawingMode( B_OP_COPY );	
	
	BString string(text);
	
	int32 lines = 1;
	const float linePosition = frame.LeftTop().y + fontHeight;
	
	while( string.CountChars() > 0 ) {
		fParent->DrawString(GetStringFromWidth(string.String(), font, textWidth, string));
		fParent->MovePenTo( fInsets.width, linePosition + fontHeight * lines);
		lines++;
	}  
}