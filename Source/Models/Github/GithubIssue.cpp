/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "GithubIssue.h"

GithubIssue::GithubIssue(BMessage message)
{
	message.FindString("title", &title);
	message.FindString("body", &body);
	message.FindString("url", &url);

	BMessage authorMsg;
	if (message.FindMessage("author", &authorMsg) == B_OK) {
		authorMsg.FindString("login", &author);
		authorMsg.FindString("avatarUrl", &authorUrl);
	}
}

GithubIssue::~GithubIssue()
{

}
