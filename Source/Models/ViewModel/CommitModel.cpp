/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "CommitModel.h"

#include "GithubClient.h"
#include "GithubCommit.h"
#include "Repository.h"
#include "NetRequester.h"

#include "Constants.h"
#include "CListItem.h"
#include "IssueTitleItem.h"
#include "MessageFinder.h"

#include <app/Messenger.h>

#include <stdio.h>

CommitModel::CommitModel(Repository *repository)
	:fGithubClient(nullptr)
	,fRepository(repository)
	,fMessenger(nullptr)
{
}

CommitModel::CommitModel(BMessage *message)
	:fGithubClient(nullptr)
	,fRepository(nullptr)
	,fMessenger(nullptr)
{
	fRepository = new Repository(*message);
}

CommitModel::~CommitModel()
{
	delete fGithubClient;
	delete fMessenger;
}

BString
CommitModel::Name()
{
	return fRepository->Name();
}

status_t
CommitModel::Archive(BMessage *message)
{
	fRepository->Save(*message);
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
	
	SetupTitle(list);

	uint8 transparency = fRepository->CurrentSettings()->Transparency();

	for (int32 i = 0; msg.GetInfo(B_MESSAGE_TYPE, i, &name, &type, &count) == B_OK; i++) {
		BMessage nodeMsg;
		if (msg.FindMessage(name, &nodeMsg) == B_OK) {
			GithubCommit commit(nodeMsg);
			CListItem *listItem = new CListItem(commit, isReplicant);
			listItem->SetTransparency(transparency);
			list->AddItem( listItem );
		}
	}
}

void
CommitModel::SetupTitle(BListView *list)
{
	if (fRepository->CurrentSettings()->ShowTitle() == false) {
		return;
	}
	
	TitleSettings settings;
	settings.title = fRepository->Name();
	settings.subTitle = "commits";

	IssueTitleItem *titleItem = new IssueTitleItem(settings, IsReplicant());
	list->AddItem( titleItem );	
}

void
CommitModel::MessageReceived(BMessage *message)
{
	switch (message->what) {
		case NetRequesterAction::DataReceived: {
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
	if (fGithubClient == nullptr || fRepository == nullptr) {
		return;
	}
	fGithubClient->RequestCommitsForRepository(fRepository->Name().String(), fRepository->Owner().String());
}
