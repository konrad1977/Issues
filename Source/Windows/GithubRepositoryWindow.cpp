/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "GithubRepositoryWindow.h"
#include "GithubProject.h"
#include "GithubTokenWindow.h"
#include "GithubClient.h"
#include "SettingsManager.h"
#include "Constants.h"

#include <posix/stdio.h>

GithubRepositoryWindow::GithubRepositoryWindow() 
	:BWindow(BRect(30,30, 300, 400), "Repositories", B_DOCUMENT_WINDOW, 0)
	,fGithubTokenWindow(NULL)
	,fGithubClient(NULL)
{
	CheckForSavedToken();
}

GithubRepositoryWindow::~GithubRepositoryWindow() 
{
	delete fGithubClient;
}

void 
GithubRepositoryWindow::CheckForSavedToken()
{
	BString token;
	LoadToken(token);
	
	if (token.Length() == 0) {
		fGithubTokenWindow = new GithubTokenWindow(this);
		fGithubTokenWindow->Show();
	} else {
		fGithubClient = new GithubClient(token.String(), this);
		fGithubClient->RequestProjects();
	}
}

void 
GithubRepositoryWindow::LoadToken(BString &token)
{
	BMessage message;
	SettingsManager manager;
	manager.LoadSettings(message);
	message.FindString("Token", &token);
}

void 
GithubRepositoryWindow::SaveToken(BMessage *message)
{
	BString token;
	if (message->FindString("Token", &token) == B_OK) {
		SettingsManager manager;
		manager.SaveSettings(*message);
	}
}

void 
GithubRepositoryWindow::ParseData(BMessage *message)
{	
	BMessage symbolMessage;
	if (message->FindMessage("Projects", &symbolMessage) == B_OK) {	
		char *name;
		uint32 type;
		int32 count;
					
		for (int32 i = 0; symbolMessage.GetInfo(B_MESSAGE_TYPE, i, &name, &type, &count) == B_OK; i++) {
			BMessage currentMessage;
			if (symbolMessage.FindMessage(name, &currentMessage) == B_OK) {
				GithubProject *project = new GithubProject(currentMessage);
				printf("%d %s\n", project->id, project->name.String());
			}
		}
	}
}

void
GithubRepositoryWindow::MessageReceived(BMessage *message) {
	switch (message->what) {
		case kDataReceivedMessage: {
			printf("kDataReceivedMessage\n");
			ParseData(message);
			break;
		}
		case kWindowQuitMessage: {
			fGithubTokenWindow = NULL;
			break;
		}
		
		case kGithubTokenSaveMessage: {
			SaveToken(message);
			
			BString token;
			LoadToken(token);
			
			delete fGithubClient;
			fGithubClient = new GithubClient(token.String(), this);
			fGithubClient->RequestProjects();
			break;
		}
		default:
			BWindow::MessageReceived(message);
	}
}

