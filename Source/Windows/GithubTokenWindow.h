/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef GITHUBTOKENWINDOW_H
#define GITHUBTOKENWINDOW_H

#include <SupportDefs.h>
#include <interface/Window.h>

class BTextControl;
class BHandler;
class BMessenger;
class GithubTokenWindow : public BWindow {
public:
	GithubTokenWindow(BHandler *handler);
	~GithubTokenWindow();

	virtual void MessageReceived(BMessage *message);
	virtual bool QuitRequested();
	
private:
			void InitLayout();
	BTextControl *TextControl();
			
	BMessenger		*fMessenger;
	BTextControl 	*fTokenControl;
};

#endif // _H
