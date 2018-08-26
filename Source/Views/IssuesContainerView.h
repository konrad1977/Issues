/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef ISSUESCONTAINERVIEW_H
#define ISSUESCONTAINERVIEW_H


#include <SupportDefs.h>
#include <interface/GroupView.h>
#include <support/String.h>

class BMessageRunner;
class BDragger;
class BListView;
class GithubClient;
class IssuesContainerView : public BGroupView {
public:
	
	IssuesContainerView(const char *repositoryName);
	IssuesContainerView(BMessage *archive);
  	~IssuesContainerView();

  	static BArchivable*	Instantiate(BMessage* archive);  	
	virtual status_t	Archive(BMessage* into, bool deep = true) const;
			status_t	SaveState(BMessage* into, bool deep = true) const;
			
  	virtual void MessageReceived(BMessage *message);
  	virtual void AttachedToWindow();
						
private:
	GithubClient *Client();
	BListView 	 *ListView();
			
	static int32 DownloadFunc(void *cookie);
			void StartAutoUpdater();
			void RequestIssues();
			void SpawnDonwloadThread();		
			void StopDownloadThread();
			
			void HandleListInvoke(BMessage *message);
			void HandleParse(BMessage *message);
			void SetupViews(bool isReplicant);	

	GithubClient 		*fGithubClient; 
	BListView			*fListView;
	BScrollView 		*fScrollView;
	BDragger			*fDragger;
	BMessageRunner		*fAutoUpdateRunner;
	
	BString	 			fRepositoryName;
	thread_id			fThreadId;
	bool 				fIsReplicant;
};


#endif // _H
