/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include <GraphicsDefs.h>

#ifndef MESSAGECONSTANTS_H
#define MESSAGECONSTANTS_H

const rgb_color kLightTextColor = { 245,245,245 };
const rgb_color kDarkTextColor 	= { 40,44,40 	};

const rgb_color kForkColor 		= { 193, 188, 194 };
const rgb_color kPrivateColor 	= { 255, 64, 80 };
const rgb_color kPublicColor 	= { 38, 201, 158 };
const rgb_color kCustomColor 	= { 198, 100, 242 };

const int32 kDataReceivedMessage 	= 'kDRM';
const int32 kWindowQuitMessage 		= 'kWQM';
const int32 kGithubTokenSaveMessage = 'kTSM';
const int32 kNoTokenMessage			= 'kNTM';
const int32 kTokenLoadedMessage		= 'kTLM';
const int32 kAutoUpdateMessage		= 'kAUM';

const int32 kContainerRequestResize		= 'kRZs';

const int32 kMenuShowCommitsForRepository	= 'mSCm';
const int32 kMenuShowIssueForRepository	 	= 'mSmm';

const int32 kShowCommitsForRepository	= 'kSCm';
const int32 kShowIssueForRepository		= 'kSmm';

const int32 kShowAddRepository			= 'kSRm';
const int32 kQuitAddRepository			= 'kQRm';
const int32 kRepositoryAdded			= 'kRAm';

const int32 kFilterChangedMessage	= 'kFCm';

const int32 kListSelectionChanged 		= 'klsc';
const int32 kListInvokedMessage 		= 'kRLM';
const int32 kIssueListInvokedMessage 	= 'kILM';


#endif // _H
