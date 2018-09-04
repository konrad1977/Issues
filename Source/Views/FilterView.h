/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef FILTERVIEW_H
#define FILTERVIEW_H


#include <SupportDefs.h>
#include <interface/View.h>

class BMessenger;
class BTextControl;
class FilterView : public BView {
public:
	FilterView();
	~FilterView();
	
	virtual void MessageReceived(BMessage *message);
	virtual void AttachedToWindow();
			void SetTarget(BHandler *handler);
	
private:
			void LayoutSubview();
	BTextControl 	*fFilterControl;
	BMessenger		*fMessenger;
};


#endif // _H
