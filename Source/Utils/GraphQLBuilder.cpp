/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "GraphQLBuilder.h"
#include <stdarg.h>

GraphQLBuilder::GraphQLBuilder()
	:fBraceBalance(0)
{
	fBuffer << "{\"query\" :\"";
}
GraphQLBuilder::~GraphQLBuilder()
{

}

GraphQLBuilder&
GraphQLBuilder::AddNode(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	return AddNode(BString().SetToFormatVarArgs(format, args));
}

GraphQLBuilder&
GraphQLBuilder::AddClosedNode(BString format, BString value)
{
	return AddClosedNode(BString().SetToFormat(format.String(), value.String()));
}

GraphQLBuilder&
GraphQLBuilder::AddValue(BString node)
{
	fBuffer << node << " ";
	return *this;
}


GraphQLBuilder&
GraphQLBuilder::AddClosedNode(BString node)
{
	fBuffer << " { " << node << " } ";
	return *this;
}

GraphQLBuilder&
GraphQLBuilder::AddNode(BString node)
{
	fBraceBalance++;
	fBuffer << " { " << node << " ";
	return *this;
}

BString
GraphQLBuilder::Query()
{
	for (int32 i = 0; i<fBraceBalance; i++) {
		fBuffer << " } ";
	}
	fBuffer << "\"}";
	return fBuffer;
}
