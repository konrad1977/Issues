/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "RepositoryWindow.h"
#include "GithubRepository.h"
#include "Repository.h"
#include "GithubTokenWindow.h"
#include "ContainerWindow.h"
#include "IssueModel.h"
#include "CommitModel.h"
#include "GithubClient.h"
#include "AddRepositoryWindow.h"
#include "RepositoryListItem.h"
#include "RepositoryManager.h"
#include "Constants.h"
#include "FilterView.h"
#include "MessageFinder.h"
#include "ROutlineListView.h"
#include "NetRequester.h"

#include <app/Application.h>

#include <locale/Catalog.h>

#include <interface/Alert.h>
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

	delete fAddRepositoryWindow;
	delete fGithubTokenWindow;
	delete fRepositoryManager;
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

	BList *publicList 	= new BList();
	BList *privateList 	= new BList();
	BList *forkedList 	= new BList();
	BList *customList 	= new BList();

	const int32 items = list->CountItems();
	for (int32 index = 0; index < items; index++) {
		Repository *repository = static_cast<Repository*>(list->ItemAtFast(index));
		if (repository->IsManuallyAdded() == true) {
			customList->AddItem(repository);
		} else if (repository->IsPrivate() == true) {
			privateList->AddItem(repository);
		} else if (repository->IsFork() == true) {
			forkedList->AddItem(repository);
		} else {
			publicList->AddItem(repository);
		}
	}

	if (fPrivateTotal < privateList->CountItems()) {
		fPrivateTotal = privateList->CountItems();
	}

	if (fPublicTotal < publicList->CountItems()) {
		fPublicTotal = publicList->CountItems();
	}

	if (fForkedTotal < forkedList->CountItems()) {
		fForkedTotal = forkedList->CountItems();
	}

	if (fCustomTotal < customList->CountItems()) {
		fCustomTotal = customList->CountItems();
	}

	PopuplateListView(CUSTOM, customList, fCustomTotal);
	PopuplateListView(PUBLIC, publicList, fPublicTotal);
	PopuplateListView(PRIVATE, privateList, fPrivateTotal);
	PopuplateListView(FORK, forkedList, fForkedTotal);

	//We need to delete the allocated list. Not the pointers its holding
	delete publicList;
	delete privateList;
	delete forkedList;
	delete customList;
}

BList *
RepositoryWindow::MakeFilter(BString filter)
{
	const BList *repositories = fRepositoryManager->Repositories();

	if (repositories == nullptr) {
		return nullptr;
	}

	if (fCurrentFilter) {
		fCurrentFilter->MakeEmpty();
	}

	delete fCurrentFilter;
	fCurrentFilter = new BList();

	const int32 count = repositories->CountItems();

	for (int32 i = 0; i<count; i++) {
		Repository *repository = static_cast<Repository*>(repositories->ItemAtFast(i));
		if (repository && repository->Name().ToLower().FindFirst(filter.ToLower()) != B_ERROR) {
			fCurrentFilter->AddItem(repository);
		}
	}
	return fCurrentFilter;
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
		Repository *repo = static_cast<Repository*>(list->ItemAtFast(index));
		RepositoryListItem *repoItem = new RepositoryListItem(repo);
		fRepositoryListView->AddUnder(repoItem, superItem);
	}
}

int
RepositoryWindow::SortRepositoriesByName(const void *first, const void *second)
{
	Repository *firstRep = *(Repository**)first;
	Repository *secondRep = *(Repository**)second;
	return strcasecmp(secondRep->Name().String(), firstRep->Name().String());
}

int
RepositoryWindow::SortRepositoriesByType(const void *first, const void *second)
{
	Repository *firstRep = *(Repository**)first;
	Repository *secondRep = *(Repository**)second;

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
			SetCurrentRepositories(fRepositoryManager->Repositories());
		}
	} else {
		SetCurrentRepositories(fRepositoryManager->Repositories());
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

		const Repository *repository = listItem->CurrentRepository();
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
			printf("RepositoryManager::Action::Added\n");
			SetCurrentRepositories(fRepositoryManager->Repositories());
			fAddRepositoryWindow = nullptr;
			break;
		}

		case RepositoryManager::Action::Exists: {
			if (fAddRepositoryWindow) {
				fAddRepositoryWindow->Lock();
				fAddRepositoryWindow->SetEnabled(true);
				fAddRepositoryWindow->Unlock();
			}
			ShowAlert("Repository Exists", "The repository already exists in your list");
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

		case NetRequesterAction::DataReceived: {
 			ParseData(message);
			break;
		}
		
		case NetRequesterAction::RequestFailed: {
 			ShowAlert("Network request failed", "The network request failed!");
			break;
		}
		
		case NetRequesterAction::ParseFailed: {
 			ShowAlert("Parse failed", "Failed to parse JSON data!");
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

	Repository *repository = listItem->CurrentRepository();
	IssueModel *model = new IssueModel(repository);
	ContainerWindow *window = new ContainerWindow(model, fRepositoryManager);
	window->Show();
}

void
RepositoryWindow::ShowCommitsWindowFromIndex(int32 index)
{
	RepositoryListItem *listItem = dynamic_cast<RepositoryListItem*>(fRepositoryListView->ItemAt(index));
	if (listItem == nullptr || listItem->CurrentRepository() == nullptr) {
		return;
	}

	Repository *repository = listItem->CurrentRepository();
	CommitModel *model = new CommitModel(repository);
	ContainerWindow *window = new ContainerWindow(model,fRepositoryManager);
	window->Show();
}

void
RepositoryWindow::HandleUserRepositories(BMessage *message)
{
	MessageFinder messageFinder;
	BMessage msg = messageFinder.FindMessage("nodes", *message);
	BMessage repositoriesMessage;

	char *name;
	uint32 type;
	int32 count;

	BList *list = new BList();

	for (int32 i = 0; msg.GetInfo(B_MESSAGE_TYPE, i, &name, &type, &count) == B_OK; i++) {
		BMessage nodeMsg;
		if (msg.FindMessage(name, &nodeMsg) == B_OK) {
			Repository *repository = new Repository();
			repository->SetIsManuallyAdded(false);
			repository->SetRepository(new GithubRepository(nodeMsg));
			list->AddItem(repository);
		}
	}

	fRepositoryManager->AddRepositories(list);
	delete list;
}

void
RepositoryWindow::HandleManualAddedRepository(BMessage *message)
{
	MessageFinder messageFinder;
	BMessage msg = messageFinder.FindMessage("repository", *message);
	Repository *repository = new Repository();
	repository->SetIsManuallyAdded(true);
	repository->SetRepository(new GithubRepository(msg));
	fRepositoryManager->AddRepository(repository);
}

void
RepositoryWindow::ParseData(BMessage *message)
{
	if (message->HasMessage("UserRepositories")) {
		HandleUserRepositories(message);
	} else if (message->HasMessage("Repository")) {
		HandleManualAddedRepository(message);
	}
}

void
RepositoryWindow::ShowAlert(const char *title, const char *text)
{
	BAlert* alert = new BAlert(title, text, "Ok", nullptr, nullptr, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
	alert->Go();
}
