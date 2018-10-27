/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "ContainerWindow.h"
#include "GithubRepository.h"
#include "RepositoryManager.h"
#include "Constants.h"
#include "ContainerView.h"
#include "ContainerModel.h"
#include "SettingsWindow.h"

#include <locale/Catalog.h>

#include <interface/MenuBar.h>
#include <interface/MenuItem.h>
#include <interface/StringItem.h>
#include <interface/GroupLayout.h>
#include <interface/LayoutBuilder.h>

#include <posix/stdio.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "ContainerWindow"

ContainerWindow::ContainerWindow(ContainerModel *container, RepositoryManager *manager)
	:BWindow(BRect(0,0,1,1), "Container", B_TITLED_WINDOW, B_FRAME_EVENTS | B_AUTO_UPDATE_SIZE_LIMITS)
	,fContainerView(nullptr)
	,fContainer(container)
	,fSettingsWindow(nullptr)
	,fRepositoryManager(manager)
	,fMenuBar(nullptr)
{
	SetTitle(container->Name());
	SetupViews();
	CenterOnScreen();
}

ContainerWindow::~ContainerWindow()
{
	if (fSettingsWindow) {
		fSettingsWindow->Lock();
		fSettingsWindow->Quit();
	}
}

void
ContainerWindow::Quit()
{
	BWindow::Quit();
}

SettingsWindow*
ContainerWindow::CurrentSettingsWindow()
{
	if (fSettingsWindow == NULL) {
		fSettingsWindow = new SettingsWindow(const_cast<Repository*>(fContainer->RepositoryModel()));
		fSettingsWindow->SetTarget(this);
	}
	return fSettingsWindow;
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
	fContainerView->SetTarget(this);
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
			CurrentSettingsWindow()->Show();
			break;
		}

		case kSettingsWindowQuit: {
			fSettingsWindow = nullptr;
			break;
		}

		case RepositoryAction::Updated: {
			fRepositoryManager->SaveRepositories();
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

