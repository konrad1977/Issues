/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "GithubIssue.h"

GithubIssue::GithubIssue(BMessage message) 
{	
	message.FindString("title", &title);
	message.FindString("body", &body);
}

GithubIssue::~GithubIssue()
{

}
