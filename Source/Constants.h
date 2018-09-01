/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include <GraphicsDefs.h>

#ifndef MESSAGECONSTANTS_H
#define MESSAGECONSTANTS_H

const rgb_color kLightTextColor = { 245,245,245 };
const rgb_color kDarkTextColor 	= { 40,44,40 	};

const int32 kDataReceivedMessage 	= 'kDRM';
const int32 kWindowQuitMessage 		= 'kWQM';
const int32 kGithubTokenSaveMessage = 'kTSM';
const int32 kNoTokenMessage			= 'kNTM';
const int32 kTokenLoadedMessage		= 'kTLM';
const int32 kAutoUpdateMessage		= 'kAUM';

const int32 kShowAddRepository		= 'kSRm';
const int32 kQuitAddRepository		= 'kQRm';
const int32 kRepositoryAdded		= 'kRAm';

const int32 kListInvokedMessage 		= 'kRLM';
const int32 kIssueListInvokedMessage 	= 'kILM';

#endif // _H
