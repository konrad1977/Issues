/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef CONTAINERMODEL_H
#define CONTAINERMODEL_H

#include <SupportDefs.h>
#include <interface/ListView.h>
#include <app/Message.h>

class ContainerModel {
public:
	virtual status_t Archive(BMessage *message) = 0;

	virtual void MessageReceived(BMessage *message) = 0;
	virtual void RequestData() = 0;

	virtual void SetTarget(BHandler *handler) = 0;

			void SetIsReplicant(bool replicant) { fIsReplicant = replicant; }
			bool IsReplicant() { return fIsReplicant; }

	void SetListView(BListView *listView) 	{ fListView = listView; }
	BListView *ListView() 	{ return fListView; }
private:
	BListView 	*fListView;
	bool		fIsReplicant;

};

#endif // _H
