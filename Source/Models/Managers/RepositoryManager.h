/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef REPOSITORYMANAGER_H
#define REPOSITORYMANAGER_H


#include <SupportDefs.h>
#include <app/Message.h>

#include "Repository.h"

class BList;
class BMessenger;
class BHandler;
class SettingsManager;
class RepositoryManager {
public:

	RepositoryManager(BHandler *handler);
	~RepositoryManager();

	BList *Repositories() const;

	BMessage* RepositoryMessage(BString name);

	void SaveRepositories();
	void AddRepositories(BList *list);
	void AddRepository(const Repository *repository);
	void RemoveRepository(const Repository *repository);
	bool HasRepository(const Repository *repository);

	enum Action {
		Added 	= 'repa',
		Removed	= 'repr',
		Loaded 	= 'repl',
		Exists	= 'repe'
	};

private:

	void LoadRepositories();

	SettingsManager *fSettingsManager;
	BList 			*fRepositoryList;
	BMessenger 		*fMessenger;
};


#endif // _H
