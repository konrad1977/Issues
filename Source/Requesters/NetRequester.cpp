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



NetRequester::NetRequester(BHandler *handler, BString caller)
	: BUrlProtocolListener()
	,fCallerName(caller)
{
	fMessenger = new BMessenger(handler);
}

NetRequester::~NetRequester()
{
	delete fMessenger;
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
	printf("%s\n", data.String());

	BMessage parsedData;
	BJson parser;

	if (parser.Parse(data, parsedData) == B_OK) {
		BMessage message(NetRequesterAction::DataReceived);
		message.AddMessage(fCallerName, &parsedData);
		fMessenger->SendMessage(&message);
	}
}

void
NetRequester::RequestCompleted(BUrlRequest* caller, bool success)
{
	BString jsonString;
	jsonString.SetTo(static_cast<const char*>(fResponseData.Buffer()), fResponseData.BufferLength());
	HandleData(jsonString);
}
