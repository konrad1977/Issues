/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef GITHUBREPOSITORYWINDOW_H
#define GITHUBREPOSITORYWINDOW_H


#include <SupportDefs.h>
#include <interface/Window.h>
#include "RepositoryTypeItem.h"

class FilterView;
class BMenuBar;
class BMenuItem;
class BOutlineListView;
class AddRepositoryWindow;
class GithubClient;
class GithubTokenWindow;
class GithubRepositoryWindow : public BWindow {
public:
	GithubRepositoryWindow();
	~GithubRepositoryWindow();

	virtual void MessageReceived(BMessage *message);


private:
			void ParseData(BMessage *message);
			void HandleFilterMessage(BMessage *message);
			void SetupViews();
			void RequestRepositories();
			void SpawnDownloadThread();
			
			void BuildRepositoryList(BList *repositories);
			void PopuplateListView(RepositoryType type, BList *list);
			void ClearRepositories();
			
	static int SortRepositoriesByName(const void *first, const void *second);
	static int SortRepositoriesByType(const void *first, const void *second);
	
	static int32 DownloadRepositories(void *cookie);
	
	GithubTokenWindow 	*fGithubTokenWindow;
	GithubClient 		*fGithubClient;
	AddRepositoryWindow	*fAddRepositoryWindow;
	BOutlineListView 	*fRepositoryListView;
	BMenuBar 			*fMenuBar;
	thread_id			fDownloadThread;
	
	BList 				*fPrivateRepositories;
	BList				*fForkedRepositories;
	BList				*fPublicRepositories;
	FilterView			*fFilterView;
};


#endif // _H
