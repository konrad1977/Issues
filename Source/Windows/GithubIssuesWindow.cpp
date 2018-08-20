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
	:BWindow(BRect(0,0,10,10), "Issues", B_FLOATING_WINDOW, B_AUTO_UPDATE_SIZE_LIMITS)
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
GithubIssuesWindow::SetupViews()
{
	SetLayout(new BGroupLayout(B_VERTICAL));
	fIssuesContainerView = new IssuesContainerView(fRepository->name.String());
	AddChild(fIssuesContainerView);
}

