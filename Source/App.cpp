/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "App.h"
#include "RepositoryWindow.h"

#include <posix/stdio.h>
#include <AboutWindow.h>

const char *kAppSignature = "application/x-vnd.konradsson-Issues";

App::App(void)
	:BApplication(kAppSignature)
	,fRepositoryWindow(nullptr)
{
	fRepositoryWindow = new RepositoryWindow();
	fRepositoryWindow->Show();
}

App::~App()
{

}

void
App::AboutRequested()  {

	BAboutWindow* window = new BAboutWindow("Issues", kAppSignature);
	window->AddCopyright(2019, "Mikael Konradsson");
	window->AddDescription("Simple application for viewing Issues and commits from Github. Can be added to your desktop as replicants.");
	window->AddExtraInfo("Distributed on MIT license terms.");
	window->Show();
}

int main() {
	App app;
	app.Run();
	return 0;
}
