/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "AddRepositoryWindow.h"
#include <app/Messenger.h>

#include <TextControl.h>
#include <Button.h>

#include <Catalog.h>
#include <GroupLayout.h>
#include <GridLayout.h>
#include <LayoutBuilder.h>
#include <GridLayoutBuilder.h>
#include <GroupLayoutBuilder.h>
#include "Constants.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "AddRepositoryWindow"

AddRepositoryWindow::AddRepositoryWindow()
	:BWindow(BRect(0,0,1,1), B_TRANSLATE("Add repository"), B_TITLED_WINDOW, B_FRAME_EVENTS | B_AUTO_UPDATE_SIZE_LIMITS)
	,fMessenger(NULL)
	,fRepositoryName(NULL)
	,fRepositoryOwner(NULL)
{
	SetupViews();
	CenterOnScreen();
}

AddRepositoryWindow::~AddRepositoryWindow()
{
	delete fMessenger;
}

bool
AddRepositoryWindow::QuitRequested()
{
	BMessage message(kQuitAddRepository);
	if (fMessenger && fMessenger->IsValid()) {
		fMessenger->SendMessage(&message);
	}
	return true;
}

void
AddRepositoryWindow::MessageReceived(BMessage *message)
{
	switch (message->what) {
		case kRepositoryAdded: {
			BMessage msg(kRepositoryAdded);
			msg.AddString("Name", fRepositoryName->Text());
			msg.AddString("Owner", fRepositoryOwner->Text());
			fMessenger->SendMessage(&msg);
			break;
		}
		default:
			BWindow::MessageReceived(message);
	}
}

void
AddRepositoryWindow::SetTarget(BHandler *handler)
{
	delete fMessenger;
	fMessenger = new BMessenger(handler);
}

void
AddRepositoryWindow::SetupViews()
{
	BGroupLayout *group = new BGroupLayout(B_VERTICAL);
	SetLayout(group);

	BButton *addRepositoryButton = new BButton("AddRepository", "Add repository", new BMessage(kRepositoryAdded));

	fRepositoryName = new BTextControl("Repository name", B_TRANSLATE("Repository name:"), "", NULL);
	fRepositoryOwner = new BTextControl("Repository owner", B_TRANSLATE("Repository name:"), "", NULL);

	BLayoutBuilder::Group<>(this, B_HORIZONTAL)
		.SetExplicitMinSize(BSize(400, 80))
		.SetInsets(10,10,10,10)
		.AddGroup(B_VERTICAL)
			.Add(fRepositoryName)
			.Add(fRepositoryOwner)
		.End()
		.AddGroup(B_VERTICAL)
			.AddGlue()
			.Add(addRepositoryButton)
		.End();
}