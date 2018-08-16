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
class BMessenger;
class GithubClient {
public:
	GithubClient(BHandler *handler);
	~GithubClient();
	
	void RequestProjects();
	void RequestIssuesForRepository(BString name);
	void SaveToken(const char *token);
	
private:
		void LoadToken();
		void InitHeaders();
		void SetTarget(BHandler *handler);
		
		BString CreateQuery(const BString &query) const;
		void RunRequest(const char *urlStr, NetRequester *requester, BString body);
	
	BHttpHeaders 	fRequestHeaders;
	BString 		fToken;
	BHandler 		*fHandler;
	BMessenger		*fMessenger;
};

#endif // _H
