/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef ISSUESCONTAINERVIEW_H
#define ISSUESCONTAINERVIEW_H


#include <SupportDefs.h>
#include <interface/View.h>

class BListView;
class GithubClient;
class IssuesContainerView : public BView {
public:
	
	IssuesContainerView(const char *repositoryName);
  	~IssuesContainerView();
  	
  	virtual void MessageReceived(BMessage *message);
  	virtual void AttachedToWindow();
						
private:

	static int32 DownloadFunc(void *cookie);
			void RequestIssues();
			void SpawnDonwloadThread();		
			void ParseIssueData(BMessage *message);
			void SetupViews();	

	GithubClient 		*fGithubClient; 
	BListView			*fListView;
	char 				*fRepositoryName;
	thread_id			fThreadId;
};


#endif // _H
