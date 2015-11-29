#include <stdio.h>
#include <stdlib.h>
#include "messageParser.h"


const char *sample = "<?xml version=\"1.0\" encoding=\"US-ASCII\" standalone=\"yes\" ssVersion=\"1\"?> \
<auth name=\"admin\" password=\"admin\"> \
<SRPC cmd=\"getIfStats\" id=\"123\" timeout=\"10\"> \
        <cmdLineParm name=\"interface\" value=\"all\"/> \
        <inputData> \
                System Fixed \
                Saved Config \
        </inputData> \
</SRPC> \
";

int main(int argc, char *argv[]) {


	Message *m=NULL;

	if(m=parseXMLBuffer(sample)) 
		printf("good\n");
	else
		printf("bad\n");

	freeMessage(m);

	return(0);
	}
