/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef REPOSITORYMANAGER_H
#define REPOSITORYMANAGER_H


#include <SupportDefs.h>
#include <app/Message.h>

class BList;
class BMessenger;
class BHandler;
class SettingsManager;
class Repository;
class RepositoryManager {
public:

	RepositoryManager(BHandler *handler);
	~RepositoryManager();

	BList *Repositories() const;

	BMessage *RepositoryLoadMessage(BString name);

	void AddRepositories(BList *list);
	void AddRepository(Repository *repository);
	void RemoveRepository(Repository *repository);
	bool HasRepository(Repository *repository);

	enum Action {
		Added 	= 'repa',
		Removed	= 'repr',
		Loaded 	= 'repl',
		Exists	= 'repe'
	};

private:

	void LoadRepositories();
	void SaveRepositories();

	SettingsManager *fSettingsManager;
	BList 			*fRepositoryList;
	BMessenger 		*fMessenger;
};


#endif // _H
