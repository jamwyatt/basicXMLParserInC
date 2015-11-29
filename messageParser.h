#ifndef _message_parsing_h_
#define _message_parsing_h_

#include <stdio.h>


// Linked list of name/value pairs ... next=NULL is the end.
struct Attributes {
	char *name;
	char *value;
	struct Attributes *next;
	};

typedef struct Attributes Attributes;

enum MessageType {query,response};
typedef enum MessageType MessageType;

struct Message {
	Attributes *xmlAttrs;		// Attributes in the <?xml header
	Attributes *authAttrs;		// Attributes in the <auth header

	MessageType type;		// Query or response

	// Common data section




	// Message Specific data
	union {
		struct {		// Query specific data
			} query;

		struct {		// Response specific data
			} resp;

		} msg;

	};
typedef struct Message Message;


// Public Access Functions

// Message
Message *parseXMLBuffer(const char *message);


// Attributes
const char *getAttributeValue(const Attributes *list, const char *name);
void printAttributes(const Attributes *list, FILE *fp);


#endif

