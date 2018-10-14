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

	Repository();
	Repository(BMessage &message);
	~Repository();

	void SetRepository(GithubRepository *repository);

	status_t Save(BMessage &message);
	status_t Load(BMessage &message);

	BString Name() const;
	BString Owner() const;
	BString Description() const;
	BString Url() const;
	BString Id() const;

	bool IsFork() const;
	bool IsPrivate() const;
	bool IsManuallyAdded() const;

	void SetIsManuallyAdded(bool value);
	int SortOrder();

	uint8 Transparency() const;
	uint8 RefreshRate() const;

private:
	GithubRepository 	*fRepository;
	bool 				fIsManuallyAdded;
};


#endif // _H
