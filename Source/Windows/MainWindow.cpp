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
	textView->SetText("Mikael testar med en liten kortare mening. Men nu skriver vi lite mer för att se vad som verkligen händer!");
	textView->SetViewColor(127,25,155);
	textView->SetTextColor(244, 244, 244);
	AddChild(textView);	
}

MainWindow::~MainWindow()
{
	
}