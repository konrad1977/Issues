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
#include "RepositoryManager.h"
#include "Constants.h"
#include "FilterView.h"
#include "MessageFinder.h"
#include "ROutlineListView.h"

#include <locale/Catalog.h>

#include <interface/PopUpMenu.h>
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
	,fRepositoryManager(NULL)
	,fAddRepositoryWindow(NULL)
	,fRepositoryListView(NULL)
	,fDownloadThread(-1)
	,fCurrentRepositories(NULL)
	,fCurrentFilter(NULL)
	,fFilterView(NULL)
	,fListMenu(NULL)
	,fMenuItemShowIssues(NULL)
	,fMenuItemShowCommits(NULL)
	,fPrivateTotal(0)
	,fPublicTotal(0)
	,fForkedTotal(0)
	,fCustomTotal(0)
{
	SetupViews();

	fCurrentRepositories = new BList();
	fGithubClient = new GithubClient(this);
	fRepositoryManager = new RepositoryManager(this);
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
		delete reinterpret_cast<GithubRepository*>(fCurrentRepositories->RemoveItem(int32(0)));
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

	fRepositoryListView = new ROutlineListView("Repositories");
	fRepositoryListView->SetInvocationMessage(new BMessage(kListInvokedMessage));
	fRepositoryListView->SetSelectionMessage(new BMessage(kListSelectionChanged));
	fRepositoryListView->SetTarget(this);

	BScrollView *scrollView = new BScrollView("Scrollview", fRepositoryListView, B_FOLLOW_ALL, 0, false, true);

	BLayoutBuilder::Menu<>(fMenuBar = new BMenuBar(Bounds(), "Menu"))
		.AddMenu(B_TRANSLATE("File"))
			.AddItem(new BMenuItem(B_TRANSLATE("Add.."), new BMessage(kShowAddRepository), 'A'))
			.AddSeparator()
			.AddItem(new BMenuItem(B_TRANSLATE("About"), NULL, 'R'))
		.End()
		.AddMenu(B_TRANSLATE("Edit"))
			.AddItem(fMenuItemShowIssues = new BMenuItem(B_TRANSLATE("Show issues"), new BMessage(kShowIssueForRepository), 'I'))
			.AddItem(fMenuItemShowCommits = new BMenuItem(B_TRANSLATE("Show commits"), new BMessage(kShowCommitsForRepository), 'C'))
		.End();

	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.SetExplicitMinSize(BSize(380, 480))
		.Add(fMenuBar)
		.Add(fFilterView)
		.Add(scrollView);

	fMenuItemShowIssues->SetEnabled(false);
	fMenuItemShowCommits->SetEnabled(false);
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
	BList *customList = fRepositoryManager->Repositories();

	if (fPrivateTotal < privateList->CountItems()) {
		fPrivateTotal = privateList->CountItems();
	}

	if (fPublicTotal < publicList->CountItems()) {
		fPublicTotal = publicList->CountItems();
	}

	if (fForkedTotal < forkedList->CountItems()) {
		fForkedTotal = forkedList->CountItems();
	}

	if (customList) {
		if (fCustomTotal < customList->CountItems()) {
			fCustomTotal = customList->CountItems();
		}
	}

	PopuplateListView(CUSTOM, customList, fCustomTotal);
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
GithubRepositoryWindow::PopuplateListView(RepositoryType type, BList *list, uint8 total)
{
	if (list == NULL || list->CountItems() == 0) {
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
GithubRepositoryWindow::HandleAddRepository(BMessage *message)
{
	BString name;
	if (message->FindString("name", &name) != B_OK) {
		return;
	}

	BString owner;
	if (message->FindString("owner", &owner) != B_OK) {
		return;
	}

	fGithubClient->RequestRepository(name.String(), owner.String());
}

void
GithubRepositoryWindow::HandleMouseDownEvents(BMessage *message)
{
	int32 index;
	bool pressed;
	uint32 buttons;
	BPoint point;

	message->FindInt32("index", &index);
	message->FindBool("pressed", &pressed);
	message->FindUInt32("buttons", &buttons);
	message->FindPoint("point", &point);

	if (pressed && buttons &B_SECONDARY_MOUSE_BUTTON) {
		RepositoryListItem *listItem = dynamic_cast<RepositoryListItem*>(fRepositoryListView->ItemAt(index));

		if (listItem == NULL) {
			return;
		}

		GithubRepository *repository = listItem->CurrentRepository();
		if (repository != NULL) {

			BMessage msg(kMenuShowIssueForRepository);
			msg.AddInt32("index", index);

			if (fListMenu == NULL) {
				fListMenu = new BPopUpMenu("menu");
				fListMenu->AddItem(new BMenuItem("Issues", &msg));
				fListMenu->AddItem(new BMenuItem("Commits", NULL));
			}

			fListMenu->Go(point, true);
			fListMenu->SetTargetForItems(this);
		}
	}
}

void
GithubRepositoryWindow::MessageReceived(BMessage *message) {
	switch (message->what) {

		case kListViewMouseEvent: {
			HandleMouseDownEvents(message);
			break;
		}
		case kRepositoryManagerAdd: {
			printf("kRepositoryManagerAdd\n");
			SetCurrentRepositories(fCurrentRepositories);
			fAddRepositoryWindow = NULL;
			break;
		}

		case kRepositoryManagerExists: {
			if (fAddRepositoryWindow) {
				fAddRepositoryWindow->Lock();
				fAddRepositoryWindow->SetEnabled(true);
				fAddRepositoryWindow->Unlock();
			}
			printf("kRepositoryManagerExists\n");
			break;
		}

		case kRepositoryManagerRemove: {
			printf("kRepositoryManagerRemove\n");
			break;
		}

		case kRepositoryManagerLoaded: {
			printf("kRepositoryManagerLoaded\n");
			break;
		}

		case kRepositoryAdded: {
			HandleAddRepository(message);
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
		case kShowIssueForRepository: {
			ShowIssuesWithIndex(fCurrentSelectedIndex);
			break;
		}

		case kListSelectionChanged: {
			int32 index = B_ERROR;

			fMenuItemShowIssues->SetEnabled(false);
			fMenuItemShowCommits->SetEnabled(false);

			fCurrentSelectedIndex = -1;
			if (message->FindInt32("index", &index) != B_OK) {
				return;
			}

			fCurrentSelectedIndex = index;

			if (dynamic_cast<RepositoryListItem*>(fRepositoryListView->ItemAt(index))) {
				fMenuItemShowIssues->SetEnabled(true);
				fMenuItemShowCommits->SetEnabled(true);
			}
			break;
		}

		case kMenuShowIssueForRepository:
		case kListInvokedMessage: {
			int32 index = B_ERROR;
			if (message->FindInt32("index", &index) != B_OK) {
				return;
			}
			ShowIssuesWithIndex(index);
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

void
GithubRepositoryWindow::ShowIssuesWithIndex(int32 index)
{
	RepositoryListItem *listItem = dynamic_cast<RepositoryListItem*>(fRepositoryListView->ItemAt(index));
	if (listItem == NULL || listItem->CurrentRepository() == NULL) {
		return;
	}

	GithubIssuesWindow *window = new GithubIssuesWindow(listItem->CurrentRepository());
	window->Show();
}

void
GithubRepositoryWindow::HandleUserRepositories(BMessage *message)
{
	while (fCurrentRepositories->CountItems()) {
		delete reinterpret_cast<GithubRepository*>(fCurrentRepositories->RemoveItem(int32(0)));
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
GithubRepositoryWindow::HandleRepository(BMessage *message)
{
	MessageFinder messageFinder;
	BMessage msg = messageFinder.FindMessage("repository", *message);
	GithubRepository *repository = new GithubRepository(msg);
	fRepositoryManager->AddRepository(repository);
}

void
GithubRepositoryWindow::ParseData(BMessage *message)
{
	if (message->HasMessage("UserRepositories")) {
		HandleUserRepositories(message);
	} else if (message->HasMessage("Repository")) {
		HandleRepository(message);
	}
}

