/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "FileDownloader.h"

#include <Url.h>
#include <UrlContext.h>
#include <UrlProtocolListener.h>
#include <UrlProtocolRoster.h>
#include <HttpRequest.h>
#include <HttpHeaders.h>
#include <TranslationUtils.h>

class FileProtocolListener: public BUrlProtocolListener {
public:
	FileProtocolListener()
		:fDownloadIO(NULL) {}
	
	virtual ~FileProtocolListener()	{ } 
	virtual void DataReceived(BUrlRequest* caller, const char* data, off_t position, ssize_t size) 
	{
		if (fDownloadIO != NULL) {
			fDownloadIO->Write(data, size);
		}
	}
	
	void SetDownloadIO(BDataIO* downloadIO) 
	{
		fDownloadIO = downloadIO;
	}

private:
	BDataIO*		fDownloadIO;
};

FileDownloader::FileDownloader(const BString &url)
	:fUrl(url)
{

}

FileDownloader::~FileDownloader()
{

}

status_t 
FileDownloader::Download(BDataIO* stream)
{
	BUrl url(fUrl);
	bool isSecure = url.Protocol() == "https";
	
	FileProtocolListener listener;
	listener.SetDownloadIO(stream);
	
	BHttpRequest request(url, isSecure, "HTTP", &listener);
	request.SetMethod(B_HTTP_GET);
	
	thread_id thread = request.Run();
	wait_for_thread(thread, NULL);

	const BHttpResult& result = dynamic_cast<const BHttpResult&>(request.Result());
	const int32 statusCode = result.StatusCode();

	if (statusCode == 200)
		return B_OK;
	return B_ERROR;
}
