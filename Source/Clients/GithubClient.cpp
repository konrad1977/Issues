/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "GithubClient.h"
#include "NetRequester.h"
#include "GraphQLBuilder.h"
#include "SettingsManager.h"

#include "Constants.h"

#include <Messenger.h>
#include <Handler.h>
#include <Locker.h>
#include <UrlProtocolRoster.h>
#include <UrlRequest.h>
#include <HttpRequest.h>

#include <posix/string.h>
#include <posix/stdlib.h>

GithubClient::GithubClient(BHandler *handler)
	:fHandler(handler)
	,fMessenger(nullptr)
{
	SetTarget(handler);
	LoadToken();
}

GithubClient::~GithubClient()
{
	delete fMessenger;
}

void
GithubClient::SetTarget(BHandler *handler)
{
	delete fMessenger;
	fMessenger = new BMessenger(handler);
}

void
GithubClient::SaveToken(const char *token)
{
	BMessage message;
	message.AddString("Token", BString(token));
	SettingsManager manager(SettingsManagerType::GithubToken);
	manager.SaveSettings(message);
	LoadToken();
}

void
GithubClient::LoadToken()
{
	BMessage message;
	SettingsManager manager(SettingsManagerType::GithubToken);
	manager.LoadSettings(message);
	if (message.FindString("Token", &fToken) == B_OK) {
		InitHeaders();
		BMessage msg(kTokenLoadedMessage);
		fMessenger->SendMessage(&msg);
	} else {
		BMessage msg(kNoTokenMessage);
		fMessenger->SendMessage(&msg);
	}
}

void
GithubClient::InitHeaders()
{
	BString token;
	token << "Bearer " << fToken;

	fRequestHeaders = BHttpHeaders();
	fRequestHeaders.AddHeader("Authorization", token.String());
	fRequestHeaders.AddHeader("Content-Type", "application/json");
	fRequestHeaders.AddHeader("Accept", "application/vnd.github.inertia-preview+json");
}

void
GithubClient::RequestRepository(const char *repository, const char *owner)
{
	NetRequester requester(fHandler, "Repository");

	GraphQLBuilder builder;
	BString query = builder
		//.AddNode("viewer")
		.AddNode("repository(name:\\\"%s\\\" owner:\\\"%s\\\")", repository, owner)
		.AddNode("name url description id owner")
		.AddNode("login")
		.Query();

	RunRequest(&requester, query);
}

void
GithubClient::RequestCommitsForRepository(const char *repository, const char *owner)
{
	NetRequester requester(fHandler, "Commits");

	GraphQLBuilder builder;
	BString query = builder
		.AddNode("repository(name:\\\"%s\\\" owner:\\\"%s\\\")", repository, owner)
		.AddNode("ref(qualifiedName:\\\"%s\\\")", "master")
		.AddNode("target")
		.AddNode("... on Commit")
		.AddNode("history(first:5)")
		.AddNode("nodes")
		.AddNode("messageHeadline message url author { name avatarUrl }")
		.Query();

	RunRequest(&requester, query);
}

void
GithubClient::RequestIssuesForRepository(const char *repository, const char *owner)
{
	if (repository == nullptr || owner == nullptr) {
		return;
	}

	NetRequester requester(fHandler, "Issues");

	GraphQLBuilder builder;
	BString query = builder
		.AddNode("repository(name:\\\"%s\\\" owner:\\\"%s\\\")", repository, owner)
		.AddNode("name")
		.AddValue("issues(first:10 states:OPEN orderBy: { field: CREATED_AT direction: DESC })")
		.AddNode("nodes")
		.AddNode("url title body author { login avatarUrl }")
		.Query();

	RunRequest(&requester, query);
}

void
GithubClient::RequestProjects()
{
	NetRequester requester(fHandler, "UserRepositories");
	GraphQLBuilder builder;
	BString query = builder
		.AddNode("viewer")
		.AddNode("repositories(first:100)")
		.AddNode("nodes")
			.AddNode("name")
			.AddValue("id")
			.AddValue("url")
			.AddValue("description")
			.AddValue("isFork")
			.AddValue("isPrivate")
			.AddValue("owner { login }")
			.AddValue("parent { owner { login } }")
		.Query();
	RunRequest(&requester, query);
}

void
GithubClient::RunRequest(NetRequester *requester, BString body) {

	BUrl url = BUrl("https://api.github.com/graphql");
	BHttpRequest* request = dynamic_cast<BHttpRequest*>(BUrlProtocolRoster::MakeRequest(url, requester));
	request->SetMethod(B_HTTP_POST);
	request->SetHeaders(fRequestHeaders);
	BMemoryIO *io = new BMemoryIO(body.String(), body.Length());
	request->AdoptInputData(io, body.Length());

	thread_id thread = request->Run();
	wait_for_thread(thread, nullptr);
	delete request;
}

