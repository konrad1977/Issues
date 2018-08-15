/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef MESSAGEFINDER_H
#define MESSAGEFINDER_H


#include <SupportDefs.h>
#include <support/String.h>
#include <app/Message.h>

class MessageFinder {
public:
	MessageFinder() {}
	~MessageFinder() {}
	BMessage FindMessage(BString name, BMessage inMessage);
	
private:

};


#endif // _H
