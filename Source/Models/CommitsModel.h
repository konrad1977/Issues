/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef COMMITSMODEL_H
#define COMMITSMODEL_H


#include <SupportDefs.h>
#include <support/String.h>
#include "ContainerModel.h"

class BMessenger;
class GithubRepository;
class GithubClient;
class CommitsModel : public ContainerModel {
public:

	CommitsModel(BString repository, BString owner);
	CommitsModel(BMessage *message);
	virtual ~CommitsModel();

	virtual BString Name();	
	virtual status_t Archive(BMessage *message);

	virtual void MessageReceived(BMessage *message);
	virtual void RequestData();
	virtual void SetTarget(BHandler *handler);

private:
			void HandleParse(BMessage *message);
			void AddCommits(BMessage *message);

	GithubClient 		*fGithubClient;
	GithubRepository	*fGithubRepository;
	BMessenger			*fMessenger;
	BString				fRepository;
	BString 			fOwner;
};

#endif // _H