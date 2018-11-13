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
	,fRepository(nullptr)
	,fSettings(nullptr)
{
	fSettings = new Settings();
}

Repository::Repository(BMessage &message)
	:fIsManuallyAdded(false)
	,fRepository(nullptr)
	,fSettings(nullptr)
{
	Load(message);
}


Repository::~Repository()
{
	delete fRepository;
	delete fSettings;
}

status_t
Repository::Save(BMessage &message)
{
	fRepository->Save(message);

	fSettings->Save(message);
	
	if (message.ReplaceBool("ManuallyAdded", fIsManuallyAdded) != B_OK) {
		message.AddBool("ManuallyAdded", fIsManuallyAdded);
	}

	return B_OK;
}

status_t
Repository::Load(BMessage &message)
{
	if(fRepository == nullptr) {
		fRepository = new GithubRepository(message);
	} else {
		fRepository->Load(message);
	}
	
	if (fSettings == nullptr) {
		fSettings = new Settings(message);	
	} else {
		fSettings->Load(message);
	}
	
	fIsManuallyAdded = message.GetBool("ManuallyAdded", false);
	return B_OK;
}

void
Repository::SetIsManuallyAdded(bool value)
{
	fIsManuallyAdded = value;
}

void
Repository::SetTarget(BHandler *handler)
{
	printf("SetTarget(%s)\n", handler->Name());
	fSettings->SetTarget(handler);
}

void
Repository::SetRepository(GithubRepository *repository)
{
	if (fRepository != repository) {
		delete fRepository;
		fRepository = repository;
	}
}

void
Repository::ReloadSavedData()
{
	if (BMessage *message = RepositoryManager(nullptr).RepositoryMessage(Name())) {
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

Settings* 
Repository::CurrentSettings() const
{
	return fSettings;
}
