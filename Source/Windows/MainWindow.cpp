/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "MainWindow.h"
#include "MultiLineStringView.h"
#include <View.h>
#include <String.h>
#include <stdio.h>

MainWindow::MainWindow(BRect rect)
	:BWindow(rect, "Main", B_DOCUMENT_WINDOW, B_QUIT_ON_WINDOW_CLOSE, 0)
{
	MultiLineStringView *textView =  new MultiLineStringView(Bounds());	

	BFont font(be_plain_font);
	font.SetSize(27.0);
	textView->SetFont(&font);

	textView->SetText(
			"Both the source and the destination of a message must agree upon its format—the command constant and the names and types of data fields."
			"They must also agree on details of the exchange—when the message can be sent, whether it requires a response, what the format of the reply should be, what it means if an expected data item is omitted, and so on."
			"None of this is a problem for messages that are used only within an application; the application developer can keep track of the details. However, protocols must be published for messages that communicate between applications. "
			"You're urged to publish the specifications for all messages your application is willing to accept from outside sources and for all those that it can package for delivery to other applications."
	);
	
	textView->SetAligntment(B_ALIGN_CENTER);
	textView->SetInsets(BSize(30, 0));
	textView->SetViewColor(255,255,255);
	textView->SetTextColor(10, 18, 15);
	AddChild(textView);	
}

MainWindow::~MainWindow()
{
	
}