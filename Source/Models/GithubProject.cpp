/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "GithubProject.h"

GithubProject::GithubProject(BMessage message) 
{
	message.FindString("name", &name);
	double tempId;
	message.FindDouble("id", &tempId);
	id = int32(tempId);
	
}

GithubProject::~GithubProject()
{

}

