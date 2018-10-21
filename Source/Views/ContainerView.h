/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef CONTAINERVIEW_H
#define CONTAINERVIEW_H


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
class ContainerView : public BView {
public:

	ContainerView(ContainerModel *model);
	ContainerView(BMessage *archive);
  	~ContainerView();

  	static BArchivable*	Instantiate(BMessage* archive);
	virtual status_t	Archive(BMessage* into, bool deep = true) const;
			status_t	SaveState(BMessage* into, bool deep = true) const;

  	virtual void MessageReceived(BMessage *message);
  	virtual void AttachedToWindow();
			void SetTarget(BHandler *handler);

	ContainerModel *Model() const { return fContainerModel; }

private:
	BListView 	 *ListView();

	static int32 DownloadFunc(void *cookie);
			void StartAutoUpdater();
			void SpawnDownloadThread();
			void StopDownloadThread();

			void SetupTargets();

			void Reisize();

			void StartNetworkMonitoring();
			bool IsConnected();

			void HandleListInvoke(BMessage *message);
			void SetupViews(bool isReplicant);

	BListView			*fListView;
	BScrollView 		*fScrollView;
	BDragger			*fDragger;
	BMessageRunner		*fAutoUpdateRunner;
	BMessenger			*fMessenger;
	ContainerModel		*fContainerModel;

	thread_id			fThreadId;
	bool 				fIsReplicant;
};


#endif // _H
