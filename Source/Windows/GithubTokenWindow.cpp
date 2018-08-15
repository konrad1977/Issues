/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "GithubTokenWindow.h"
#include "Constants.h"

#include <TextControl.h>
#include <Button.h>

#include <Catalog.h>
#include <GroupLayout.h>
#include <GridLayout.h>
#include <LayoutBuilder.h>
#include <GridLayoutBuilder.h>
#include <GroupLayoutBuilder.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "GithubTokenWindow"

GithubTokenWindow::GithubTokenWindow(BHandler *handler) 
	:BWindow(BRect(40,40, 200, 200), B_TRANSLATE("GithubToken"), B_FLOATING_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, B_AUTO_UPDATE_SIZE_LIMITS)
	,fMessenger(NULL)	
	,fTokenControl(NULL)
{
	fMessenger = new BMessenger(handler);

	InitLayout();
	CenterOnScreen();
}

GithubTokenWindow::~GithubTokenWindow()
{
	delete fMessenger;
}

void 
GithubTokenWindow::MessageReceived(BMessage *message)
{
	switch (message->what) {
		case kGithubTokenSaveMessage: {
			BMessage message(kGithubTokenSaveMessage);
			BString name(TextControl()->Text());
			message.AddString("Token", name);
			fMessenger->SendMessage(&message);
			QuitRequested();
			Quit();
			break;
		}
		default:
			BWindow::MessageReceived(message);
	}
}

BTextControl *
GithubTokenWindow::TextControl() 
{
	if (fTokenControl == NULL) {
		fTokenControl = new BTextControl("Name", B_TRANSLATE("Personal access token:"), "", NULL);
	}
	return fTokenControl;
}

void 
GithubTokenWindow::InitLayout()
{
	BGroupLayout *group = new BGroupLayout(B_VERTICAL);
	SetLayout(group);
	
	BButton *createButton = new BButton("Save", "Save", new BMessage(kGithubTokenSaveMessage));
	createButton->SetTarget(this);
	
	BLayoutBuilder::Group<>(this, B_HORIZONTAL)
		.SetExplicitMinSize(BSize(700, 40))
		.SetInsets(10,10,10,10)
		.Add(TextControl())
		.Add(createButton);
}

bool
GithubTokenWindow::QuitRequested()
{
	BMessage message(kWindowQuitMessage);
	if (fMessenger && fMessenger->IsValid()) {
		fMessenger->SendMessage(&message);
	}
	return true;
}
