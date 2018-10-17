/*
 * Copyright 2018 Mikael Konradsson <mikael.konradson@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "Repository.h"
#include "GithubRepository.h"
#include "RepositoryManager.h"
#include <app/Messenger.h>
#include <app/Handler.h>
#include <posix/stdio.h>

Repository::Repository()
	:fIsManuallyAdded(false)
	,fRefreshrate(10)
	,fTransparency(127)
	,fRepository(nullptr)
	,fMessenger(nullptr)
{

}

Repository::Repository(BMessage &message)
	:fIsManuallyAdded(false)
	,fRefreshrate(10)
	,fTransparency(127)
	,fRepository(nullptr)
	,fMessenger(nullptr)
{
	Load(message);
}


Repository::~Repository()
{
	delete fRepository;
	delete fMessenger;
	printf("Repository::~Repository()\n");
}

status_t
Repository::Save(BMessage &message)
{
	fRepository->Save(message);
	message.AddBool("ManuallyAdded", fIsManuallyAdded);

	if (message.ReplaceUInt8("Refreshrate", fRefreshrate) != B_OK) {
		message.AddUInt8("Refreshrate", fRefreshrate);
	}

	if (message.ReplaceUInt8("Transparency", fTransparency) != B_OK) {
		message.AddUInt8("Transparency", fTransparency);
	}
}

status_t
Repository::Load(BMessage &message)
{
	fRepository = new GithubRepository(message);
	printf("Loaded: %s\n", fRepository->Name().String());
	fIsManuallyAdded = message.GetBool("ManuallyAdded", false);
	fTransparency = message.GetUInt8("Transparency", 127);
	fRefreshrate = message.GetUInt8("Refreshrate", 10);
}

void
Repository::SetIsManuallyAdded(bool value)
{
	fIsManuallyAdded = value;
}

void
Repository::SetTarget(BHandler *handler)
{
	delete fMessenger;
	fMessenger = new BMessenger(handler);
}

void
Repository::SetRepository(GithubRepository *repository)
{
	delete fRepository;
	fRepository = repository;
	printf("SetRepository\n");
}

void
Repository::ReloadSavedData()
{
	if (BMessage *message = RepositoryManager(nullptr).RepositoryLoadMessage(Name())) {
		Load(*message);
	}
}

BString
Repository::Name() const
{
	return fRepository->Name();
}

BString
Repository::Owner() const
{
	return fRepository->Owner();
}

BString
Repository::Description() const
{
	return fRepository->Description();
}

BString
Repository::Url() const
{
	return fRepository->Url();
}

BString
Repository::Id() const
{
	return fRepository->Id();
}

int
Repository::SortOrder()
{
	if (IsFork()) {
		return 2;
	} else if (IsPrivate()) {
		return 1;
	}
	return 0;
}

void
Repository::NotifyUpdates()
{
	if (fMessenger == NULL) {
		return;
	}

	BMessage message(RepositoryAction::SettingsChanged);
	fMessenger->SendMessage(&message);
}

bool
Repository::IsManuallyAdded() const
{
	return fIsManuallyAdded;
}

bool
Repository::IsFork() const
{
	return fRepository->IsFork();
}

bool
Repository::IsPrivate() const
{
	return fRepository->IsPrivate();
}

void
Repository::SetTransparency(uint8 value)
{
	if (fTransparency != value) {
		fTransparency = value;
		NotifyUpdates();
	}
}

void
Repository::SetRefreshRate(uint8 value)
{
	if (fRefreshrate != value) {
		fRefreshrate = value;
		NotifyUpdates();
	}
}

uint8
Repository::Transparency() const
{
	return fTransparency;
}

uint8
Repository::RefreshRate() const
{
	return fRefreshrate;
}

