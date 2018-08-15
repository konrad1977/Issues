/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "MessageFinder.h"
	
BMessage 
MessageFinder::FindMessage(BString name, BMessage inMessage)
{
	char *messageName;
	uint32 type;
	int32 count;
	
	for (int32 i = 0; inMessage.GetInfo(B_MESSAGE_TYPE, i, &messageName, &type, &count) == B_OK; i++) {
		BMessage subMessage;
		if (inMessage.FindMessage(messageName, &subMessage) ==B_OK) {
			BString currentName(messageName);
			return name == messageName ? subMessage : FindMessage(name, subMessage);
		}
	}
	return inMessage;	
}
