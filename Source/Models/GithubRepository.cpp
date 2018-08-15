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
	message.FindBool("fork", &fIsFork);
	message.FindBool("private", &fIsPrivate);
	message.FindString("url", &url);
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

