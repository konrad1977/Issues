/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef ISSUESCONTAINERVIEW_H
#define ISSUESCONTAINERVIEW_H


#include <SupportDefs.h>
#include <interface/View.h>
#include <support/String.h>
#include "ContainerModel.h"

class BMessageRunner;
class BDragger;
class BListView;
class GithubRepository;
class GithubClient;
class RepositoryTitleView;
class IssuesContainerView : public BView {
public:

	IssuesContainerView(ContainerModel *model);
	IssuesContainerView(BMessage *archive);
  	~IssuesContainerView();

  	static BArchivable*	Instantiate(BMessage* archive);
	virtual status_t	Archive(BMessage* into, bool deep = true) const;
			status_t	SaveState(BMessage* into, bool deep = true) const;

  	virtual void MessageReceived(BMessage *message);
  	virtual void AttachedToWindow();

	ContainerModel *Model() const { return fContainerModel; }

private:
	BListView 	 *ListView();

	static int32 DownloadFunc(void *cookie);
			void StartAutoUpdater();
			void SpawnDonwloadThread();
			void StopDownloadThread();

			void Reisize();

			void HandleListInvoke(BMessage *message);

			void HandleParse(BMessage *message);
			void AddIssues(BMessage *message);
			void AddRepository(BMessage *message);

			void SetupViews(bool isReplicant);

	BListView			*fListView;
	BScrollView 		*fScrollView;
	BDragger			*fDragger;
	BMessageRunner		*fAutoUpdateRunner;

	thread_id			fThreadId;
	bool 				fIsReplicant;

	BString	 			fRepository;
	BString	 			fOwner;
	ContainerModel		*fContainerModel;
};


#endif // _H
