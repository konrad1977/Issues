/*
 * Copyright 2018 Mikael Konradsson <mikael.konradson@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <SupportDefs.h>
#include <String.h>
#include <Message.h>

enum RepositoryAction {
	SettingsChanged = 'rchs'
};

class BHandler;
class BMessenger;
class BMessage;
class GithubRepository;
class Repository {
public:

	Repository();
	Repository(BMessage &message);
	~Repository();

	void SetRepository(GithubRepository *repository);
	void SetTarget(BHandler *handler);
	void ReloadSavedData();

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

	void SetTransparency(uint8 value);
	void SetRefreshRate(uint8 value);

	uint8 Transparency() const;
	uint8 RefreshRate() const;

private:

	void NotifyUpdates();

	bool 				fIsManuallyAdded;

	uint8				fRefreshrate;
	uint8 				fTransparency;

	GithubRepository 	*fRepository;
	BMessenger 			*fMessenger;
};


#endif // _H
