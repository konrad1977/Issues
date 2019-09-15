/*
 * Copyright 2018, Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef SETTINGS_H
#define SETTINGS_H

#include <SupportDefs.h>
#include <String.h>
#include <Message.h>

enum SettingsAction {
	Updated = 'rchs'
};

class BHandler;
class BMessenger;
class Settings {
public:

	Settings();
	Settings(BMessage message);
	~Settings();

	void SetTarget(BHandler *handler);

	status_t Load(BMessage &message);
	status_t Save(BMessage &message);

	void SetShowTitle(bool value);
	void SetShowAuthorName(bool value);
	void SetShowAuthorAvatar(bool value);
	void SetHideDescription(bool value);

	void SetTransparency(uint8 value);
	void SetRefreshRate(uint8 value);

	uint8 Transparency() const;
	uint8 RefreshRate() const;

	bool ShowTitle() const;
	bool ShowAuthorName() const;
	bool ShowAuthorAvatar() const;
	bool HideDescription() const;

private:

	void NotifyUpdates();

	bool				fShowTitle;
	bool				fShowAuthorName;
	bool				fShowAuthorAvatar;
	bool				fHideDescription;
	uint8				fRefreshrate;
	uint8 				fTransparency;
	BMessenger			*fMessenger;
};


#endif // _H
