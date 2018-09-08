/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "CommitsModel.h"

#include "GithubClient.h"
#include "GithubIssue.h"
#include "GithubRepository.h"

#include "Constants.h"
#include "CListItem.h"
#include "IssueTitleItem.h"
#include "MessageFinder.h"

#include <app/Messenger.h>

#include <stdio.h>

CommitsModel::CommitsModel(BString repository, BString owner)
	:fGithubClient(NULL)
	,fGithubRepository(NULL)
	,fMessenger(NULL)
	,fRepository(repository)
	,fOwner(owner)
{

}

CommitsModel::CommitsModel(BMessage *message)
	:fGithubClient(NULL)
	,fGithubRepository(NULL)
	,fMessenger(NULL)
{
	message->FindString("Repository", &fRepository);
	message->FindString("Owner", &fOwner);
}

CommitsModel::~CommitsModel()
{
	delete fGithubClient;
	delete fMessenger;
}

BString 
CommitsModel::Name() 
{
	return fRepository;
}

status_t
CommitsModel::Archive(BMessage *message)
{
	message->AddString("Repository", fRepository);
	message->AddString("Owner", fOwner);
	message->AddString("type", "commits");
	return B_ERROR;
}

void
CommitsModel::HandleParse(BMessage *message)
{
	if (message->HasMessage("Commits") == false) {
		return;
	}

	AddCommits(message);

	if(fMessenger) {
		BMessage resizeMsg(kContainerRequestResize);
		fMessenger->SendMessage(&resizeMsg);
	}
}

void
CommitsModel::AddCommits(BMessage *message)
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
	
	IssueTitleItem *titleItem = new IssueTitleItem(fRepository.String(), isReplicant);
	list->AddItem( titleItem );
	
	for (int32 i = 0; msg.GetInfo(B_MESSAGE_TYPE, i, &name, &type, &count) == B_OK; i++) {
		BMessage nodeMsg;
		if (msg.FindMessage(name, &nodeMsg) == B_OK) {
			GithubIssue issue(nodeMsg);
			CListItem *listItem = new CListItem(issue, isReplicant);
			list->AddItem( listItem );
		}
	}
}

void
CommitsModel::MessageReceived(BMessage *message)
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
CommitsModel::SetTarget(BHandler *handler)
{
	delete fMessenger;
	fMessenger = new BMessenger(handler);

	delete fGithubClient;
	fGithubClient = new GithubClient(handler);
}

void
CommitsModel::RequestData()
{
	if (fGithubClient == NULL) {
		return;
	}
	fGithubClient->RequestCommitsForRepository(fRepository.String(), fOwner.String());
}
