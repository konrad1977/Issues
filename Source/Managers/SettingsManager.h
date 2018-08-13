/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef SETTINGSLOADER_H
#define SETTINGSLOADER_H


#include <SupportDefs.h>
#include <Message.h>
#include <Path.h>

class BList;
class BLocker;
class BMessenger;
class SettingsManager {
public:
	SettingsManager();
	~SettingsManager();
	
	void StartMonitoring(BHandler *handler);
	
	status_t LoadSettings(BMessage &message);
	status_t SaveSettings(BMessage message);	
	
private:

			void SaveWithLock(BMessage *message);
	const char*  SavePath() const;

	const char 	*fFileName;	
	BLocker *fLocker;
};

#endif // _H
