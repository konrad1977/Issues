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
	,fCurrentRepositories(NULL)
	,fCurrentFilter(NULL)
	,fFilterView(NULL)
	,fPrivateTotal(0)
	,fPublicTotal(0)
	,fForkedTotal(0)
{
	SetupViews();

	fCurrentRepositories = new BList();
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

	while (fCurrentRepositories->CountItems()) {
		delete fCurrentRepositories->RemoveItem(int32(0));
	}

	delete fCurrentRepositories;

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
GithubRepositoryWindow::SetCurrentRepositories(BList *list)
{
	if (fRepositoryListView == NULL) {
		return;
	}

	fRepositoryListView->MakeEmpty();

	BList *publicList = MakePublicRepositories(list);
	BList *privateList = MakePrivateRepositories(list);
	BList *forkedList = MakeForkedRepositories(list);

	if (fPrivateTotal < privateList->CountItems()) {
		fPrivateTotal = privateList->CountItems();
	}

	if (fPublicTotal < publicList->CountItems()) {
		fPublicTotal = publicList->CountItems();
	}

	if (fForkedTotal < forkedList->CountItems()) {
		fForkedTotal = forkedList->CountItems();
	}

	PopuplateListView(PUBLIC, publicList, fPublicTotal);
	PopuplateListView(PRIVATE, privateList, fPrivateTotal);
	PopuplateListView(FORK, forkedList, fForkedTotal);

	//We need to delete the allocated list. Not the pointers its holding
	delete publicList;
	delete privateList;
	delete forkedList;
}

BList *
GithubRepositoryWindow::MakeFilter(BString filter)
{
	if (fCurrentRepositories == NULL) {
		return NULL;
	}

	if (fCurrentFilter) {
		fCurrentFilter->MakeEmpty();
	}

	delete fCurrentFilter;
	fCurrentFilter = new BList();

	const int32 count = fCurrentRepositories->CountItems();

	for (int32 i = 0; i<count; i++) {
		GithubRepository *repository = static_cast<GithubRepository*>(fCurrentRepositories->ItemAtFast(i));
		if (repository && repository->name.ToLower().FindFirst(filter.ToLower()) != B_ERROR) {
			fCurrentFilter->AddItem(repository);
		}
	}
	return fCurrentFilter;
}

BList *
GithubRepositoryWindow::MakePrivateRepositories(BList *repositories) const
{
	if (repositories == NULL) {
		return NULL;
	}

	BList *list = new BList();

	const int32 items = repositories->CountItems();
	for (int32 i = 0; i<items; i++) {
		GithubRepository *repository = static_cast<GithubRepository*>(repositories->ItemAt(i));
		if (repository->IsPrivate() == true) {
			list->AddItem(repository);
		}
	}
	return list;
}

BList *
GithubRepositoryWindow::MakeForkedRepositories(BList *repositories) const
{
	if (repositories == NULL) {
		return NULL;
	}

	BList *list = new BList();

	const int32 items = repositories->CountItems();
	for (int32 i = 0; i<items; i++) {
		GithubRepository *repository = static_cast<GithubRepository*>(repositories->ItemAt(i));
		if (repository->IsFork() == true) {
			list->AddItem(repository);
		}
	}
	return list;
}

BList *
GithubRepositoryWindow::MakePublicRepositories(BList *repositories) const
{
	if (repositories == NULL) {
		return NULL;
	}

	BList *list = new BList();

	const int32 items = repositories->CountItems();
	for (int32 i = 0; i<items; i++) {
		GithubRepository *repository = static_cast<GithubRepository*>(repositories->ItemAt(i));
		if (repository->IsFork() == false && repository->IsPrivate() == false) {
			list->AddItem(repository);
		}
	}
	return list;
}

void
GithubRepositoryWindow::ParseData(BMessage *message)
{
	if (message->HasMessage("GithubRepositories") == false) {
		return;
	}

	while (fCurrentRepositories->CountItems()) {
		delete fCurrentRepositories->RemoveItem(int32(0));
	}

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
			fCurrentRepositories->AddItem(repository);
		}
	}

	fCurrentRepositories->SortItems(SortRepositoriesByName);
	SetCurrentRepositories(fCurrentRepositories);
}

void
GithubRepositoryWindow::PopuplateListView(RepositoryType type, BList *list, uint8 total)
{
	if (list == NULL) {
		return;
	}

	const int32 items = list->CountItems();
	RepositoryTypeItem *superItem = new RepositoryTypeItem(type);
	superItem->SetVisibleItems(list->CountItems(), total);
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
	BString filter;
	if (message->FindString("Filter", &filter) == B_OK) {
		if (filter.Length() > 0) {
			SetCurrentRepositories(MakeFilter(filter));
		} else {
			SetCurrentRepositories(fCurrentRepositories);
		}
	} else {
		SetCurrentRepositories(fCurrentRepositories);
	}
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

