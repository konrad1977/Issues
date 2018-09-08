/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef CONTAINER_WINDOW_H
#define CONTAINER_WINDOW_H

#include <SupportDefs.h>
#include <interface/Window.h>

class ContainerView;
class ContainerModel;
class ContainerWindow : public BWindow {
public:
	ContainerWindow(ContainerModel *model);
	~ContainerWindow();		
private:
			void SetupViews();		
			
	ContainerView	*fContainerView;
	ContainerModel	*fContainer;
};

#endif // _H
