/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef GITHUBREPOSITORY_H
#define GITHUBREPOSITORY_H


#include <SupportDefs.h>
#include <Message.h>
#include <String.h>

class GithubRepository {
public:
	GithubRepository(BMessage message);
	~GithubRepository();

	BString name;
	BString owner;
	BString description;
	BString url;
	BString id;

	bool IsFork() const;
	bool IsPrivate() const;

	int SortOrder();

	status_t Save(BMessage &message);
	status_t Load(BMessage &message);

private:
	bool fIsFork;
	bool fIsPrivate;

};


#endif // _H
