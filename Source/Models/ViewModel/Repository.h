/*
 * Copyright 2018 Mikael Konradsson <mikael.konradson@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <SupportDefs.h>
#include <String.h>
#include <Message.h>

class GithubRepository;
class Repository {
public:

	Repository(GithubRepository *repository);
	~Repository();

	status_t Save(BMessage &message);
	status_t Load(BMessage &message);

	const BString& Name() const;
	const BString& Owner() const;
	const BString& Description() const;
	const BString& Url() const;
	const BString& Id() const;

	bool IsFork() const;
	bool IsPrivate() const;
	int SortOrder();

	uint8 Transparency() const;
	uint8 RefreshRate() const;

private:
	GithubRepository *fRepository;
};


#endif // _H
