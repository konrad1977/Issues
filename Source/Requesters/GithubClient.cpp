/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "GithubClient.h"
#include "NetRequester.h"

#include <Handler.h>
#include <Locker.h>
#include <UrlProtocolRoster.h>
#include <UrlRequest.h>
#include <HttpRequest.h>

GithubClient::GithubClient(const char *token, BHandler *handler)
	:fHandler(handler)
{
	fToken << "token " << token;

	fRequestHeaders = BHttpHeaders();
	fRequestHeaders.AddHeader("Authorization", fToken.String());
	fRequestHeaders.AddHeader("Content-Type", "application/json");
	fRequestHeaders.AddHeader("Accept", "application/vnd.github.inertia-preview+json");
}

GithubClient::~GithubClient()
{

}

void 
GithubClient::RequestProjects()
{
	const char *requestUrl = "https://api.github.com/user/repos?affiliation=owner";
	NetRequester requester(fHandler, "Projects");
	RunRequest(requestUrl, &requester);
}

void
GithubClient::RunRequest(const char *urlStr, NetRequester *requester) {

	BUrl url = BUrl(urlStr);	
	BHttpRequest* request = dynamic_cast<BHttpRequest*>(BUrlProtocolRoster::MakeRequest(url, requester));
	request->SetHeaders(fRequestHeaders);
	
	thread_id thread = request->Run();
	wait_for_thread(thread, NULL);
	delete request;
}		

