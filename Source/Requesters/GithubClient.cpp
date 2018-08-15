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
	fToken << "Bearer " << token;

	fRequestHeaders = BHttpHeaders();
	fRequestHeaders.AddHeader("Authorization", fToken.String());
	fRequestHeaders.AddHeader("Content-Type", "application/json");
	fRequestHeaders.AddHeader("Accept", "application/vnd.github.inertia-preview+json");
}

GithubClient::~GithubClient()
{

}

BString 
GithubClient::CreateQuery(const BString &query) const
{
	BString str("{\"query\" : \"query { viewer {");
	str << query << "}}\"}";
	return str;
}

void 
GithubClient::RequestIssuesForRepository(BString name)
{
	BString query(CreateQuery("repositories(first:1) { nodes { name url description id }}"));
	
	NetRequester requester(fHandler, "Issues");
	RunRequest("https://api.github.com/graphql", &requester, query);
}

void 
GithubClient::RequestProjects()
{
	const char *requestUrl = "https://api.github.com/graphql";
	NetRequester requester(fHandler, "GithubRepositories");
	RunRequest(requestUrl, &requester, CreateQuery("repositories(first:100) { nodes { name url description id }}"));
}

void
GithubClient::RunRequest(const char *urlStr, NetRequester *requester, BString body) {

	printf("Body: %s\n", body.String());
	
	BUrl url = BUrl(urlStr);	
	BHttpRequest* request = dynamic_cast<BHttpRequest*>(BUrlProtocolRoster::MakeRequest(url, requester));
	request->SetMethod(B_HTTP_POST);
	request->SetHeaders(fRequestHeaders);
	BMemoryIO *io = new BMemoryIO(body.String(), body.Length());
	request->AdoptInputData(io, body.Length());
	
	thread_id thread = request->Run();
	wait_for_thread(thread, NULL);
	delete request;
}		

