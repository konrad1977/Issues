/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef REPOSITORYMANAGER_H
#define REPOSITORYMANAGER_H


#include <SupportDefs.h>

class BList;
class SettingsManager;
class GithubRepository;
class RepositoryManager {
public:
	RepositoryManager();
	~RepositoryManager();

	BList *Repositories() const;

	void AddRepository(GithubRepository *repository);
	void RemoveRepository(GithubRepository *repository);
	bool HasRepository(GithubRepository *repository);

private:

	void LoadRepositories();
	void SaveRepositories();

	SettingsManager *fSettingsManager;
	BList 			*fList;
};


#endif // _H
