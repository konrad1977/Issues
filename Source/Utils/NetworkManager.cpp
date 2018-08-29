/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "NetworkManager.h"
#include <Handler.h>
#include <Messenger.h>

#include <net/NetworkRoster.h>
#include <net/NetworkInterface.h>

NetworkManager::NetworkManager(BHandler *handler)
	:fMessenger(NULL)
	,fIsConnected(false)
	,fThreadId(-1)
{
	fMessenger = new BMessenger(handler);
}

NetworkManager::~NetworkManager()
{
	delete fMessenger;
}

void 
NetworkManager::RunConnectionCheck() 
{
	StartConnectionThread();
}

void
NetworkManager::CheckInternetConnection()
{
	if (IsConnectedToInternet() == true) {
		BMessage message(IS_CONNECTED);
		fMessenger->SendMessage(&message);
	} else {
		BMessage message(IS_CONNECTED);
		fMessenger->SendMessage(&message);
	}	
}

int32 
NetworkManager::CheckConnectionThread(void *cookie)
{
	NetworkManager *manager = static_cast<NetworkManager*>(cookie);
	manager->CheckInternetConnection();
}

void 
NetworkManager::StartConnectionThread()
{
	StopConnectionThread();

	fThreadId = spawn_thread(&CheckConnectionThread, "Internet connection check", B_NORMAL_PRIORITY, this);
	if (fThreadId >= 0)
		resume_thread(fThreadId);
}

void 
NetworkManager::StopConnectionThread()
{
	if (fThreadId == -1) {
		return;
	}
	wait_for_thread(fThreadId, NULL);
	fThreadId = -1;
}
	
bool
NetworkManager::IsConnectedToInternet()  
{	
	BNetworkRoster& roster = BNetworkRoster::Default();
	BNetworkInterface interface;
	
	uint32 cookie = 0;
	while (roster.GetNextInterface(&cookie, interface) == B_OK) {
		const uint32 flags = interface.Flags();
		if ((flags & IFF_LOOPBACK) == 0) {
			if ((flags & (IFF_UP | IFF_LINK)) == (IFF_UP | IFF_LINK)) {
				return true;
			}
		}
	}
	return false;
}

