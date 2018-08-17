/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include "IssuesContainerView.h"
#include "GithubClient.h"
#include "GithubIssue.h"
#include "IssueListItem.h"
#include "Constants.h"
#include "MessageFinder.h"

#include <interface/GroupLayout.h>
#include <interface/LayoutBuilder.h>
#include <interface/ListView.h>
#include <interface/ScrollView.h>

#include <posix/stdlib.h>
#include <posix/string.h>

IssuesContainerView::IssuesContainerView(const char *repositoryName)
	:BView("issues", B_SUPPORTS_LAYOUT) 
	,fGithubClient(NULL)
	,fListView(NULL)
	,fRepositoryName(NULL)
	,fThreadId(-1)
{
	SetupViews();
 	fRepositoryName = strdup(repositoryName);
}

IssuesContainerView::~IssuesContainerView()
{
	free(fRepositoryName);
	delete fGithubClient;	
}

void 
IssuesContainerView::AttachedToWindow()
{
	fGithubClient = new GithubClient(this);
	SpawnDonwloadThread();
}

void
IssuesContainerView::MessageReceived(BMessage *message)
{
	switch (message->what) {
		case kDataReceivedMessage: {
 			ParseIssueData(message);
			break;
		}	
		default:
			BView::MessageReceived(message);
	}
}

int32
IssuesContainerView::DownloadFunc(void *cookie)
{
	IssuesContainerView *view = static_cast<IssuesContainerView*>(cookie);
	view->RequestIssues();
	return 0;
}

void 
IssuesContainerView::RequestIssues()
{
	fGithubClient->RequestIssuesForRepository(fRepositoryName);
}

void 
IssuesContainerView::SpawnDonwloadThread()	
{	
	wait_for_thread(fThreadId, NULL);

	fThreadId = spawn_thread(&DownloadFunc, "Download Issues", B_NORMAL_PRIORITY, this);
	if (fThreadId >= 0)
		resume_thread(fThreadId);
}

void 
IssuesContainerView::ParseIssueData(BMessage *message)
{
	if (message->HasMessage("Issues") == false) {
		return;
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
			GithubIssue *issue = new GithubIssue(nodeMsg);
			IssueListItem *listItem = new IssueListItem(issue);
			fListView->AddItem( listItem );
		}
	}
	fListView->Invalidate();
}

void
IssuesContainerView::SetupViews()
{	
	fListView = new BListView("Issues", B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL | B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE);
	BScrollView *scrollView = new BScrollView("Scrollview", fListView, B_FOLLOW_ALL, 0, false, true);
	
	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(scrollView);
}
