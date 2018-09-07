/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include "IssuesContainerView.h"
#include "GithubClient.h"
#include "GithubIssue.h"
#include "GithubRepository.h"

#include "RepositoryTitleView.h"
#include "IssueListItem.h"
#include "Constants.h"
#include "MessageFinder.h"
#include "IssueContainerModel.h"

#include <interface/GroupLayout.h>
#include <interface/LayoutBuilder.h>
#include <interface/Window.h>
#include <interface/ListView.h>
#include <interface/ListItem.h>
#include <interface/ScrollView.h>
#include <interface/Dragger.h>

#include <app/MessageRunner.h>
#include <app/Roster.h>


#include <posix/stdlib.h>
#include <posix/string.h>
#include <posix/stdio.h>

const float kDraggerSize = 7;
extern const char *kAppSignature;

IssuesContainerView::IssuesContainerView(ContainerModel *model)
	:BView("Issues", B_DRAW_ON_CHILDREN)
	,fListView(NULL)
	,fScrollView(NULL)
	,fDragger(NULL)
	,fAutoUpdateRunner(NULL)
	,fThreadId(-1)
	,fIsReplicant(false)
	,fContainerModel(model)
{
	SetupViews(fIsReplicant);
	SpawnDonwloadThread();
}

IssuesContainerView::IssuesContainerView(BMessage *message)
	:BView(message)
	,fListView(NULL)
	,fScrollView(NULL)
	,fDragger(NULL)
	,fAutoUpdateRunner(NULL)
	,fThreadId(-1)
	,fIsReplicant(true)
	,fContainerModel(NULL)
{
	BString type;
	if (message->FindString("type", &type) == B_OK) {
		if (type == "issues") {
			fContainerModel = new IssueContainerModel(message);
		}
	}

	SetupViews(fIsReplicant);
	SpawnDonwloadThread();
}


IssuesContainerView::~IssuesContainerView()
{
	while(fListView->CountItems()) {
		delete fListView->RemoveItem(int32(0));
	}
	delete fListView;
}

status_t
IssuesContainerView::Archive(BMessage* into, bool deep) const
{
	into->AddString("add_on", kAppSignature);
	Model()->Archive(into);
	return BView::Archive(into, false);
}

BArchivable*
IssuesContainerView::Instantiate(BMessage* archive)
{
	return new IssuesContainerView(archive);
}

status_t
IssuesContainerView::SaveState(BMessage* into, bool deep) const
{
	return B_OK;
}

void
IssuesContainerView::AttachedToWindow()
{
	StartAutoUpdater();
	ListView()->SetTarget(this);
	Model()->SetTarget(this);
	BView::AttachedToWindow();
}

void
IssuesContainerView::Reisize()
{
	if (fListView == NULL ) {
		return;
	}

	float width;
	float height;
	fListView->GetPreferredSize(&width, &height);
	fListView->SetExplicitMinSize(BSize(320, height < 420 ? height : 420));

	if (fIsReplicant) {
		//height += fRepositoryTitleView->MinSize().height;
		height += kDraggerSize;
		ResizeTo(Bounds().Width(), height);
	} else if (BWindow *window = dynamic_cast<BWindow*>(Parent())) {
		window->CenterOnScreen();
	}
	fListView->Invalidate();
}

void
IssuesContainerView::MessageReceived(BMessage *message)
{
	Model()->MessageReceived(message);

	switch (message->what) {
		case kContainerRequestResize: {
			Reisize();
			printf("Resize\n");
			break;
		}

		case kIssueListInvokedMessage: {
			HandleListInvoke(message);
			break;
		}

		case kAutoUpdateMessage: {
			SpawnDonwloadThread();
			break;
		}
		default:
			BView::MessageReceived(message);
	}
}

void
IssuesContainerView::StartAutoUpdater()
{
	delete fAutoUpdateRunner;

	BMessenger view(this);
	bigtime_t seconds = 10;

	BMessage autoUpdateMessage(kAutoUpdateMessage);
	fAutoUpdateRunner = new BMessageRunner(view, &autoUpdateMessage, (bigtime_t) seconds * 1000 * 1000);
}

BListView *
IssuesContainerView::ListView()
{
	if (fListView == NULL) {
		fListView = new BListView("Issues", B_SINGLE_SELECTION_LIST, B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE);
		fListView->SetInvocationMessage( new BMessage(kIssueListInvokedMessage ));
	}
	return fListView;
}

int32
IssuesContainerView::DownloadFunc(void *cookie)
{
	IssuesContainerView *view = static_cast<IssuesContainerView*>(cookie);
	view->Model()->RequestData();
	return 0;
}

void
IssuesContainerView::SpawnDonwloadThread()
{
	StopDownloadThread();

	fThreadId = spawn_thread(&DownloadFunc, "Download Issues", B_NORMAL_PRIORITY, this);
	if (fThreadId >= 0)
		resume_thread(fThreadId);
}

void
IssuesContainerView::StopDownloadThread()
{
	if (fThreadId == -1) {
		return;
	}
	wait_for_thread(fThreadId, NULL);
	fThreadId = -1;
}

void
IssuesContainerView::HandleListInvoke(BMessage *message)
{
	int32 index = B_ERROR;
	if (message->FindInt32("index", &index) == B_OK) {
		IssueListItem *listItem = dynamic_cast<IssueListItem*>(ListView()->ItemAt(index));

		if (listItem == NULL) {
			return;
		}

		const char *url = listItem->CurrentIssue().url.String();
		if (strlen(url) > 10 ) {
			be_roster->Launch("text/html", 1, const_cast<char **>(&url));
		}
	}
}

void
IssuesContainerView::AddRepository(BMessage *message)
{
	/*
	MessageFinder messageFinder;
	BMessage msg = messageFinder.FindMessage("repository", *message);

	delete fGithubRepository;
	fGithubRepository = new GithubRepository(msg);
	fRepositoryTitleView->SetRepository(fGithubRepository);
	*/
}

void
IssuesContainerView::SetupViews(bool isReplicant)
{
	//fRepositoryTitleView = new RepositoryTitleView(isReplicant);

	if (isReplicant == false) {
		SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
		fScrollView = new BScrollView("Scrollview", ListView(), 0, false, true, B_NO_BORDER);
	} else {
		SetViewColor(B_TRANSPARENT_COLOR);
		ListView()->SetViewColor(B_TRANSPARENT_COLOR);
	}

	fContainerModel->SetListView(ListView());
	fContainerModel->SetIsReplicant(isReplicant);
	fContainerModel->SetTarget(this);

	BSize draggerSize = BSize(kDraggerSize,kDraggerSize);
	fDragger = new BDragger(this);

	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.AddGroup(B_VERTICAL, 0)
			//.Add(fRepositoryTitleView)
			.Add(isReplicant ? static_cast<BView*>(ListView()) : static_cast<BView*>(fScrollView))
		.End()
		.AddGroup(B_HORIZONTAL)
			.AddGlue()
			.SetExplicitMinSize(draggerSize)
			.SetExplicitMaxSize(draggerSize)
			.Add(fDragger)
		.End()
	.End();
}
