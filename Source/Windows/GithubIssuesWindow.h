/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef GITHUBISSUES_WINDOW_H
#define GITHUBISSUES_WINDOW_H

#include <SupportDefs.h>
#include <interface/Window.h>

class IssuesContainerView;
class GithubRepository;
class GithubIssuesWindow : public BWindow {
public:
	GithubIssuesWindow(GithubRepository *repository);
	~GithubIssuesWindow();		
	
	virtual void MessageReceived(BMessage *message);
	
private:
			void SetupViews();
			
	GithubRepository 	*fRepository;
	IssuesContainerView	*fIssuesContainerView;
};

#endif // _H
