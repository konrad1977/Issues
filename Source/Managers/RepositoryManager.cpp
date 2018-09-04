/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "RepositoryManager.h"
#include "SettingsManager.h"
#include "GithubRepository.h"

#include <support/List.h>

RepositoryManager::RepositoryManager()
	:fSettingsManager(NULL)
{
	fList = new BList();
	fSettingsManager = new SettingsManager();
}

RepositoryManager::~RepositoryManager()
{
	delete fSettingsManager;
	delete fList;
}

BList *
RepositoryManager::Repositories() const
{
	return fList;
}

void
RepositoryManager::AddRepository(GithubRepository *repository)
{
	if (HasRepository(repository)) {
		return;
	}

	fList->AddItem(reinterpret_cast<void*>(repository));
	SaveRepositories();
}

void
RepositoryManager::RemoveRepository(GithubRepository *repository)
{
	const int32 items = fList->CountItems();

	for (int32 i = 0; i<items; i++) {
		GithubRepository *item = static_cast<GithubRepository*>(fList->ItemAtFast(i));

		if (item == NULL) {
			continue;
		}

		if (repository->id == item->id) {
			fList->RemoveItem(i);
			SaveRepositories();
			break;
		}
	}
}

bool
RepositoryManager::HasRepository(GithubRepository *repository)
{
	const int32 items = fList->CountItems();
	for (int32 i = 0; i<items; i++) {
		GithubRepository *item = static_cast<GithubRepository*>(fList->ItemAtFast(i));
		if (item == NULL) {
			continue;
		}
		if (item->id == repository->id) {
			return true;
		}
	}
	return false;
}

void
RepositoryManager::LoadRepositories()
{
	BMessage message;
	fSettingsManager->LoadSettings(message);

	int32 index = 0;
	BMessage repositoryMessage;
	while ( (message.FindMessage("Repositories", index, &repositoryMessage) == B_OK )) {
		BString repositoryName;
		if (repositoryMessage.FindString("Repository", &repositoryName) == B_OK) {
			GithubRepository *repository = new GithubRepository(repositoryMessage);
			fList->AddItem((void*)repository);
		}
		index++;
	}
}

void
RepositoryManager::SaveRepositories()
{
	BMessage message;
	const int32 items = fList->CountItems();
	for (int32 i = 0; i<items; i++) {
		GithubRepository *item = static_cast<GithubRepository*>(fList->ItemAtFast(i));

		if (item == NULL) {
			continue;
		}
		BMessage repositoryMessage;
		repositoryMessage.AddString("Repository", item->name);
		item->Save(repositoryMessage);
		message.AddMessage("Repositories", &repositoryMessage);
	}

	fSettingsManager->SaveSettings(message);
}


