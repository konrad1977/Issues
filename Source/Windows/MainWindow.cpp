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
		"Tesla har nyligen anställt en Volvodesigner vid namn Ian Kettle som var hjärnan bakom designen på Volvo XC40. Han började hos Tesla som en senior designer för några veckor sedan enligt Auto Express och rapporterar till chefsdesignern Frank Von Holzhausen."
		"Tesla har valt att inte kommentera men man kan se på Kettles LinkedIn profil att han lämnade Volvo i februari där han jobbat i sex år först i Göteborg och sedan i Los Angeles."
		"Det finns än så länge ingen indikation på vad han kommer att börja jobba på hos Tesla men det finns flera olika kandidater. Det kan möjligtvis vara så att han kommer jobba på en färdig version av Teslas Roadster eller projekt såsom Model Y. Möjligheten finns även att han jobbar på en omgjord version av Model S och Model X inför 2021. "
	);
	textView->SetViewColor(255,255,255);
	AddChild(textView);
}

MainWindow::~MainWindow()
{
	
}