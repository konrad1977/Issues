/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "ROutlineListView.h"
#include <interface/Window.h>

ROutlineListView::ROutlineListView(const char *name)
	:BOutlineListView(name, B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL | B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE)
	,fMessenger(NULL)
{

}

ROutlineListView::~ROutlineListView()
{
	delete fMessenger;
}

void
ROutlineListView::SetTarget(BHandler *handler)
{
	delete fMessenger;
	fMessenger = new BMessenger(handler);
}

bool
ROutlineListView::SendMouseEvents(BPoint where, bool pressed)
{
	BMessage *message = NULL;
	message = Window()->CurrentMessage();
	uint32 buttons = message->FindInt32("buttons");
	int32 index = IndexOf(where);

	BMessage msg(kListViewMouseEvent);
	msg.AddInt32("index", index);
	msg.AddBool("pressed", pressed);
	msg.AddUInt32("buttons", buttons);
	msg.AddPoint("point", ConvertToScreen(where));

	fMessenger->SendMessage(&msg);

	if (buttons &B_SECONDARY_MOUSE_BUTTON) {
		return true;
	}
	return false;
}

void
ROutlineListView::MouseDown(BPoint where)
{
	if (SendMouseEvents(where, true) == false) {
		BOutlineListView::MouseDown(where);
	}
}

void
ROutlineListView::MouseUp(BPoint where)
{
	SendMouseEvents(where, false);
	BOutlineListView::MouseUp(where);
}
