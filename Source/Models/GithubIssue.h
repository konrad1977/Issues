/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef _H
#define _H


#include <SupportDefs.h>
#include <app/Message.h>
#include <support/String.h>

class GithubIssue {
public:
	GithubIssue(BMessage message);
	~GithubIssue();

	BString title;
	BString body;								
private:

};

#endif // _H
