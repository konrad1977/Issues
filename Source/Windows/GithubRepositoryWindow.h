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
class RepositoryManager;
class GithubTokenWindow;
class GithubRepositoryWindow : public BWindow {
public:
	GithubRepositoryWindow();
	~GithubRepositoryWindow();

	virtual void MessageReceived(BMessage *message);

private:
			void SetCurrentRepositories(BList *list);

			BList *MakeFilter(BString filter);

			BList *MakePrivateRepositories(BList *list) const;
			BList *MakeForkedRepositories(BList *list) const;
			BList *MakePublicRepositories(BList *list) const;

			void ParseData(BMessage *message);

			void HandleUserRepositories(BMessage *message);
			void HandleRepository(BMessage *message);

			void HandleAddRepository(BMessage *message);
			void HandleFilterMessage(BMessage *message);
			void SetupViews();
			void RequestRepositories();
			void SpawnDownloadThread();

			void PopuplateListView(RepositoryType type, BList *list, uint8 total);
			void ClearRepositories();

	static int SortRepositoriesByName(const void *first, const void *second);
	static int SortRepositoriesByType(const void *first, const void *second);

	static int32 DownloadRepositories(void *cookie);

	GithubTokenWindow 	*fGithubTokenWindow;
	GithubClient 		*fGithubClient;
	RepositoryManager	*fRepositoryManager;
	AddRepositoryWindow	*fAddRepositoryWindow;
	BOutlineListView 	*fRepositoryListView;
	BMenuBar 			*fMenuBar;
	thread_id			fDownloadThread;

	BList				*fCurrentRepositories;
	BList				*fCurrentFilter;
	FilterView			*fFilterView;

	uint8 				fPrivateTotal;
	uint8 				fPublicTotal;
	uint8 				fForkedTotal;
};


#endif // _H
