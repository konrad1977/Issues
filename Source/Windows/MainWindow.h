/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef _H
#define _H


#include <SupportDefs.h>
#include <Window.h>

class BView;
class MainWindow : public BWindow {
public:
	MainWindow(BRect rect);
	~MainWindow();
	
private:
	void RunStringTest();
	BView *view;
};


#endif // _H
