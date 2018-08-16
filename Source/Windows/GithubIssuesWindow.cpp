/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "GithubIssuesWindow.h"
#include "GithubClient.h"
#include "GithubRepository.h"
#include "GithubIssue.h"
#include "IssueListItem.h"
#include "Constants.h"
#include "MessageFinder.h"

#include <interface/MenuBar.h>
#include <interface/MenuItem.h>
#include <interface/StringItem.h>
#include <interface/GroupLayout.h>
#include <interface/LayoutBuilder.h>
#include <interface/ListView.h>
#include <interface/ScrollView.h>

#include <posix/stdio.h>

GithubIssuesWindow::GithubIssuesWindow(GithubRepository *repository)
	:BWindow(BRect(30,30,300,300), "Issues", B_DOCUMENT_WINDOW, 0)
	,fRepository(repository)
	,fGithubClient(NULL)
	,fListView(NULL)
{
	SetTitle(fRepository->name.String());
	SetupViews();
	
	fGithubClient = new GithubClient(this);
	RequestIssues();
}

GithubIssuesWindow::~GithubIssuesWindow()
{
	delete fGithubClient;	
}

void
GithubIssuesWindow::ParseIssueData(BMessage *message) 
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
}

void 
GithubIssuesWindow::MessageReceived(BMessage *message)
{
	switch (message->what) {
		case kDataReceivedMessage: {
 			ParseIssueData(message);
			break;
		}	
		default:
			BWindow::MessageReceived(message);
	}
}

void 
GithubIssuesWindow::RequestIssues()
{
	fGithubClient->RequestIssuesForRepository(fRepository->name);
}

void 
GithubIssuesWindow::SetupViews()
{
	BGroupLayout *layout = new BGroupLayout(B_VERTICAL);
	layout->SetSpacing(0);
	SetLayout(layout);
	
	fListView = new BListView("Issues", B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL | B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE);
	BScrollView *scrollView = new BScrollView("Scrollview", fListView, B_FOLLOW_ALL, 0, false, true);
	
	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(scrollView);
}

