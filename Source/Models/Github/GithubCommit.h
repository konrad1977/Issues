/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef GITHUBCOMMIT_H
#define GITHUBCOMMIT_H


#include <SupportDefs.h>
#include <app/Message.h>
#include <support/String.h>

class GithubCommit {
public:
	GithubCommit(BMessage message);
	~GithubCommit();

	BString title;
	BString body;	
	BString url;
	BString author;
	
private:

};

#endif // _H