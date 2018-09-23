/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "CListModel.h"

CListModel::CListModel(const GithubIssue &issue)
	:fTitle(issue.title)
	,fBody(issue.body)
	,fUrl(issue.url)
	,fAuthor(issue.author)
	,fAuthorUrl(issue.authorUrl)
{

}

CListModel::CListModel(const GithubCommit &commit)
	:fTitle(commit.title)
	,fBody(commit.body)
	,fUrl(commit.url)
	,fAuthor(commit.author)
	,fAuthorUrl(commit.authorUrl)
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
