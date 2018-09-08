/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "ContainerWindow.h"
#include "GithubRepository.h"
#include "Constants.h"
#include "ContainerView.h"
#include "ContainerModel.h"

#include <interface/MenuBar.h>
#include <interface/MenuItem.h>
#include <interface/StringItem.h>
#include <interface/GroupLayout.h>
#include <interface/LayoutBuilder.h>

#include <posix/stdio.h>

ContainerWindow::ContainerWindow(ContainerModel *container)
	:BWindow(BRect(0,0,1,1), "Container", B_TITLED_WINDOW, B_FRAME_EVENTS | B_AUTO_UPDATE_SIZE_LIMITS)
	,fContainerView(NULL)
	,fContainer(container)
{
	SetTitle(container->Name());
	SetupViews();
	CenterOnScreen();
}

ContainerWindow::~ContainerWindow()
{
	delete fContainer;
}

void
ContainerWindow::SetupViews()
{
	fContainerView = new ContainerView(fContainer);
	fContainerView->SetExplicitMinSize(BSize(380, B_SIZE_UNSET));
	fContainerView->SetExplicitMaxSize(BSize(B_SIZE_UNLIMITED, B_SIZE_UNLIMITED));

	SetLayout(new BGroupLayout(B_VERTICAL));
	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(fContainerView);
}

