#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "messageParser.h"

void *freeMessage(Message *m) {

	if(!m)
		return(NULL);
	// Message specific cleanup

	// Cleanup the final object
	free(m);
	return(NULL); // return null for replacement of caller Message Pointer
	}


const char * parsingError(const char *errStr, const char *contextStr) {
	fprintf(stderr,"Parsing Error : %s near (%-25.25s)\n",errStr,contextStr);
	return(NULL);
	}

const char *eatWhiteSpace(const char *ptr) {
	while(ptr && (*ptr != 0) && isspace(*ptr))
		ptr++;
	return(ptr);
	}


const char *expectAndConsume(const char *ptr, const char *target) {
	ptr=eatWhiteSpace(ptr);
	if(strncmp(target,ptr,strlen(target)) == 0)
		return(ptr + strlen(target));
	else
		return(ptr);
	}


static char nameMap[255];
static int nameMapInitDone = 0;

const char *getName(const char *msgPtr, char **name) {
	// A string of characters [A-Za-z0-9_-.] ... no length restrictions?
	if(!nameMapInitDone) {
		int x;
		memset(nameMap,0,sizeof(nameMap));
		for(x='A';x<='Z';x++) nameMap[x] = 1;
		for(x='a';x<='z';x++) nameMap[x] = 1;
		for(x='0';x<='9';x++) nameMap[x] = 1;
		nameMap['.'] = 1;
		nameMap['-'] = 1;
		nameMap['_'] = 1;
		}
	msgPtr=eatWhiteSpace(msgPtr);
	*name = NULL;
	const char *start = msgPtr;
	while(msgPtr && *msgPtr && (nameMap[*msgPtr] == 1))
		msgPtr++;
	const char *stop = msgPtr;
	if(stop - start > 1) {
		*name = malloc(stop - start + 1);	// Place to store the string
		strncpy(*name,start,stop-start);
		return(msgPtr);				// good find
		}
	return(NULL);	// didn't find anything?
	}



const char *getString(const char *msgPtr, char **value) {
	// String is bracketed with \" characters and uses escape sequences to include "special" characters
	msgPtr=eatWhiteSpace(msgPtr);
	*value = NULL;
	if(msgPtr && (*msgPtr == '"')) {
		msgPtr++;	// skip the \"
		const char *start = msgPtr;
		while(*msgPtr && (*msgPtr != '\"'))
			msgPtr++;
		if(msgPtr && (*msgPtr != '"'))
			return(NULL);		// string doesn't end with a \"
		const char *stop = msgPtr++;		// Increment past the \"
		*value = malloc(stop-start + 1);
		strncpy(*value,start, stop-start);
		return(msgPtr);
		}
	return(NULL);	// string doesn't start with \"
	}



const char *getNameValuePair(Attributes **attrs,const char *msgPtr) {
	char *name=NULL;
	char *value=NULL;

	// A name value pair is optional - else is no problem
	if(msgPtr = getName(msgPtr,&name)) {
		if(!(msgPtr = expectAndConsume(msgPtr,"="))) {
			free(name);
			name=NULL;
			return(parsingError("missing assignment in the name value pair", msgPtr));
			}
		if(!(msgPtr = getString(msgPtr,&value))) {
			free(name);
			name=NULL;
			return(parsingError("missing value string in name value pair", msgPtr));
			}
		// Got a name and a Value ... now record in an Attributes object
		*attrs = malloc(sizeof(Attributes));
		(*attrs)->next = NULL;
		(*attrs)->name = name;
		(*attrs)->value = value;
		}
	return(msgPtr);
	}


const char *addNameValuePairs(Attributes **attrs, const char *msgPtr) {
	while(msgPtr = getNameValuePair(attrs,msgPtr))
		*attrs = &((*attrs)->next);	// point to the "next" field's address for maintaining the linked list
	return(msgPtr);
	}


const char *parseXmlHdr(Message *m, const char *msgPtr) {

	if(!(msgPtr = expectAndConsume(msgPtr,"<")))
		return(parsingError("missing start to the tag", msgPtr));
	if(!(msgPtr = expectAndConsume(msgPtr,"?xml")))
		return(parsingError("missing ?xml in the XML hdr", msgPtr));

	if(!(msgPtr = addNameValuePairs(&(m->xmlAttrs),msgPtr)))	// parse all N/V pairs (zero or many)
		reutrn(NULL);	// Error occurred during parsing

	if(!(msgPtr = expectAndConsume(msgPtr,"?>")))
		return(parsingError("missing ?> to code the XML hdr", msgPtr));

	return(NULL);	// Error
	}



// "Message" Functions
Message *parseXMLBuffer(const char *message) {
	char *ptr=message;

	Message m = malloc(sizeof(Message));

	if((!ptr=parseXmlHdr(m,ptr)) || (!ptr=parseOptionalAuthHdr(m,ptr)) || (!ptr=parseQueryOrResponse(m,ptr))) 
		m=freeMessage(m);

	return(m);
	}


// "Attributes" functions
const char *getAttributeValue(const Attributes *list, const char *name) {
	}

void printAttributes(const Attributes *list, FILE *fp) {
	}



