/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef ADDREPOSITORYWINDOW_H
#define ADDREPOSITORYWINDOW_H


#include <SupportDefs.h>
#include <interface/Window.h>

class BMessenger;
class BTextControl;
class AddRepositoryWindow : public BWindow {
public:
	AddRepositoryWindow();
	~AddRepositoryWindow();

	virtual void MessageReceived(BMessage *message);
	virtual bool QuitRequested();
			void SetTarget(BHandler *handler);

private:
			void SetupViews();

	BMessenger 		*fMessenger;
	BTextControl 	*fRepositoryName;
	BTextControl 	*fRepositoryOwner;

};


#endif // _H
