/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H


#include <SupportDefs.h>
#include <support/String.h>
#include <Window.h>

class Repository;
class BSlider;
class BCheckBox;
class BMessenger;
class BHandler;
class SettingsWindow : public BWindow {
public:
	SettingsWindow(Repository *repository);
	~SettingsWindow();

			void ShowWithName(const char *name);
	virtual void MessageReceived(BMessage *message);
			void Quit();
			void SetTarget(BHandler *handler);
private:
			void InitLayout();
			void InitSavedValues();
			void UpdateTransparencyLabel(uint8 value);
			void UpdateRefrehLabel(uint8 value);

	BMessenger 			*fMessenger;
	BSlider 			*fTransparencySlider;
	BSlider 			*fRefreshRateSlider;
	BCheckBox			*fShowTitleCheckbox;
	BCheckBox			*fShowAuthorName;
	BCheckBox			*fShowAuthorAvatar;
	BCheckBox			*fHideDescription;
	Repository			*fRepository;
	BString 			fTransparencyLabel;
	BString 			fRefreshLabel;


	enum Action {
		TransparencyChanged 	= 'tcms',
		TransparancyModified	= 'tsmm',
		RefreshrateChanged		= 'rrcm',
		RefreshrateModified		= 'rrmm',
		ShowTitleChanged		= 'shtm',
		ShowAuthorNameChanged	= 'atnm',
		ShowAuthorAvatarChanged	= 'saac',
		HideDescriptionChanged	= 'hidc'
	};
};


#endif // _H
