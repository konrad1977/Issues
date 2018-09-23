/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include "GithubCommit.h"

GithubCommit::GithubCommit(BMessage message)
{
	message.FindString("messageHeadline", &title);
	message.FindString("message", &body);
	message.FindString("url", &url);

	BMessage authorMsg;
	if (message.FindMessage("author", &authorMsg) == B_OK) {
		authorMsg.FindString("name", &author);
		authorMsg.FindString("avatarUrl", &authorUrl);
	}
}

GithubCommit::~GithubCommit()
{

}
