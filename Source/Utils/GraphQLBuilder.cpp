/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "GraphQLBuilder.h"

GraphQLBuilder::GraphQLBuilder()
	:fBraceBalance(0)
{
	fBuffer << "{ \"query\" : \"query ";
}
GraphQLBuilder::~GraphQLBuilder()
{
	
}
	
GraphQLBuilder&
GraphQLBuilder::AddNode(BString node)
{
	fBraceBalance++;
	fBuffer << " { " << node;
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
