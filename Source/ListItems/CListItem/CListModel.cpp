/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "CListModel.h"

CListModel::CListModel(const GithubIssue issue, Settings *settings)
	:fTitle(issue.title)
	,fBody(issue.body)
	,fUrl(issue.url)
	,fAuthor(issue.author)
	,fAuthorUrl(issue.authorUrl)
	,fSettings(settings)
{

}

CListModel::CListModel(const GithubCommit commit, Settings *settings)
	:fTitle(commit.title)
	,fBody(commit.body)
	,fUrl(commit.url)
	,fAuthor(commit.author)
	,fAuthorUrl(commit.authorUrl)
	,fSettings(settings)
{

}

BString
CListModel::Title() const
{
	return fTitle;
}

BString
CListModel::Body() const
{
	return fBody;
}

BString
CListModel::Url() const
{
	return fUrl;
}

BString
CListModel::Author() const
{
	return fAuthor;
}

BString
CListModel::AuthorUrl() const
{
	return fAuthorUrl;
}

bool
CListModel::ShowAuthorName() const
{
	return fSettings->ShowAuthorName();
}

bool
CListModel::ShowAuthorAvatar() const
{
	return fSettings->ShowAuthorAvatar();
}

bool
CListModel::ShowDescription() const
{
	return fSettings->HideDescription() == false;
}
