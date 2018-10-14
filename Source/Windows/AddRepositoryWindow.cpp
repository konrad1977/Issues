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
	,fMessenger(nullptr)
	,fRepositoryName(nullptr)
	,fRepositoryOwner(nullptr)
	,fAddButton(nullptr)
{
	SetupViews();
	CenterOnScreen();
}

AddRepositoryWindow::~AddRepositoryWindow()
{
	delete fMessenger;
}

void
AddRepositoryWindow::Quit()
{
	BMessage message(kQuitAddRepository);
	if (fMessenger && fMessenger->IsValid()) {
		fMessenger->SendMessage(&message);
	}
	BWindow::Quit();
}

void
AddRepositoryWindow::MessageReceived(BMessage *message)
{
	switch (message->what) {
		case kRepositoryAdded: {
			BMessage msg(kRepositoryAdded);
			msg.AddString("name", fRepositoryName->Text());
			msg.AddString("owner", fRepositoryOwner->Text());
			fMessenger->SendMessage(&msg);
			fAddButton->SetEnabled(false);
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
AddRepositoryWindow::SetEnabled(bool value)
{
	if (fAddButton) {
		fAddButton->SetEnabled(value);
	}
}

void
AddRepositoryWindow::SetupViews()
{
	BGroupLayout *group = new BGroupLayout(B_VERTICAL);
	SetLayout(group);

	fAddButton = new BButton("AddRepository", "Add repository", new BMessage(kRepositoryAdded));

	fRepositoryName = new BTextControl("Repository name", B_TRANSLATE("Repository name:"), "", nullptr);
	fRepositoryOwner = new BTextControl("Repository owner", B_TRANSLATE("Repository owner:"), "", nullptr);

	BLayoutBuilder::Group<>(this, B_HORIZONTAL)
		.SetExplicitMinSize(BSize(400, 80))
		.SetInsets(10,10,10,10)
		.AddGroup(B_VERTICAL)
			.Add(fRepositoryName)
			.Add(fRepositoryOwner)
		.End()
		.AddGroup(B_VERTICAL)
			.AddGlue()
			.Add(fAddButton)
		.End();
}