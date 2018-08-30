/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef GITHUBREPOSITORYWINDOW_H
#define GITHUBREPOSITORYWINDOW_H


#include <SupportDefs.h>
#include <interface/Window.h>

class BMenuBar;
class BMenuItem;
class BListView;
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
			void SetupViews();
			void RequestRepositories();
			void SpawnDownloadThread();
	static int32 DownloadRepositories(void *cookie);

	GithubTokenWindow 	*fGithubTokenWindow;
	GithubClient 		*fGithubClient;
	AddRepositoryWindow	*fAddRepositoryWindow;
	BListView 			*fRepositoryListView;
	BMenuBar 			*fMenuBar;
	thread_id			fDownloadThread;
};


#endif // _H
