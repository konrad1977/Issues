/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "GithubRepositoryWindow.h"
#include "GithubRepository.h"
#include "GithubTokenWindow.h"
#include "GithubClient.h"
#include "SettingsManager.h"
#include "RepositoryListItem.h"
#include "Constants.h"
#include "MessageFinder.h"

#include <locale/Catalog.h>

#include <interface/MenuBar.h>
#include <interface/MenuItem.h>
#include <interface/StringItem.h>
#include <interface/GroupLayout.h>
#include <interface/LayoutBuilder.h>
#include <interface/ListView.h>
#include <interface/ScrollView.h>
#include <posix/stdio.h>


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "GithubRepositoryWindow"

GithubRepositoryWindow::GithubRepositoryWindow() 
	:BWindow(BRect(30,30, 300, 400), "Repositories", B_DOCUMENT_WINDOW, B_QUIT_ON_WINDOW_CLOSE)
	,fGithubTokenWindow(NULL)
	,fGithubClient(NULL)
	,fRepositoryListView(NULL)
{
	SetupViews();
	CheckForSavedToken();
}

GithubRepositoryWindow::~GithubRepositoryWindow() 
{
	delete fGithubClient;
}

void 
GithubRepositoryWindow::RequestRepositories()
{
	fGithubClient->RequestProjects();
}
			
void 
GithubRepositoryWindow::RequestIssuesForRepository(BString name)
{
	fGithubClient->RequestIssuesForRepository(name);
}

void
GithubRepositoryWindow::SetupViews() 
{
	fRepositoryListView = new BListView("Repositories", B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL | B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE);
	BScrollView *scrollView = new BScrollView("Scrollview", fRepositoryListView, B_FOLLOW_ALL, 0, false, true);
	fRepositoryListView->SetInvocationMessage(new BMessage(kRepositoryListInvokedMessage));
	
	BGroupLayout *layout = new BGroupLayout(B_VERTICAL);
	layout->SetSpacing(0);
	SetLayout(layout);
	
	BLayoutBuilder::Menu<>(fMenuBar = new BMenuBar(Bounds(), "Menu"))
		.AddMenu(B_TRANSLATE("Edit"))
			.AddItem(new BMenuItem(B_TRANSLATE("About"), NULL, 'R'))
		.End();
	
	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(fMenuBar)
		.Add(scrollView);
}

void 
GithubRepositoryWindow::CheckForSavedToken()
{
	BString token;
	LoadToken(token);
	
	if (token.Length() == 0) {
		fGithubTokenWindow = new GithubTokenWindow(this);
		fGithubTokenWindow->Show();
	} else {
		fGithubClient = new GithubClient(token.String(), this);
		fGithubClient->RequestProjects();
	}
}

void 
GithubRepositoryWindow::LoadToken(BString &token)
{
	BMessage message;
	SettingsManager manager;
	manager.LoadSettings(message);
	message.FindString("Token", &token);
}

void 
GithubRepositoryWindow::SaveToken(BMessage *message)
{
	BString token;
	if (message->FindString("Token", &token) == B_OK) {
		SettingsManager manager;
		manager.SaveSettings(*message);
	}
}

void 
GithubRepositoryWindow::ParseData(BMessage *message)
{	
	if (message->HasMessage("GithubRepositories") == false) {
		return;
	}
	
	MessageFinder messageFinder;
	BMessage msg = messageFinder.FindMessage("nodes", *message);

	fRepositoryListView->MakeEmpty();
	
	BMessage repositoriesMessage;
	char *name;
	uint32 type;
	int32 count;
				
	for (int32 i = 0; msg.GetInfo(B_MESSAGE_TYPE, i, &name, &type, &count) == B_OK; i++) {
		BMessage nodeMsg;
		if (msg.FindMessage(name, &nodeMsg) == B_OK) {
			GithubRepository *repository = new GithubRepository(nodeMsg);
			RepositoryListItem *listItem = new RepositoryListItem(repository);
			fRepositoryListView->AddItem( listItem );
		}
	}
}

void
GithubRepositoryWindow::MessageReceived(BMessage *message) {
	switch (message->what) {
		case kDataReceivedMessage: {
			printf("kDataReceivedMessage\n");
			ParseData(message);
			break;
		}
		case kWindowQuitMessage: {
			fGithubTokenWindow = NULL;
			break;
		}
		
		case kRepositoryListInvokedMessage: {
			int32 index = B_ERROR;
			if (message->FindInt32("index", &index) == B_OK) {
				RepositoryListItem *listItem = dynamic_cast<RepositoryListItem*>(fRepositoryListView->ItemAt(index));
				if (listItem && listItem->CurrentRepository()) {
					RequestIssuesForRepository(listItem->CurrentRepository()->name);
				}
				printf("Found item\n");
			}
			break;	
		}
		
		case kGithubTokenSaveMessage: {
			SaveToken(message);
			
			BString token;
			LoadToken(token);
			
			delete fGithubClient;
			fGithubClient = new GithubClient(token.String(), this);
			RequestRepositories();
			break;
		}
		default:
			BWindow::MessageReceived(message);
	}
}

