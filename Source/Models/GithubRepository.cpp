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
	message.AddString("name", name);
	message.AddString("description", description);
	message.AddBool("isFork", fIsFork);
	message.AddBool("isPrivate", fIsPrivate);
	message.AddString("url", url);
	message.AddString("id", id);

	BMessage ownerMsg;
	ownerMsg.AddString("login", owner);
	message.AddMessage("owner", &ownerMsg);
}

status_t
GithubRepository::Load(BMessage &message)
{
	message.FindString("name", &name);
	message.FindString("description", &description);
	message.FindBool("isFork", &fIsFork);
	message.FindBool("isPrivate", &fIsPrivate);
	message.FindString("url", &url);
	message.FindString("id", &id);

	BMessage ownerMessage;
	if (message.FindMessage("owner", &ownerMessage) == B_OK) {
		ownerMessage.FindString("login", &owner);
	}

	printf("------------------\n");
	message.PrintToStream();

	BMessage parentMesssage;
	if (message.FindMessage("parent", &parentMesssage) == B_OK) {
		BMessage parentOwnerMsg;
		if (parentMesssage.FindMessage("owner", &parentOwnerMsg) == B_OK ) {
			BString parentOwner;
			parentOwnerMsg.FindString("login", &parentOwner);
			owner = parentOwner;
		}
	}

	printf("Owner %s\n", owner.String());
	printf("------------------\n");
	return B_OK;
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

void
GithubRepository::PrintToStream()
{

}

