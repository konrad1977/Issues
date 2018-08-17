/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "GithubIssuesWindow.h"
#include "GithubRepository.h"
#include "Constants.h"
#include "IssuesContainerView.h"

#include <interface/MenuBar.h>
#include <interface/MenuItem.h>
#include <interface/StringItem.h>
#include <interface/GroupLayout.h>
#include <interface/LayoutBuilder.h>

#include <posix/stdio.h>

GithubIssuesWindow::GithubIssuesWindow(GithubRepository *repository)
	:BWindow(BRect(30,30,360,400), "Issues", B_FLOATING_WINDOW, 0)
	,fRepository(repository)
	,fIssuesContainerView(NULL)
{
	SetTitle(fRepository->name.String());
	SetupViews();
	CenterOnScreen();
}

GithubIssuesWindow::~GithubIssuesWindow()
{
}

void 
GithubIssuesWindow::MessageReceived(BMessage *message)
{

}

void 
GithubIssuesWindow::SetupViews()
{
	BGroupLayout *layout = new BGroupLayout(B_VERTICAL);
	layout->SetSpacing(0);
	SetLayout(layout);
	
	fIssuesContainerView = new IssuesContainerView(fRepository->name.String());

	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(fIssuesContainerView);
}

