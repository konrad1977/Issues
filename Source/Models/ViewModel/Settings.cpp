/*
 * Copyright 2018, Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "Settings.h"
#include <app/Messenger.h>

#include <stdio.h>

Settings::Settings()
	:fShowTitle(true)
	,fRefreshrate(10)
	,fTransparency(127)
	,fMessenger(nullptr)
{

}

Settings::Settings(BMessage message)
	:fShowTitle(true)
	,fRefreshrate(10)
	,fTransparency(127)
	,fMessenger(nullptr)
{
	Load(message);
}

Settings::~Settings()
{
	delete fMessenger;
}

void 
Settings::SetTarget(BHandler *handler)
{
	delete fMessenger;
	fMessenger = new BMessenger(handler);
}


status_t 
Settings::Load(BMessage &message)
{
	fRefreshrate 	= message.GetUInt8("Refreshrate", 60);
	fTransparency 	= message.GetUInt8("Transparency", 127);	
	fShowTitle 		= message.GetBool("ShowTitle", true);
}

void
Settings::NotifyUpdates()
{
	if (fMessenger == nullptr) {
		return;
	}

	BMessage message(SettingsAction::Updated);
	fMessenger->SendMessage(&message);
	printf("NotifyUpdates\n");
}
	
status_t
Settings::Save(BMessage &message)
{
	if (message.ReplaceUInt8("Refreshrate", RefreshRate()) != B_OK) {
		message.AddUInt8("Refreshrate", RefreshRate());
	}

	if (message.ReplaceUInt8("Transparency", Transparency()) != B_OK) {
		message.AddUInt8("Transparency", Transparency());
	}
	
	if (message.ReplaceUInt8("ShowTitle", ShowTitle()) != B_OK) {
		message.AddBool("ShowTitle", ShowTitle());
	}
	
	return B_OK;
}
	
void 
Settings::SetShowTitle(bool value) 
{
	fShowTitle = value;
	NotifyUpdates();
}

void 
Settings::SetTransparency(uint8 value) 
{
	if (fTransparency != value) {
		fTransparency = value;
		NotifyUpdates();
	}
}

void 
Settings::SetRefreshRate(uint8 value) 
{
	if (fRefreshrate != value) {
		fRefreshrate = value;
		NotifyUpdates();
	}
}

bool
Settings::ShowTitle() const 
{
	return fShowTitle;
}

uint8 
Settings::Transparency() const 
{
	return fTransparency;
}

uint8 
Settings::RefreshRate() const 
{
	return fRefreshrate;
}