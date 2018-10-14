/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "RepositoryWindow.h"
#include "GithubRepository.h"
#include "GithubTokenWindow.h"
#include "ContainerWindow.h"
#include "IssueModel.h"
#include "CommitModel.h"
#include "GithubClient.h"
#include "AddRepositoryWindow.h"
#include "SettingsManager.h"
#include "RepositoryListItem.h"
#include "RepositoryManager.h"
#include "Constants.h"
#include "FilterView.h"
#include "MessageFinder.h"
#include "ROutlineListView.h"

#include <app/Application.h>

#include <locale/Catalog.h>

#include <interface/PopUpMenu.h>
#include <interface/MenuBar.h>
#include <interface/MenuItem.h>
#include <interface/StringItem.h>
#include <interface/GroupLayout.h>
#include <interface/LayoutBuilder.h>
#include <interface/OutlineListView.h>
#include <interface/ScrollView.h>
#include <interface/GroupView.h>

#include <posix/stdio.h>


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "RepositoryWindow"

RepositoryWindow::RepositoryWindow()
	:BWindow(BRect(30,30, 1, 1), "Repositories", B_DOCUMENT_WINDOW, B_FRAME_EVENTS | B_QUIT_ON_WINDOW_CLOSE | B_AUTO_UPDATE_SIZE_LIMITS)
	,fGithubTokenWindow(nullptr)
	,fGithubClient(nullptr)
	,fRepositoryManager(nullptr)
	,fAddRepositoryWindow(nullptr)
	,fRepositoryListView(nullptr)
	,fDownloadThread(-1)
	,fCurrentRepositories(nullptr)
	,fCurrentFilter(nullptr)
	,fFilterView(nullptr)
	,fListMenu(nullptr)
	,fPopupIssueItem(nullptr)
	,fPopupCommitItem(nullptr)
	,fMenuItemShowIssues(nullptr)
	,fMenuItemShowCommits(nullptr)
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

RepositoryWindow::~RepositoryWindow()
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
RepositoryWindow::DownloadRepositories(void *cookie)
{
	RepositoryWindow *window = static_cast<RepositoryWindow*>(cookie);
	window->fGithubClient->RequestProjects();
	return 0;
}

void
RepositoryWindow::SpawnDownloadThread()
{
	fDownloadThread = spawn_thread(&DownloadRepositories, "Download Data", B_NORMAL_PRIORITY, this);
	if (fDownloadThread >= 0)
		resume_thread(fDownloadThread);
}

