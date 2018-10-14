/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "GithubRepository.h"
#include <string>
#include <HttpAuthentication.h>

GithubRepository::GithubRepository(BMessage message)
{
	Load(message);
}

GithubRepository::~GithubRepository()
{

}

status_t
GithubRepository::Save(BMessage &message)
{
	message.AddString("name", fName);
	message.AddString("description", fDescription);
	message.AddBool("isFork", fIsFork);
	message.AddBool("isPrivate", fIsPrivate);
	message.AddString("url", fUrl);
	message.AddString("id", fId);

	BMessage ownerMsg;
	ownerMsg.AddString("login", fOwner);
	message.AddMessage("owner", &ownerMsg);
}

status_t
GithubRepository::Load(BMessage &message)
{
	message.FindString("name", &fName);
	message.FindString("description", &fDescription);
	message.FindBool("isFork", &fIsFork);
	message.FindBool("isPrivate", &fIsPrivate);
	message.FindString("url", &fUrl);
	message.FindString("id", &fId);

	BMessage ownerMessage;
	if (message.FindMessage("owner", &ownerMessage) == B_OK) {
		ownerMessage.FindString("login", &fOwner);
	}

	BMessage parentMesssage;
	if (message.FindMessage("parent", &parentMesssage) == B_OK) {
		BMessage parentOwnerMsg;
		if (parentMesssage.FindMessage("owner", &parentOwnerMsg) == B_OK ) {
			BString parentOwner;
			parentOwnerMsg.FindString("login", &parentOwner);
			fOwner = parentOwner;
		}
	}
	return B_OK;
}

BString
GithubRepository::Name() const
{
	return fName;
}

BString
GithubRepository::Owner() const
{
	return fOwner;
}

BString
GithubRepository::Description() const
{
	return fDescription;
}

const BString
GithubRepository::Url() const
{
	return fUrl;
}

const BString
GithubRepository::Id() const
{
	return fId;
}

bool
GithubRepository::IsFork() const
{
	return fIsFork;
}

bool
GithubRepository::IsPrivate() const
{
	return fIsPrivate;
}

int
GithubRepository::SortOrder()
{
	if (IsFork()) {
		return 2;
	} else if (IsPrivate()) {
		return 1;
	}
	return 0;
}
