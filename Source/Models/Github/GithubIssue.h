/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef GITHUBISSUE_H
#define GITHUBISSUE_H


#include <SupportDefs.h>
#include <app/Message.h>
#include <support/String.h>

class GithubIssue {
public:
	GithubIssue(BMessage message);
	~GithubIssue();

	BString title;
	BString body;	
	BString url;
	BString author;
	
private:

};

#endif // _H
