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
		"Mikael testar att skriva lite längre text med som innehåller följande bokstäver. "
	);
	textView->SetViewColor(255,255,255);
	AddChild(textView);
}

MainWindow::~MainWindow()
{
	
}