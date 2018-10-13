/*
 * Copyright 2018 Mikael Konradsson <mikael.konradson@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "Repository.h"
#include "GithubRepository.h"

Repository::Repository(GithubRepository *repository)
	:fRepository(repository)
{

}

Repository::~Repository()
{

}

status_t
Repository::Save(BMessage &message)
{

}

status_t
Repository::Load(BMessage &message)
{

}

const BString&
Repository::Name() const
{
	return fRepository->name;
}

const BString&
Repository::Owner() const
{
	return fRepository->owner;
}

const BString&
Repository::Description() const
{
	return fRepository->description;
}

const BString&
Repository::Url() const
{
	return fRepository->url;
}

const BString&
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

