/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "App.h"
#include "MainWindow.h"
#include "GithubRepositoryWindow.h"

#include <posix/stdio.h>


const char *kAppSignature = "application/x-vnd.konradsson.Issues";

App::App(void)
	:BApplication(kAppSignature)
	,fGithubRepositoryWindow(NULL)
{		
	fGithubRepositoryWindow = new GithubRepositoryWindow();
	fGithubRepositoryWindow->Show();
	
	
	MainWindow *win = new MainWindow(BRect(30,30, 300, 200));
	win->Show();
}

App::~App() 
{
	
}

int main() {
	App app;
	app.Run();
	return 0; 	
}
