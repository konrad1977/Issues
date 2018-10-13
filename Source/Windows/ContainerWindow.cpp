/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "ContainerWindow.h"
#include "GithubRepository.h"
#include "Constants.h"
#include "ContainerView.h"
#include "ContainerModel.h"

#include <locale/Catalog.h>

#include <interface/MenuBar.h>
#include <interface/MenuItem.h>
#include <interface/StringItem.h>
#include <interface/GroupLayout.h>
#include <interface/LayoutBuilder.h>

#include <posix/stdio.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "ContainerWindow"

ContainerWindow::ContainerWindow(ContainerModel *container)
	:BWindow(BRect(0,0,1,1), "Container", B_TITLED_WINDOW, B_FRAME_EVENTS | B_AUTO_UPDATE_SIZE_LIMITS)
	,fContainerView(nullptr)
	,fContainer(container)
	,fMenuBar(nullptr)
{
	SetTitle(container->Name());
	SetupViews();
	CenterOnScreen();
}

ContainerWindow::~ContainerWindow()
{
	delete fContainer;
}

void
ContainerWindow::Quit()
{
	BWindow::Quit();
}

void
ContainerWindow::BuildMenu()
{
	BLayoutBuilder::Menu<>(fMenuBar = new BMenuBar(Bounds(), "Menu"))
		.AddMenu(B_TRANSLATE("File"))
			.AddItem(new BMenuItem(B_TRANSLATE("Settings"), new BMessage(Actions::SHOW_SETTINGS_WINDOW), 'P'))
			.AddSeparator()
			.AddItem(new BMenuItem(B_TRANSLATE("Close"), new BMessage(Actions::QUIT), 'W'))
		.End();
}

void
ContainerWindow::SetupViews()
{
	fContainerView = new ContainerView(fContainer);
	fContainerView->SetExplicitMinSize(BSize(380, B_SIZE_UNSET));
	fContainerView->SetExplicitMaxSize(BSize(B_SIZE_UNLIMITED, B_SIZE_UNLIMITED));

	BuildMenu();

	SetLayout(new BGroupLayout(B_VERTICAL));
	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(fMenuBar)
		.Add(fContainerView);
}

void
ContainerWindow::MessageReceived(BMessage *message)
{
	switch(message->what) {
		case Actions::SHOW_SETTINGS_WINDOW: {
			printf("Show settings\n");
			break;
		}

		case Actions::QUIT: {
			Quit();
			break;
		}
		default:
			BWindow::MessageReceived(message);
	}
}

