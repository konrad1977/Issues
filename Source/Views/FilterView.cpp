/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "FilterView.h"
#include <interface/TextControl.h>

#include "Constants.h"
#include <LayoutBuilder.h>
#include <GridView.h>
#include <GridLayoutBuilder.h>
#include <GroupLayoutBuilder.h>

FilterView::FilterView()
	:BView("Filter", B_WILL_DRAW)
	,fFilterControl(NULL)
	,fMessenger(NULL)
{
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
}

FilterView::~FilterView()
{
	delete fMessenger;
}

void
FilterView::SetTarget(BHandler *handler)
{
	delete fMessenger;
	fMessenger = new BMessenger(handler);
}

void
FilterView::AttachedToWindow()
{
	LayoutSubview();
	BView::AttachedToWindow();
}

void 
FilterView::MessageReceived(BMessage *message)
{
	switch (message->what) {
		case kFilterChangedMessage: {
			if (fFilterControl == NULL || fFilterControl->Text() == NULL || fMessenger == NULL) {
				return;
			}
			
			BMessage message(kFilterChangedMessage);
			message.AddString("Filter", fFilterControl->Text());
			fMessenger->SendMessage(&message);
			break;
		}
		default:
			BView::MessageReceived(message);
			break;
	}
}

void
FilterView::LayoutSubview()
{
	fFilterControl = new BTextControl("Filter:", "", NULL);
	fFilterControl->SetModificationMessage(new BMessage(kFilterChangedMessage));
	fFilterControl->SetTarget(this);
	
	SetExplicitMinSize(BSize(B_SIZE_UNSET, 40));
	
	BLayoutBuilder::Group<>(this, B_HORIZONTAL, 0)
		.SetInsets(10,10,10,10)
		.Add(fFilterControl)
	.End();
}
