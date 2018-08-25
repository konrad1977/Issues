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
	SetInsets(BSize(0,0));
	fParent->SetLowColor(fParent->ViewColor());
}

MultiLineTextDrawer::~MultiLineTextDrawer()
{

}

void
MultiLineTextDrawer::SetTextColor(uchar red, uchar green, uchar blue, uchar alpha)
{
	rgb_color color = { red, green, blue, alpha };
	SetTextColor(color);
}

void
MultiLineTextDrawer::SetTextColor(rgb_color color)
{
	if (fTextColor != color) {
		fTextColor = color;
	}
}

void
MultiLineTextDrawer::SetFont(BFont *font)
{
	if (fFont != font) {
		fParent->SetFont(font);
	}
}

void
MultiLineTextDrawer::SetInsets(BSize size)
{
	if (fInsets != size) {
		fInsets = size;
	}
}

const BSize 
MultiLineTextDrawer::Insets() const
{
	return fInsets;
}

void
MultiLineTextDrawer::SetAligntment(alignment align)
{
	if (fAlignment != align) {
		fAlignment = align;
	}
}

const int32
MultiLineTextDrawer::FindLineBreak(BString &text, uint32 offset) const {

	int32 index = B_ERROR;
	if ((index = text.FindLast(" ", offset)) != B_ERROR) {
		return index;
	}
	else if ((index = text.FindLast("-", offset)) != B_ERROR) {
		return index+1;
	}
	else if ((index = text.FindLast("/", offset)) != B_ERROR) {
		return index+1;
	}
	return B_ERROR;
}

const char *
MultiLineTextDrawer::GetStringFromWidth(const char *input, BFont *font, float width, BString &output)
{
	BString buffer(input);
	
	const size_t size = buffer.Length();
	uint32 charatersThatFits = CharactedFittedFor(buffer, font, width);

	int32 breakAt = B_ERROR;

	if ( (size > charatersThatFits) && (breakAt = FindLineBreak(buffer, charatersThatFits)) != B_ERROR) {
		charatersThatFits = breakAt;
	}
	if ((breakAt = buffer.FindFirst('\n', 0)) != B_ERROR) {
		charatersThatFits = breakAt;
	}
	output.Remove(0, charatersThatFits).Trim();
	return buffer.Remove(charatersThatFits, size).String();
}

const uint32
MultiLineTextDrawer::CharactedFittedFor(BString text, BFont *font, float width) const
{
	if (text.CountChars() == 0) {
		return 0;
	}

	const float textWidth = font->StringWidth(text.String());
	const float sizePerChar = textWidth / text.CountChars();
	return int32(width / sizePerChar);
}

const float
MultiLineTextDrawer::GetFontHeight(BFont &font) const
{
	font_height fh;
	font.GetHeight(&fh);
	return ceilf(fh.ascent + fh.descent + fh.leading);
}

float
MultiLineTextDrawer::DrawString(BRect frame, const char *text, bool disableOutput)
{
	BFont font;
	fParent->GetFont(&font);

	const float fontHeight = GetFontHeight(font);
	BRect textFrame = frame.InsetBySelf(fInsets.width, fInsets.height);
	BString string(text);

	int32 lines = 0;
	const float linePosition = frame.LeftTop().y + fontHeight;

	if (disableOutput == false) {
		fParent->SetHighColor(fTextColor);
		fParent->SetDrawingMode( B_OP_OVER );
	}

	switch (fAlignment) {
		case B_ALIGN_LEFT: {
			while( string.CountChars() > 0 ) {
				const char *textToRender = GetStringFromWidth(string.String(), &font, textFrame.Width(), string);
				if (disableOutput == false) {
					fParent->MovePenTo(textFrame.LeftTop().x, linePosition + fontHeight * lines);
					fParent->DrawString(textToRender);
				}
				lines++;
			}
			break;
		}

		case B_ALIGN_CENTER: {
			while( string.CountChars() > 0 ) {
				const char *textToRender = GetStringFromWidth(string.String(), &font, textFrame.Width(), string);
				const float width = font.StringWidth(textToRender);
				if (disableOutput == false) {
					fParent->MovePenTo(textFrame.LeftTop().x + (frame.Width() - width) / 2.0, linePosition + fontHeight * lines);
					fParent->DrawString(textToRender);
				}
				lines++;
			}
			break;
		}

		case B_ALIGN_RIGHT: {
			while( string.CountChars() > 0 ) {
				const char *textToRender = GetStringFromWidth(string.String(), &font, textFrame.Width(), string);
				const float width = font.StringWidth(textToRender);
				if (disableOutput == false) {
					fParent->MovePenTo(frame.RightTop().x - width, linePosition + fontHeight * lines);
					fParent->DrawString(textToRender);
				}
				lines++;
			}
			break;
		}
		default:
			break;
	}
	return fontHeight * lines;
}
