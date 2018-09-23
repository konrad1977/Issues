/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
 
#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <SupportDefs.h>
#include <support/String.h>

class BDataIO;
class FileDownloader {
public:
	FileDownloader(const BString &url);
	~FileDownloader();
	
	status_t Download(BDataIO* stream);
private:
	BString fUrl;
};

#endif // _H