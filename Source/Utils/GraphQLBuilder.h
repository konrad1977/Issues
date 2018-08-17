/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef GRAPHQLBUILDER_H
#define GRAPHQLBUILDER_H


#include <SupportDefs.h>
#include <support/String.h>


class GraphQLBuilder {
public:
	GraphQLBuilder();
	~GraphQLBuilder();
	
	GraphQLBuilder& AddNode(BString format, BString args);
	GraphQLBuilder& AddNode(BString node);
	
	GraphQLBuilder& AddClosedNode(BString node);
	GraphQLBuilder& AddClosedNode(BString format, BString value);
	
	BString Query();
								
private:
	BString fBuffer;
	int32 fBraceBalance;
};

#endif // _H
