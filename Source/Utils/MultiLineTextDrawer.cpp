/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include "MultiLineTextDrawer.h"
#include <String.h>

#include <posix/stdio.h>
#include <posix/stdlib.h>
#include <string>

MultiLineTextDrawer::MultiLineTextDrawer(BView *parent)
	:fParent(parent)
	,fAlignment(B_ALIGN_LEFT)
{
	rgb_color textColor = { 0,0,0 };
	SetTextColor(textColor);
	SetInsets(BSize(10,10));
	
	fParent->SetLowColor(fParent->ViewColor());	
}

MultiLineTextDrawer::~MultiLineTextDrawer()
{
	
}

void 
MultiLineTextDrawer::Invalidate() 
{
	fParent->Invalidate();
}
	
void 
MultiLineTextDrawer::SetTextColor(rgb_color color)
{
	fTextColor = color;
	Invalidate();
}

void
MultiLineTextDrawer::SetInsets(BSize size) 
{
	if (fInsets != size) {
		fInsets = size;
		Invalidate();
	}
}

void 
MultiLineTextDrawer::SetAligntment(alignment align)
{
	if (fAlignment != align) {
		fAlignment = align;
		Invalidate();
	}
}


const char *
MultiLineTextDrawer::GetStringFromWidth(const char *input, BFont font, float width, BString &output)
{
	BString buffer(input);
	const size_t size = buffer.Length();
	uint32 charatersThatFits = CharactedFittedFor(buffer, &font, width);
	
	int32 breakAt = B_ERROR;

	if ( size > charatersThatFits && (breakAt = buffer.FindLast(" ", charatersThatFits)) != B_ERROR) {
		charatersThatFits = breakAt;
	} 
	if ((breakAt = buffer.FindFirst('\n', 0)) != B_ERROR) {
		charatersThatFits = breakAt; 
	}
	output.Remove(0, charatersThatFits).Trim();
	return buffer.Remove(charatersThatFits, size).String();
}
	
uint32 
MultiLineTextDrawer::CharactedFittedFor(BString text, BFont *font, float width) const
{
	if (text.CountChars() == 0) {
		return 0;
	}
	
	const float textWidth = font->StringWidth(text.String());	
	const float sizePerChar = textWidth / text.CountChars();	
	return int32((width - fInsets.width * 2) / sizePerChar);
}

const float 
MultiLineTextDrawer::GetFontHeight(BFont &font) const
{
	font_height fh;
	font.GetHeight(&fh);
	return fh.ascent + fh.descent + fh.leading;
}

void 
MultiLineTextDrawer::DrawString(BRect frame, const char *text)
{
	BFont font;
	fParent->GetFont(&font); 
	
	const float fontHeight = GetFontHeight(font);
	BRect textFrame = frame.InsetBySelf(fInsets.width, fInsets.height);

	fParent->SetHighColor(254, 227, 227);
	fParent->FillRect(textFrame);
	
	fParent->SetHighColor(fTextColor);
	fParent->SetDrawingMode( B_OP_OVER );	
	
	BString string(text);
	
	int32 lines = 0;
	const float linePosition = frame.LeftTop().y + fontHeight;

	switch (fAlignment) {
		case B_ALIGN_LEFT: {
			while( string.CountChars() > 0 ) {
				const char *textToRender = GetStringFromWidth(string.String(), font, textFrame.Width(), string);
				fParent->MovePenTo(textFrame.LeftTop().x, linePosition + fontHeight * lines);
				fParent->DrawString(textToRender);
				lines++;
			}
			break;			
		}

		case B_ALIGN_CENTER: {					
			while( string.CountChars() > 0 ) {
				const char *textToRender = GetStringFromWidth(string.String(), font, textFrame.Width(), string);
				const float width = font.StringWidth(textToRender);
				fParent->MovePenTo(textFrame.LeftTop().x + (frame.Width() - width) / 2.0, linePosition + fontHeight * lines);	
				fParent->DrawString(textToRender);
				lines++;
			}
			break;
		}
		
		case B_ALIGN_RIGHT: {					
			while( string.CountChars() > 0 ) {
				const char *textToRender = GetStringFromWidth(string.String(), font, textFrame.Width(), string);
				const float width = font.StringWidth(textToRender);
				fParent->MovePenTo(frame.RightTop().x - width, linePosition + fontHeight * lines);	
				fParent->DrawString(textToRender);
				lines++;
			}
			break;
		}
		default:
			break;
	}
}