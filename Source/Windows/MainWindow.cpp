/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "MainWindow.h"
#include <View.h>
#include "MultiLineStringView.h"

MainWindow::MainWindow(BRect rect)
	:BWindow(rect, "Main", B_DOCUMENT_WINDOW, B_QUIT_ON_WINDOW_CLOSE, 0)
{
	MultiLineStringView *textView =  new MultiLineStringView(Bounds());
	textView->SetText(
		"Bacon ipsum dolor amet short ribs beef ribs meatball tri-tip kielbasa pig tail burgdoggen. "
		"Boudin landjaeger ham hock shank alcatra fatback hamburger jowl, corned beef pancetta leberkas filet mignon chuck. Ham bacon jowl, frankfurter buffalo beef ribs corned beef chicken. "
		"Spare ribs tongue flank beef. Strip steak burgdoggen jowl, chuck t-bone hamburger pork loin. Shoulder fatback buffalo rump beef cupim."
	);
	textView->SetViewColor(255,255,255);
	AddChild(textView);
}

MainWindow::~MainWindow()
{
	
}