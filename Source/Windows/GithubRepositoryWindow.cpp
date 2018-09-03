/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "GithubRepositoryWindow.h"
#include "GithubRepository.h"
#include "GithubTokenWindow.h"
#include "GithubIssuesWindow.h"
#include "GithubClient.h"
#include "AddRepositoryWindow.h"
#include "SettingsManager.h"
#include "RepositoryListItem.h"
#include "Constants.h"
#include "FilterView.h"
#include "MessageFinder.h"

#include <locale/Catalog.h>

#include <interface/MenuBar.h>
#include <interface/MenuItem.h>
#include <interface/StringItem.h>
#include <interface/GroupLayout.h>
#include <interface/LayoutBuilder.h>
#include <interface/OutlineListView.h>
#include <interface/ScrollView.h>
#include <GroupView.h>
#include <posix/stdio.h>


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "GithubRepositoryWindow"

GithubRepositoryWindow::GithubRepositoryWindow()
	:BWindow(BRect(30,30, 1, 1), "Repositories", B_DOCUMENT_WINDOW, B_FRAME_EVENTS | B_QUIT_ON_WINDOW_CLOSE | B_AUTO_UPDATE_SIZE_LIMITS)
	,fGithubTokenWindow(NULL)
	,fGithubClient(NULL)
	,fAddRepositoryWindow(NULL)
	,fRepositoryListView(NULL)
	,fDownloadThread(-1)
	,fPrivateRepositories(NULL)
	,fForkedRepositories(NULL)
	,fPublicRepositories(NULL)
	,fFilterView(NULL)
{
	SetupViews();
	fPrivateRepositories = new BList();
	fForkedRepositories = new BList();
	fPublicRepositories = new BList();
	fGithubClient = new GithubClient(this);

	CenterOnScreen();
}

GithubRepositoryWindow::~GithubRepositoryWindow()
{
	if (fGithubTokenWindow) {
		fGithubTokenWindow->Lock();
		fGithubTokenWindow->Quit();
	}

	if (fAddRepositoryWindow) {
		fAddRepositoryWindow->Lock();
		fAddRepositoryWindow->Quit();
	}

	ClearRepositories();

	delete fPrivateRepositories;
	delete fForkedRepositories;
	delete fPublicRepositories;

	delete fAddRepositoryWindow;
	delete fGithubTokenWindow;
	delete fGithubClient;
}

int32
GithubRepositoryWindow::DownloadRepositories(void *cookie)
{
	GithubRepositoryWindow *window = static_cast<GithubRepositoryWindow*>(cookie);
	window->fGithubClient->RequestProjects();
	return 0;
}

void
GithubRepositoryWindow::SpawnDownloadThread()
{
	fDownloadThread = spawn_thread(&DownloadRepositories, "Download Data", B_NORMAL_PRIORITY, this);
	if (fDownloadThread >= 0)
		resume_thread(fDownloadThread);
}

void
GithubRepositoryWindow::SetupViews()
{
	fFilterView = new FilterView();
	fFilterView->SetTarget(this);
	
	fRepositoryListView = new BOutlineListView("Repositories", B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL | B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE);
	BScrollView *scrollView = new BScrollView("Scrollview", fRepositoryListView, B_FOLLOW_ALL, 0, false, true);
	fRepositoryListView->SetInvocationMessage(new BMessage(kListInvokedMessage));

	BLayoutBuilder::Menu<>(fMenuBar = new BMenuBar(Bounds(), "Menu"))
		.AddMenu(B_TRANSLATE("Edit"))
			.AddItem(new BMenuItem(B_TRANSLATE("Add.."), new BMessage(kShowAddRepository), 'A'))
			.AddSeparator()
			.AddItem(new BMenuItem(B_TRANSLATE("About"), NULL, 'R'))
		.End();

	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.SetExplicitMinSize(BSize(380, 480))
		.Add(fMenuBar)
		.Add(fFilterView)
		.Add(scrollView);
}

void
GithubRepositoryWindow::ParseData(BMessage *message)
{
	if (message->HasMessage("GithubRepositories") == false) {
		return;
	}

	ClearRepositories();
	BList *list = new BList();

	MessageFinder messageFinder;
	BMessage msg = messageFinder.FindMessage("nodes", *message);

	BMessage repositoriesMessage;
	char *name;
	uint32 type;
	int32 count;

	for (int32 i = 0; msg.GetInfo(B_MESSAGE_TYPE, i, &name, &type, &count) == B_OK; i++) {
		BMessage nodeMsg;
		if (msg.FindMessage(name, &nodeMsg) == B_OK) {
			GithubRepository *repository = new GithubRepository(nodeMsg);
			list->AddItem(repository);
		}
	}

	list->SortItems(SortRepositoriesByName);
	BuildRepositoryList(list);
	delete list;

	PopuplateListView(PUBLIC, fPublicRepositories);
	PopuplateListView(PRIVATE, fPrivateRepositories);
	PopuplateListView(FORK, fForkedRepositories);
}

