/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef GITHUBCLIENT_H
#define GITHUBCLIENT_H


#include <SupportDefs.h>
#include <String.h>
#include <HttpHeaders.h>

class NetRequester;
class GithubClient {
public:
	GithubClient(const char *token, BHandler *handler);
	~GithubClient();
	
	void RequestProjects();
	void RequestIssuesForRepository(int32 id);
	
private:
		void RunRequest(const char *urlStr, NetRequester *requester);
	
	BHttpHeaders 	fRequestHeaders;
	BString 		fToken;
	BHandler 		*fHandler;
};

#endif // _H
