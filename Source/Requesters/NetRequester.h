/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
*/
 
#ifndef NETREQUESTER_H
#define NETREQUESTER_H


#include <SupportDefs.h>
#include <UrlProtocolListener.h>
#include <String.h>

class BHandler;
class NetRequester: public BUrlProtocolListener {
public:
	NetRequester(BHandler *handler, const char *messageName);
	virtual ~NetRequester();
	virtual	void ResponseStarted(BUrlRequest* caller);
	virtual	void DataReceived(BUrlRequest* caller, const char* data, off_t position, ssize_t size);
	virtual	void RequestCompleted(BUrlRequest* caller, bool success);		
	
private:
			void HandleData(BString data); 
	BHandler 			*fHandler;
	BMallocIO			fResponseData;
	const char *		fMessageName;
};

#endif // _H
