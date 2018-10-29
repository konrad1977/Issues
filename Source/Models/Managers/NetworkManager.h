/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <SupportDefs.h>
#include <OS.h>

const uint32 IS_CONNECTED 	= 'ISCm';
const uint32 NO_CONNECTION 	= 'NOCm'; 

class BHandler;
class BMessenger;
class NetworkManager  {
public:
	NetworkManager(BHandler *handler);
	~NetworkManager();	

	void RunConnectionCheck();
	
private:
			void CheckInternetConnection();
			bool IsConnectedToInternet();
	static int32 CheckConnectionThread(void *cookie);
			void StartConnectionThread();
			void StopConnectionThread();
	
	BMessenger *fMessenger;
	bool 		fIsConnected;
	thread_id	fThreadId;
};


#endif // _H
