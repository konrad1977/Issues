/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "IssueContainerModel.h"
#include "GithubClient.h"
#include "GithubIssue.h"
#include "GithubRepository.h"
#include "Constants.h"
#include "RepositoryTitleView.h"
#include "IssueListItem.h"
#include "MessageFinder.h"

#include <app/Messenger.h>

#include <stdio.h>

IssueContainerModel::IssueContainerModel(BString repository, BString owner)
	:fGithubClient(NULL)
	,fGithubRepository(NULL)
	,fRepositoryTitleView(NULL)
	,fMessenger(NULL)
	,fRepository(repository)
	,fOwner(owner)
{

}

IssueContainerModel::IssueContainerModel(BMessage *message)
	:fGithubClient(NULL)
	,fGithubRepository(NULL)
	,fRepositoryTitleView(NULL)
	,fMessenger(NULL)
{
	message->FindString("Repository", &fRepository);
	message->FindString("Owner", &fOwner);
}

IssueContainerModel::~IssueContainerModel()
{
	delete fGithubClient;
	delete fMessenger;
}

status_t
IssueContainerModel::Archive(BMessage *message)
{
	message->AddString("Repository", fRepository);
	message->AddString("Owner", fOwner);
	message->AddString("type", "issues");
	return B_ERROR;
}

void
IssueContainerModel::HandleParse(BMessage *message)
{
	if (message->HasMessage("Issues")) {
		AddIssues(message);
	} else if (message->HasMessage("Repository")) {
		AddRepository(message);
	}

	if(fMessenger) {
		BMessage resizeMsg(kContainerRequestResize);
		fMessenger->SendMessage(&resizeMsg);
	}
}

void
IssueContainerModel::AddIssues(BMessage *message)
{
	bool isReplicant = IsReplicant();
	MessageFinder messageFinder;
	BMessage msg = messageFinder.FindMessage("nodes", *message);
	BMessage repositoriesMessage;

	char *name;
	uint32 type;
	int32 count;

	BListView *list = ListView();

	while(list->CountItems()) {
		delete list->RemoveItem(int32(0));
	}

	for (int32 i = 0; msg.GetInfo(B_MESSAGE_TYPE, i, &name, &type, &count) == B_OK; i++) {
		BMessage nodeMsg;
		if (msg.FindMessage(name, &nodeMsg) == B_OK) {
			GithubIssue issue(nodeMsg);
			IssueListItem *listItem = new IssueListItem(issue, isReplicant);
			list->AddItem( listItem );
		}
	}
}

void
IssueContainerModel::AddRepository(BMessage *message)
{
	printf("AddRepository\n");
}

void
IssueContainerModel::MessageReceived(BMessage *message)
{
	switch (message->what) {
		case kDataReceivedMessage: {
 			HandleParse(message);
			break;
		}
		default:
			break;
	}
}

void
IssueContainerModel::SetTarget(BHandler *handler)
{
	delete fMessenger;
	fMessenger = new BMessenger(handler);

	delete fGithubClient;
	fGithubClient = new GithubClient(handler);
}

void
IssueContainerModel::RequestData()
{
	if (fGithubClient == NULL) {
		return;
	}

	if (fGithubRepository == NULL) {
		fGithubClient->RequestRepository(fRepository.String(), fOwner.String());
	}
	fGithubClient->RequestIssuesForRepository(fRepository.String(), fOwner.String());
}