/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef CONTAINER_WINDOW_H
#define CONTAINER_WINDOW_H

#include <SupportDefs.h>
#include <interface/Window.h>

class BMenuBar;
class RepositoryManager;
class SettingsWindow;
class ContainerView;
class ContainerModel;
class ContainerWindow : public BWindow {
public:
	ContainerWindow(ContainerModel *model, RepositoryManager *manager);
	~ContainerWindow();

	virtual void MessageReceived(BMessage *message);
	virtual void Quit();

private:
			void SetupViews();
			void BuildMenu();

	SettingsWindow *CurrentSettingsWindow();

	ContainerView		*fContainerView;
	ContainerModel		*fContainer;
	SettingsWindow		*fSettingsWindow;
	RepositoryManager 	*fRepositoryManager;
	BMenuBar			*fMenuBar;

	enum Actions {
		SHOW_SETTINGS_WINDOW 	= 'shos',
		QUIT					= 'quit'
	};
};

#endif // _H
