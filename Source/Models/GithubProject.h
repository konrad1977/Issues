/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef GITHUBPROJECT_H
#define GITHUBPROJECT_H


#include <SupportDefs.h>
#include <Message.h>
#include <String.h>

class GithubProject {
public:
	GithubProject(BMessage message);
	~GithubProject();

	BString name;
	BString description;
	BString url;
	
	int32 id;
	
	bool IsFork() const;
	bool IsPrivate() const;
	
private:
	bool fIsFork;
	bool fIsPrivate;
	
};


#endif // _H
