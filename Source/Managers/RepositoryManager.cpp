/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "RepositoryManager.h"
#include "SettingsManager.h"
#include "Repository.h"
#include "Constants.h"

#include <support/List.h>
#include <app/Messenger.h>
#include <app/Handler.h>

#include <posix/stdio.h>

RepositoryManager::RepositoryManager(BHandler *handler)
	:fSettingsManager(nullptr)
	,fRepositoryList(nullptr)
	,fMessenger(nullptr)
{
	fSettingsManager = new SettingsManager();
	fMessenger = new BMessenger(handler);
	fRepositoryList = new BList();
	LoadRepositories();

	printf("RepositoryManager::LoadedItems(%d)\n", fRepositoryList->CountItems());
}

RepositoryManager::~RepositoryManager()
{
	delete fMessenger;
	delete fSettingsManager;

	while (fRepositoryList->CountItems()) {
		delete fRepositoryList->RemoveItem(int32(0));
	}

	delete fRepositoryList;
	printf("RepositoryManager::~RepositoryManager()\n");
}

BList *
RepositoryManager::Repositories() const
{
	return fRepositoryList;
}

void
RepositoryManager::AddRepositories(BList *list)
{
	const int32 items = list->CountItems();
	bool needsSave = false;
	for (int32 i = 0; i<items; i++) {
		Repository *item = static_cast<Repository*>(list->ItemAtFast(i));

		if (HasRepository(item)) {
			continue;
		}
		needsSave = true;
		fRepositoryList->AddItem(item);
	}

	if (needsSave) {
		SaveRepositories();
	}

	BMessage msg(Action::Added);
	fMessenger->SendMessage(&msg);
}

void
RepositoryManager::AddRepository(Repository *repository)
{
	if (HasRepository(repository)) {
		BMessage msg(Action::Exists);
		fMessenger->SendMessage(&msg);
		return;
	}

	fRepositoryList->AddItem(repository);
	SaveRepositories();

	BMessage msg(Action::Added);
	fMessenger->SendMessage(&msg);
}

void
RepositoryManager::RemoveRepository(Repository *repository)
{
	const int32 items = fRepositoryList->CountItems();

	for (int32 i = 0; i<items; i++) {
		Repository *item = static_cast<Repository*>(fRepositoryList->ItemAtFast(i));

		if (item == nullptr) {
			continue;
		}

		if (repository->Id() == item->Id()) {
			fRepositoryList->RemoveItem(i);

			SaveRepositories();

			BMessage msg(Action::Removed);
			fMessenger->SendMessage(&msg);
			break;
		}
	}
}

bool
RepositoryManager::HasRepository(Repository *repository)
{
	const int32 items = fRepositoryList->CountItems();
	for (int32 i = 0; i<items; i++) {
		Repository *item = static_cast<Repository*>(fRepositoryList->ItemAtFast(i));

		if (item == nullptr) {
			continue;
		}

		if (item->Id() == repository->Id()) {
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
	message.PrintToStream();

	int32 index = 0;
	BMessage repositoryMessage;
	while ( (message.FindMessage("Repositories", index, &repositoryMessage) == B_OK )) {
		BString repositoryName;
		if (repositoryMessage.FindString("Repository", &repositoryName) == B_OK) {
			Repository *repository = new Repository(repositoryMessage);
			fRepositoryList->AddItem((void*)repository);
		}
		index++;
	}
}

void
RepositoryManager::SaveRepositories()
{
	BMessage message;
	fSettingsManager->LoadSettings(message);

	message.RemoveName("Repositories");

	const int32 items = fRepositoryList->CountItems();
	for (int32 i = 0; i<items; i++) {
		Repository *item = static_cast<Repository*>(fRepositoryList->ItemAtFast(i));

		if (item == nullptr) {
			continue;
		}

		BMessage repositoryMessage;
		repositoryMessage.AddString("Repository", item->Name());
		item->Save(repositoryMessage);
		message.AddMessage("Repositories", &repositoryMessage);
	}
	fSettingsManager->SaveSettings(message);
}
