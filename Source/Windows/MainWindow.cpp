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
		"Tesla har nyligen anställt en Volvodesigner vid namn Ian Kettle. Som var hjärnan bakom designen på Volvo XC40. Han började hos Tesla"
	);
	
//	textView->SetText("Mikael testar med en liten kortare mening. Men nu skriver vi lite mer för att se vad som verkligen händer!");
	textView->SetViewColor(255,255,255);
	AddChild(textView);
}

MainWindow::~MainWindow()
{
	
}