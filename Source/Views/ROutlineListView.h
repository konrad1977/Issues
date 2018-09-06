/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef ROUTLINELISTVIEW_H
#define ROUTLINELISTVIEW_H


#include <SupportDefs.h>
#include <interface/OutlineListView.h>

const int32 kListViewMouseEvent = 'kLOm';

class BMessenger;
class ROutlineListView : public BOutlineListView {
public:
	ROutlineListView(const char *name);
	~ROutlineListView();

	void SetTarget(BHandler *handler);

	virtual void MouseDown(BPoint where);
	virtual void MouseUp(BPoint where);

private:
			bool SendMouseEvents(BPoint where, bool pressed);
	BMessenger *fMessenger;
};


#endif // _H
