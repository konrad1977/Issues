/*
 * Copyright 2018 Mikael Konradsson <mikael.konradson@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "Repository.h"
#include "GithubRepository.h"

Repository::Repository()
	:fRepository(nullptr)
	,fIsManuallyAdded(false)
{

}

Repository::Repository(BMessage &message)
	:fRepository(nullptr)
	,fIsManuallyAdded(false)
{
	Load(message);
}


Repository::~Repository()
{
	delete fRepository;
}

status_t
Repository::Save(BMessage &message)
{
	fRepository->Save(message);
	message.AddBool("ManuallyAdded", fIsManuallyAdded);
}

status_t
Repository::Load(BMessage &message)
{
	fRepository = new GithubRepository(message);
	fIsManuallyAdded = message.FindBool("ManuallyAdded");
}

void
Repository::SetIsManuallyAdded(bool value)
{
	fIsManuallyAdded = value;
}

void
Repository::SetRepository(GithubRepository *repository)
{
	delete fRepository;
	fRepository = repository;
}

BString&
Repository::Name() const
{
	return fRepository->name;
}

BString&
Repository::Owner() const
{
	return fRepository->owner;
}

BString&
Repository::Description() const
{
	return fRepository->description;
}

BString&
Repository::Url() const
{
	return fRepository->url;
}

BString&
Repository::Id() const
{
	return fRepository->id;
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

uint8
Repository::Transparency() const
{
	return 127;
}

uint8
Repository::RefreshRate() const
{
	return 10;
}