void
GithubRepositoryWindow::PopuplateListView(RepositoryType type, BList *list)
{
	if (list == NULL) {
		return;
	}

	const int32 items = list->CountItems();
	RepositoryTypeItem *superItem = new RepositoryTypeItem(type);
	superItem->SetVisibleItems(list->CountItems(), list->CountItems());
	fRepositoryListView->AddItem(superItem);

	for( int32 index = 0; index < items; index++) {
		GithubRepository *repo = static_cast<GithubRepository*>(list->ItemAtFast(index));
		RepositoryListItem *repoItem = new RepositoryListItem(repo);
		fRepositoryListView->AddUnder(repoItem, superItem);
	}
}

int
GithubRepositoryWindow::SortRepositoriesByName(const void *first, const void *second)
{
	GithubRepository *firstRep = *(GithubRepository**)first;
	GithubRepository *secondRep = *(GithubRepository**)second;
	return strcasecmp(secondRep->name.String(), firstRep->name.String());
}

void
GithubRepositoryWindow::BuildRepositoryList(BList *repositories)
{
	if (repositories == NULL) {
		return;
	}

	const int32 items = repositories->CountItems();
	for (int32 i = 0; i<items; i++) {
		GithubRepository *repository = static_cast<GithubRepository*>(repositories->ItemAt(i));
		if (repository->IsFork()) {
			fForkedRepositories->AddItem(repository);
		} else if (repository->IsPrivate()) {
			fPrivateRepositories->AddItem(repository);
		} else {
			fPublicRepositories->AddItem(repository);
		}
	}
}

void
GithubRepositoryWindow::ClearRepositories() {

	while (fPrivateRepositories->CountItems()) {
		delete fPrivateRepositories->RemoveItem(int32(0));
	}

	while (fForkedRepositories->CountItems()) {
		delete fForkedRepositories->RemoveItem(int32(0));
	}

	while (fPublicRepositories->CountItems()) {
		delete fPublicRepositories->RemoveItem(int32(0));
	}
}

int
GithubRepositoryWindow::SortRepositoriesByType(const void *first, const void *second)
{
	GithubRepository *firstRep = *(GithubRepository**)first;
	GithubRepository *secondRep = *(GithubRepository**)second;

	if (firstRep->SortOrder() == secondRep->SortOrder()) {
		return 0;
	}
	return firstRep->SortOrder() - secondRep->SortOrder();
}

void 
GithubRepositoryWindow::HandleFilterMessage(BMessage *message)
{
	message->PrintToStream();
}

void
GithubRepositoryWindow::MessageReceived(BMessage *message) {
	switch (message->what) {
		case kRepositoryAdded: {
			message->PrintToStream();
			break;
		}
		case kFilterChangedMessage: {
			HandleFilterMessage(message);
			break;
		}
		case kTokenLoadedMessage: {
			printf("Token loaded\n");
			SpawnDownloadThread();
			break;
		}

		case kNoTokenMessage: {
			fGithubTokenWindow = new GithubTokenWindow(this);
			fGithubTokenWindow->Show();
			break;
		}

		case kShowAddRepository: {
			fAddRepositoryWindow = new AddRepositoryWindow();
			fAddRepositoryWindow->SetTarget(this);
			fAddRepositoryWindow->Show();

			printf("Show add repository\n");
			break;
		}

		case kQuitAddRepository: {
			fAddRepositoryWindow = NULL;
			break;
		}

		case kDataReceivedMessage: {
 			ParseData(message);
			break;
		}
		case kWindowQuitMessage: {
			fGithubTokenWindow = NULL;
			break;
		}

		case kListInvokedMessage: {

			int32 index = B_ERROR;
			if (message->FindInt32("index", &index) != B_OK) {
				return;
			}

			RepositoryListItem *listItem = dynamic_cast<RepositoryListItem*>(fRepositoryListView->ItemAt(index));
			if (listItem == NULL || listItem->CurrentRepository() == NULL) {
				return;
			}

			GithubIssuesWindow *window = new GithubIssuesWindow(listItem->CurrentRepository());
			window->Show();
			break;
		}

		case kGithubTokenSaveMessage: {
			BString token;
			if (message->FindString("Token", &token) == B_OK) {
				fGithubClient->SaveToken(token.String());
			}
			break;
		}
		default:
			BWindow::MessageReceived(message);
	}
}

