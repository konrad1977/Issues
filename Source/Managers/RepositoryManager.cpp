/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "RepositoryManager.h"
#include "SettingsManager.h"
#include "GithubRepository.h"
#include "Constants.h"

#include <support/List.h>
#include <app/Messenger.h>
#include <app/Handler.h>

RepositoryManager::RepositoryManager(BHandler *handler)
	:fSettingsManager(nullptr)
	,fMessenger(nullptr)
{
	fList = new BList();
	fSettingsManager = new SettingsManager();
	fMessenger = new BMessenger(handler);
	LoadRepositories();
}

RepositoryManager::~RepositoryManager()
{
	delete fMessenger;
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
		BMessage msg(kRepositoryManagerExists);
		fMessenger->SendMessage(&msg);
		return;
	}

	fList->AddItem(reinterpret_cast<void*>(repository));
	SaveRepositories();

	BMessage msg(kRepositoryManagerAdd);
	fMessenger->SendMessage(&msg);
}

void
RepositoryManager::RemoveRepository(GithubRepository *repository)
{
	const int32 items = fList->CountItems();

	for (int32 i = 0; i<items; i++) {
		GithubRepository *item = static_cast<GithubRepository*>(fList->ItemAtFast(i));

		if (item == nullptr) {
			continue;
		}

		if (repository->id == item->id) {
			fList->RemoveItem(i);
			SaveRepositories();
			BMessage msg(kRepositoryManagerRemove);
			fMessenger->SendMessage(&msg);
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
		if (item == nullptr) {
			continue;
		}
		if (item->url == repository->url) {
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
	BMessage msg(kRepositoryManagerLoaded);
	fMessenger->SendMessage(&msg);
}

void
RepositoryManager::SaveRepositories()
{
	BMessage message;
	fSettingsManager->LoadSettings(message);

	const int32 items = fList->CountItems();
	for (int32 i = 0; i<items; i++) {
		GithubRepository *item = static_cast<GithubRepository*>(fList->ItemAtFast(i));

		if (item == nullptr) {
			continue;
		}
		BMessage repositoryMessage;
		repositoryMessage.AddString("Repository", item->name);
		item->Save(repositoryMessage);
		message.AddMessage("Repositories", &repositoryMessage);
	}

	fSettingsManager->SaveSettings(message);
}
