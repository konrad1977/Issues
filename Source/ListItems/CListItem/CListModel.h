/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef CLISTMODEL_H
#define CLISTMODEL_H


#include <SupportDefs.h>
#include "GithubIssue.h"
#include "GithubCommit.h"
#include "Settings.h"

class CListModel {
public:

	CListModel(const GithubIssue issue, Settings* settings);
	CListModel(const GithubCommit issue, Settings* settings);
	
	BString Title() const;
	BString Body() const;
	BString Url() const;
	BString Author() const;
	BString AuthorUrl() const;
	
	Settings* CurrentSettings() const; 
	
private:
	BString fTitle;
	BString fBody;
	BString fUrl;
	BString fAuthor;
	BString fAuthorUrl;
	Settings *fSettings;
};


#endif // _H
