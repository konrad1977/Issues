/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "NetRequester.h"
#include "Constants.h"
#include <Application.h>

#include <Messenger.h>
#include <Looper.h>
#include <stdio.h>
#include <Json.h>



NetRequester::NetRequester(BHandler *handler, const char *messageName)
	: BUrlProtocolListener()
	,fHandler(handler)
	,fMessageName(messageName)
{

}

NetRequester::~NetRequester()
{

}

void
NetRequester::ResponseStarted(BUrlRequest* caller)
{
}

void
NetRequester::DataReceived(BUrlRequest* caller, const char* data, off_t position, ssize_t size)
{
	fResponseData.Write(data, size);
}

void
NetRequester::HandleData(BString data)
{
	//printf("%s\n", data.String());

	BMessage parsedData;
	BJson parser;
	status_t status = parser.Parse(data, parsedData);

	BMessenger messenger(fHandler);
	BMessage message(kDataReceivedMessage);
	message.AddMessage(fMessageName, &parsedData);
	messenger.SendMessage(&message);
}

void
NetRequester::RequestCompleted(BUrlRequest* caller, bool success)
{
	BString jsonString;
	jsonString.SetTo(static_cast<const char*>(fResponseData.Buffer()), fResponseData.BufferLength());
	HandleData(jsonString);
}