void
RepositoryWindow::SetupViews()
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
			.AddItem(new BMenuItem(B_TRANSLATE("Add Repository" B_UTF8_ELLIPSIS), new BMessage(MenuAction::AddRepository), 'R'))
			.AddItem(new BMenuItem(B_TRANSLATE("About" B_UTF8_ELLIPSIS), new BMessage(MenuAction::About), 'A'))
			.AddSeparator()
			.AddItem(new BMenuItem(B_TRANSLATE("Quit" B_UTF8_ELLIPSIS), new BMessage(MenuAction::Quit), 'A'))

		.End()
		.AddMenu(B_TRANSLATE("Edit"))
			.AddItem(fMenuItemShowIssues = new BMenuItem(B_TRANSLATE("Show issues"), new BMessage(MenuAction::Issues), 'I'))
			.AddItem(fMenuItemShowCommits = new BMenuItem(B_TRANSLATE("Show commits"), new BMessage(MenuAction::Commits), 'C'))
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
RepositoryWindow::SetCurrentRepositories(BList *list)
{
	if (fRepositoryListView == nullptr) {
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
RepositoryWindow::MakeFilter(BString filter)
{
	if (fCurrentRepositories == nullptr) {
		return nullptr;
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
RepositoryWindow::MakePrivateRepositories(BList *repositories) const
{
	if (repositories == nullptr) {
		return nullptr;
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
RepositoryWindow::MakeForkedRepositories(BList *repositories) const
{
	if (repositories == nullptr) {
		return nullptr;
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
RepositoryWindow::MakePublicRepositories(BList *repositories) const
{
	if (repositories == nullptr) {
		return nullptr;
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
RepositoryWindow::PopuplateListView(RepositoryType type, BList *list, uint8 total)
{
	if (list == nullptr || list->CountItems() == 0) {
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
RepositoryWindow::SortRepositoriesByName(const void *first, const void *second)
{
	GithubRepository *firstRep = *(GithubRepository**)first;
	GithubRepository *secondRep = *(GithubRepository**)second;
	return strcasecmp(secondRep->name.String(), firstRep->name.String());
}

int
RepositoryWindow::SortRepositoriesByType(const void *first, const void *second)
{
	GithubRepository *firstRep = *(GithubRepository**)first;
	GithubRepository *secondRep = *(GithubRepository**)second;

	if (firstRep->SortOrder() == secondRep->SortOrder()) {
		return 0;
	}
	return firstRep->SortOrder() - secondRep->SortOrder();
}

void
RepositoryWindow::HandleFilterMessage(BMessage *message)
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
RepositoryWindow::HandleAddRepository(BMessage *message)
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
RepositoryWindow::HandleMouseDownEvents(BMessage *message)
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

		if (listItem == nullptr) {
			return;
		}

		GithubRepository *repository = listItem->CurrentRepository();
		if (repository == nullptr) {
			return;
		}

		BMessage issueMsg(MenuAction::IssuesPopUp);
		issueMsg.AddInt32("index", index);

		BMessage commitsMsg(MenuAction::CommitsPopUp);
		commitsMsg.AddInt32("index", index);

		if (fListMenu == nullptr) {
			fListMenu = new BPopUpMenu("menu", false, false);
			fListMenu->AddItem(fPopupIssueItem = new BMenuItem("Issues", &issueMsg));
			fListMenu->AddItem(fPopupCommitItem = new BMenuItem("Commits", &commitsMsg));
			fListMenu->SetTargetForItems(this);
		} else {
			fPopupIssueItem->SetMessage(&issueMsg);
			fPopupCommitItem->SetMessage(&commitsMsg);
		}

		fListMenu->Go(point, true);
	}
}

void
RepositoryWindow::MessageReceived(BMessage *message) {
	switch (message->what) {

		case MenuAction::Quit: {
			be_app->PostMessage(B_QUIT_REQUESTED);
			break;
		}

		case kListViewMouseEvent: {
			HandleMouseDownEvents(message);
			break;
		}
		case RepositoryManager::Action::Added: {
			SetCurrentRepositories(fCurrentRepositories);
			fAddRepositoryWindow = nullptr;
			break;
		}

		case RepositoryManager::Action::Exists: {
			if (fAddRepositoryWindow) {
				fAddRepositoryWindow->Lock();
				fAddRepositoryWindow->SetEnabled(true);
				fAddRepositoryWindow->Unlock();
			}
			break;
		}

		case RepositoryManager::Action::Removed: {
			printf("kRepositoryManagerRemove\n");
			break;
		}

		case RepositoryManager::Action::Loaded: {
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

		case MenuAction::AddRepository: {
			fAddRepositoryWindow = new AddRepositoryWindow();
			fAddRepositoryWindow->SetTarget(this);
			fAddRepositoryWindow->Show();
			break;
		}

		case kQuitAddRepository: {
			fAddRepositoryWindow = nullptr;
			break;
		}

		case kDataReceivedMessage: {
 			ParseData(message);
			break;
		}

		case kWindowQuitMessage: {
			fGithubTokenWindow = nullptr;
			break;
		}

		case MenuAction::Issues: {
			ShowIssuesWindowFromIndex(fCurrentSelectedIndex);
			break;
		}

		case MenuAction::Commits: {
			ShowCommitsWindowFromIndex(fCurrentSelectedIndex);
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

		case MenuAction::IssuesPopUp:
		case kListInvokedMessage: {
			int32 index = B_ERROR;
			if (message->FindInt32("index", &index) != B_OK) {
				return;
			}
			ShowIssuesWindowFromIndex(index);
			break;
		}

		case MenuAction::CommitsPopUp: {
			int32 index = B_ERROR;
			if (message->FindInt32("index", &index) != B_OK) {
				return;
			}
			ShowCommitsWindowFromIndex(index);
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
RepositoryWindow::ShowIssuesWindowFromIndex(int32 index)
{
	RepositoryListItem *listItem = dynamic_cast<RepositoryListItem*>(fRepositoryListView->ItemAt(index));
	if (listItem == nullptr || listItem->CurrentRepository() == nullptr) {
		return;
	}

	GithubRepository *repository = listItem->CurrentRepository();
	IssueModel *model = new IssueModel(repository->name, repository->owner);
	ContainerWindow *window = new ContainerWindow(model);
	window->Show();
}

void
RepositoryWindow::ShowCommitsWindowFromIndex(int32 index)
{
	RepositoryListItem *listItem = dynamic_cast<RepositoryListItem*>(fRepositoryListView->ItemAt(index));
	if (listItem == nullptr || listItem->CurrentRepository() == nullptr) {
		return;
	}

	GithubRepository *repository = listItem->CurrentRepository();
	CommitModel *model = new CommitModel(repository->name, repository->owner);
	ContainerWindow *window = new ContainerWindow(model);
	window->Show();
}

void
RepositoryWindow::HandleUserRepositories(BMessage *message)
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
RepositoryWindow::HandleRepository(BMessage *message)
{
	MessageFinder messageFinder;
	BMessage msg = messageFinder.FindMessage("repository", *message);
	GithubRepository *repository = new GithubRepository(msg);
	fRepositoryManager->AddRepository(repository);
}

void
RepositoryWindow::ParseData(BMessage *message)
{
	if (message->HasMessage("UserRepositories")) {
		HandleUserRepositories(message);
	} else if (message->HasMessage("Repository")) {
		HandleRepository(message);
	}
}

