/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef CLISTMODEL_H
#define CLISTMODEL_H


#include <SupportDefs.h>
#include "GithubIssue.h"
#include "GithubCommit.h"

class CListModel {
public:
	CListModel(const GithubIssue &issue);
	CListModel(const GithubCommit &issue);
	

	BString Title() const;
	BString Body() const;
	BString Url() const;
	BString Author() const;
	BString AuthorUrl() const;
	
private:
	BString fTitle;
	BString fBody;
	BString fUrl;
	BString fAuthor;
	BString fAuthorUrl;
};


#endif // _H
