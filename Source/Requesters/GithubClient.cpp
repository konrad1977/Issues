/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "GithubClient.h"
#include "NetRequester.h"
#include "GraphQLBuilder.h"

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

void 
GithubClient::RequestIssuesForRepository(BString name)
{
	const char *requestUrl = "https://api.github.com/graphql";
	
	NetRequester requester(fHandler, "Issues");
	
	GraphQLBuilder builder;
	BString query = builder
		.AddNode("viewer")
		.AddNode("repository(name:\\\"%s\\\")", name)
		.AddNode("issues(first:100)")
		.AddNode("nodes")
		.AddNode("title")
		.Query();

	RunRequest(requestUrl, &requester, query);
}

void 
GithubClient::RequestProjects()
{
	const char *requestUrl = "https://api.github.com/graphql";
	NetRequester requester(fHandler, "GithubRepositories");
	GraphQLBuilder builder;
	BString query = builder
		.AddNode("viewer")
		.AddNode("repositories(first:100)")
		.AddNode("nodes")
		.AddNode("name url description id")
		.Query();

	RunRequest(requestUrl, &requester, query);
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

