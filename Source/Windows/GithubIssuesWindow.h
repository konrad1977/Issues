/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef GITHUBISSUES_WINDOW_H
#define GITHUBISSUES_WINDOW_H

#include <SupportDefs.h>
#include <interface/Window.h>

class ContainerView;
class GithubRepository;
class GithubIssuesWindow : public BWindow {
public:
	GithubIssuesWindow(GithubRepository *repository);
	~GithubIssuesWindow();		
		
private:
			void SetupViews();
			
	GithubRepository 	*fRepository;
	ContainerView	*fContainerView;
};

#endif // _H
