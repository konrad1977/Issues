/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "SettingsWindow.h"
#include "Repository.h"
#include "Constants.h"

#include <Catalog.h>
#include <Messenger.h>
#include <Box.h>
#include <Slider.h>
#include <Button.h>
#include <LayoutBuilder.h>
#include <GroupLayout.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "SettingsWindow"

SettingsWindow::SettingsWindow(Repository *repository)
	:BWindow(BRect(0,0, 480, 320), B_TRANSLATE("Settings"), B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS | B_CLOSE_ON_ESCAPE | B_NOT_ZOOMABLE | B_AUTO_UPDATE_SIZE_LIMITS )
	,fMessenger(nullptr)
	,fTransparencySlider(nullptr)
	,fRefreshRateSlider(nullptr)
	,fRepository(repository)
{
	InitLayout();
	InitSavedValues();
	CenterOnScreen();
}

SettingsWindow::~SettingsWindow()
{
	delete fMessenger;
}

void
SettingsWindow::ShowWithName(const char *name)
{
	BString str;
	str << name << " " << B_TRANSLATE("Settings");
	SetTitle(str.String());
	Show();
}

void
SettingsWindow::SetTarget(BHandler *handler)
{
	delete fMessenger;
	fMessenger = new BMessenger(handler);
}

void
SettingsWindow::Quit()
{
	BMessage message(kSettingsWindowQuit);
	if (fMessenger && fMessenger->IsValid()) {
		fMessenger->SendMessage(&message);
	}
	BWindow::Quit();
}

void
SettingsWindow::InitSavedValues()
{
	const uint8 transparency = fRepository->Transparency();
	const uint8 refreshRate = fRepository->RefreshRate();

	fTransparencySlider->SetValue(static_cast<int32>(transparency));
	fRefreshRateSlider->SetValue(static_cast<int32>(refreshRate));
	UpdateTransparencyLabel(transparency);
	UpdateRefrehLabel(refreshRate);
}

void
SettingsWindow::UpdateTransparencyLabel(uint8 value)
{
	BString str = fTransparencyLabel;
	str << " " << value;
	fTransparencySlider->SetLabel(str.String());
}

void
SettingsWindow::UpdateRefrehLabel(uint8 value)
{
	BString str = fRefreshLabel;
	str << " " << value;
	fRefreshRateSlider->SetLabel(str.String());
}

void
SettingsWindow::InitLayout()
{
	BGroupLayout *groupLayout = new BGroupLayout(B_VERTICAL);
	SetLayout(groupLayout);

	fTransparencyLabel = B_TRANSLATE("Replicant transparency");

	fTransparencySlider = new BSlider("Transparency", fTransparencyLabel.String(), new BMessage(Action::TransparencyChanged), 0, 255, B_HORIZONTAL);
	fTransparencySlider->SetModificationMessage(new BMessage(Action::TransparancyModified));

	fRefreshLabel = B_TRANSLATE("Refresh interval (in seconds)");

	fRefreshRateSlider = new BSlider("Refresh", fRefreshLabel.String(), new BMessage(Action::RefreshrateChanged), 1, 300, B_HORIZONTAL);
	fRefreshRateSlider->SetModificationMessage(new BMessage(Action::RefreshrateModified));

	BGroupLayout *transparencyGroup = BLayoutBuilder::Group<>(B_VERTICAL)
		.SetInsets(15,10)
		.Add(fTransparencySlider)
		.Add(fRefreshRateSlider);


	BGroupLayout *settingsGroup = BLayoutBuilder::Group<>(B_VERTICAL)
		.SetInsets(5,10)
		.Add(transparencyGroup->View());

	AddChild(settingsGroup->View());
}

void
SettingsWindow::MessageReceived(BMessage *message)
{
	switch (message->what) {

		case Action::TransparencyChanged: {
			uint8 newValue = static_cast<uint8>(fTransparencySlider->Value());
			fRepository->SetTransparency(newValue);
			break;
		}

		case Action::TransparancyModified: {
			uint8 newValue = static_cast<uint8>(fTransparencySlider->Value());
			UpdateTransparencyLabel(newValue);
			break;
		}

		case Action::RefreshrateChanged: {
			uint8 newValue = static_cast<uint8>(fRefreshRateSlider->Value());
			fRepository->SetRefreshRate(newValue);
			break;
		}

		case Action::RefreshrateModified: {
			uint8 newValue = static_cast<uint8>(fRefreshRateSlider->Value());
			UpdateRefrehLabel(newValue);
			break;
		}

		default:
			BWindow::MessageReceived(message);
	}
}