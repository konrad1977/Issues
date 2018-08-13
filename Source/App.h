/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef APP_H
#define APP_H

#include <SupportDefs.h>
#include <Application.h>

class GithubClient;
class SettingsManager;
class GithubTokenWindow;
class App: public BApplication {
public:
	App(void);		
	~App();
	
	virtual void MessageReceived(BMessage *message);
			void ParseData(BMessage *message);
	
private:	
			void LoadToken(BString &token);
			void SaveToken(BMessage *message);
			void ShowTokenWindow();
	
	GithubTokenWindow *fGithubTokenWindow;
	GithubClient *fGithubClient;
};

#endif // _H
