/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "App.h"
#include "RepositoryWindow.h"

#include <posix/stdio.h>

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

int main() {
	App app;
	app.Run();
	return 0;
}
