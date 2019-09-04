/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include "SettingsManager.h"
#include "Constants.h"

#include <support/Locker.h>
#include <support/String.h>
#include <support/List.h>

#include <app/Roster.h>
#include <app/Message.h>

#include <storage/Entry.h>
#include <storage/Path.h>
#include <storage/FindDirectory.h>
#include <storage/File.h>
#include <storage/Node.h>
#include <storage/NodeMonitor.h>

#include <string.h>
#include <stdio.h>

SettingsManager::SettingsManager(SettingsManagerType type)
	: fLocker(nullptr)
{
	switch (type) {
		case SettingsManagerType::GithubToken: {
			fFileName = "IssueToken";
			break;
		}
		case SettingsManagerType::SavedData: {
			fFileName = "IssuesSettings";
			break;
		}
	}
	fLocker = new BLocker("SettingsLocker");
}

SettingsManager::~SettingsManager()
{
	delete fLocker;
}

status_t
SettingsManager::StartMonitoring(BHandler *handler)
{
	BPath path;
    status_t status = find_directory(B_USER_SETTINGS_DIRECTORY, &path);
	if (status != B_OK) {
		return status;
    }

    status = path.Append(fFileName.String());
    if (status != B_OK) {
        return status;
    }

	BNode node;
	status = node.SetTo(path.Path());

    if (status != B_OK) {
        return status;
    }

	node_ref ref;
	node.GetNodeRef( &ref );

    status = node.InitCheck();
    if (status != B_OK) {
        return status;
    }

    watch_node(&ref, B_WATCH_ALL, handler );
    return B_OK;
}

void
SettingsManager::SaveWithLock(BMessage *message)
{
	fLocker->Lock();
	SaveSettings(*message);
	fLocker->Unlock();
}

status_t
SettingsManager::SaveSettings(BMessage message)
{
	BPath path;
    status_t status = find_directory(B_USER_SETTINGS_DIRECTORY, &path);
	if (status != B_OK) {
		return status;
    }

    status = path.Append(fFileName.String());
    if (status != B_OK) {
        return status;
    }

	BFile file(path.Path(), B_WRITE_ONLY | B_ERASE_FILE | B_CREATE_FILE);

    status = file.InitCheck();
    if (status != B_OK) {
        return status;
    }

    return message.Flatten(&file);
}

status_t
SettingsManager::LoadSettings(BMessage &message)
{
	BPath path;
    status_t status = find_directory(B_USER_SETTINGS_DIRECTORY, &path);
	if (status != B_OK) {
		return status;
    }

    status = path.Append(fFileName.String());
    if (status != B_OK) {
        return status;
    }

	BFile file(path.Path(), B_READ_ONLY);
	status = file.InitCheck();
    if (status != B_OK) {
        return status;
    }

    status = message.Unflatten(&file);
	if (status != B_OK) {
		return status;
	}
	return B_OK;
}
