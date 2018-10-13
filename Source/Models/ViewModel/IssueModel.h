/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef ISSUEMODEL_H
#define ISSUEMODEL_H


#include <SupportDefs.h>
#include <support/String.h>
#include "ContainerModel.h"

class BMessenger;
class GithubRepository;
class GithubClient;
class IssueModel : public ContainerModel {
public:

	IssueModel(BString repository, BString owner);
	IssueModel(BMessage *message);
	virtual ~IssueModel();

	virtual BString Name();
	virtual status_t Archive(BMessage *message);

	virtual void MessageReceived(BMessage *message);
	virtual void RequestData();
	virtual void SetTarget(BHandler *handler);

	const GithubRepository* Repository() { return fGithubRepository; }


private:
			void HandleParse(BMessage *message);
			void AddIssues(BMessage *message);

	GithubClient 		*fGithubClient;
	GithubRepository	*fGithubRepository;
	BMessenger			*fMessenger;
	BString				fRepository;
	BString 			fOwner;
};

#endif // _H
