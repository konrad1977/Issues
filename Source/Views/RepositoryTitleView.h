/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef REPOSITORYTITLEVIEW_H
#define REPOSITORYTITLEVIEW_H


#include <SupportDefs.h>
#include <interface/View.h>

class GithubRepository;
class RepositoryTitleView : public BView {
public:
	RepositoryTitleView();
	~RepositoryTitleView();
	
	virtual	BSize MinSize();
	virtual void Draw(BRect rect);
			void SetRepository(GithubRepository *repository);
	
private:
			void DrawBackground();
			void DrawRepository();
			
	GithubRepository *fRepository;
};


#endif // _H
