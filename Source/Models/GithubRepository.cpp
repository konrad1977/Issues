/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "GithubRepository.h"
#include <string>
#include <HttpAuthentication.h>

GithubRepository::GithubRepository(BMessage message)
{
	message.FindString("name", &name);
	message.FindString("description", &description);
	message.FindBool("isFork", &fIsFork);
	message.FindBool("isPrivate", &fIsPrivate);
	message.FindString("url", &url);

	BMessage ownerMessage;
	if (message.FindMessage("owner", &ownerMessage) == B_OK) {
		ownerMessage.FindString("login", &owner);
	}
}

GithubRepository::~GithubRepository()
{

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

