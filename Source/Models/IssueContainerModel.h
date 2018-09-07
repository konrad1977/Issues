/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef ISSUE_CONTAINER_MODEL_H
#define ISSUE_CONTAINER_MODEL_H


#include <SupportDefs.h>
#include <support/String.h>
#include "ContainerModel.h"

class BMessenger;
class GithubRepository;
class GithubClient;
class RepositoryTitleView;
class IssueContainerModel : public ContainerModel {
public:

	IssueContainerModel(BString repository, BString owner);
	virtual ~IssueContainerModel();

	virtual status_t Archive(BMessage *message);
	virtual status_t Unarchive(BMessage *message);

	virtual void MessageReceived(BMessage *message);
	virtual void RequestData();
	virtual void SetTarget(BHandler *handler);

private:
			void HandleParse(BMessage *message);
			void AddIssues(BMessage *message);
			void AddRepository(BMessage *message);

	GithubClient 		*fGithubClient;
	GithubRepository	*fGithubRepository;
	RepositoryTitleView	*fRepositoryTitleView;
	BMessenger			*fMessenger;
	BString				fRepository;
	BString 			fOwner;
};

#endif // _H
