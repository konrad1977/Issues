/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "CommitModel.h"

#include "GithubClient.h"
#include "GithubIssue.h"
#include "GithubRepository.h"
#include "GithubCommit.h"

#include "Constants.h"
#include "CListItem.h"
#include "IssueTitleItem.h"
#include "MessageFinder.h"

#include <app/Messenger.h>

#include <stdio.h>

CommitModel::CommitModel(BString repository, BString owner)
	:fGithubClient(NULL)
	,fGithubRepository(NULL)
	,fMessenger(NULL)
	,fRepository(repository)
	,fOwner(owner)
{

}

CommitModel::CommitModel(BMessage *message)
	:fGithubClient(NULL)
	,fGithubRepository(NULL)
	,fMessenger(NULL)
{
	message->FindString("Repository", &fRepository);
	message->FindString("Owner", &fOwner);
}

CommitModel::~CommitModel()
{
	delete fGithubClient;
	delete fMessenger;
}

BString
CommitModel::Name()
{
	return fRepository;
}

status_t
CommitModel::Archive(BMessage *message)
{
	message->AddString("Repository", fRepository);
	message->AddString("Owner", fOwner);
	message->AddString("type", "commits");
	return B_ERROR;
}

void
CommitModel::HandleParse(BMessage *message)
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
CommitModel::AddCommits(BMessage *message)
{
	message->PrintToStream();


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

	TitleSettings settings;
	settings.title = fRepository;
	settings.subTitle = "commits";

	IssueTitleItem *titleItem = new IssueTitleItem(settings, isReplicant);
	list->AddItem( titleItem );

	for (int32 i = 0; msg.GetInfo(B_MESSAGE_TYPE, i, &name, &type, &count) == B_OK; i++) {
		BMessage nodeMsg;
		if (msg.FindMessage(name, &nodeMsg) == B_OK) {
			GithubCommit commit(nodeMsg);
			CListItem *listItem = new CListItem(commit, isReplicant);
			list->AddItem( listItem );
		}
	}
}

void
CommitModel::MessageReceived(BMessage *message)
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
CommitModel::SetTarget(BHandler *handler)
{
	delete fMessenger;
	fMessenger = new BMessenger(handler);

	delete fGithubClient;
	fGithubClient = new GithubClient(handler);
}

void
CommitModel::RequestData()
{
	if (fGithubClient == NULL) {
		return;
	}
	fGithubClient->RequestCommitsForRepository(fRepository.String(), fOwner.String());
}
