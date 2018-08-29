/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef REPOSITORYTITLEVIEW_H
#define REPOSITORYTITLEVIEW_H


#include <SupportDefs.h>
#include <interface/View.h>

#include "GithubRepository.h"
#include "MultiLineTextDrawer.h"

class ColorManager;
class RepositoryTitleView : public BView {
public:
	RepositoryTitleView(bool isReplicant);
	~RepositoryTitleView();

	virtual BSize MinSize();
	virtual BSize MaxSize();
	
	virtual void Draw(BRect rect);
			void SetRepository(GithubRepository *repository);
	
private:
			void DrawBackground();
			void DrawRepository();
			
	GithubRepository 		*fRepository;
	MultiLineTextDrawer 	*fDrawer;
	ColorManager			*fColorManager;
	float 					fHeight;
	bool 					fIsReplicant;
};


#endif // _H
