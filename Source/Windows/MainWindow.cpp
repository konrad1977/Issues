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
	textView->SetText("Bacon ipsum dolor amet buffalo brisket turkey bacon rump. Flank boudin chicken cow, frankfurter porchetta tail sirloin. Spare ribs sirloin pork, meatloaf tongue short ribs shank ball tip tail corned beef t-bone. Chicken brisket pork loin hamburger, fatback spare ribs buffalo short loin chuck jowl short ribs sirloin ribeye ball tip. Fatback tenderloin turducken meatloaf meatball ball tip doner ham hock salami strip steak frankfurter turkey brisket hamburger. Prosciutto turkey picanha ribeye ham hock t-bone chuck meatball tri-tip. Prosciutto leberkas venison ribeye tenderloin pork belly pig buffalo salami pastrami shank meatball.");
	textView->SetViewColor(127,127,127);
	AddChild(textView);
}

MainWindow::~MainWindow()
{
	
}