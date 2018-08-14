/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "GithubProject.h"

GithubProject::GithubProject(BMessage message) 
{
	double tempId;
	message.FindDouble("id", &tempId);
	id = int32(tempId);
	
	message.FindString("name", &name);
	message.FindString("description", &description);
	message.FindBool("fork", &fIsFork);
	message.FindBool("private", &fIsPrivate);
	message.FindString("url", &url);
}

GithubProject::~GithubProject()
{

}

bool
GithubProject::IsFork() const 
{
	return fIsFork;
}

bool
GithubProject::IsPrivate() const 
{
	return fIsPrivate;
}

