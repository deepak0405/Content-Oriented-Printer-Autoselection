#include <cups/ipp.h>
#include <stdlib.h>
#include <string.h>
#include <cups/cups.h>
#include "types.h"
#include <stdio.h>



int main(int argc, char *argv[]) {

	//uri of ipp printer
	char *uri;  
	char uri1[1024];
	http_t	*http = NULL;
	// attribute variable
	ipp_attribute_t	*attr;

	ipp_t *request, *response = NULL;
	http_uri_status_t	uri_status;
	int		family;
	char scheme[64],userpass[256],hostname[256],resource[1024];
  	int  port;
  	http_encryption_t encryption;
  	char buffer[1024]; 

  	//taking uri from input
  	uri = argv[1];

  	uri_status = httpSeparateURI(HTTP_URI_CODING_ALL, uri,
                                   scheme, sizeof(scheme),
                                   userpass, sizeof(userpass),
				   					hostname, sizeof(hostname),
				   					&(port),
				  					resource, sizeof(resource));


	if (uri_status != HTTP_URI_OK)
  	{
    printf("Incorrect URI - %s.\n", httpURIStatusString(uri_status));
    return (1);
  	}

  	
  	httpAssembleURI(HTTP_URI_CODING_ALL, uri1, sizeof(uri1), scheme, NULL,
                      hostname, port,resource);

  	family = AF_UNSPEC;
  	if ((http = httpConnect2(hostname,port, NULL,family,
                           encryption, 1, 30000, NULL)) == NULL) 
  	{
    printf("Can't connect to %s on port %d.\n",hostname,port);
    return 1;
  	}


  	request = ippNewRequest(IPP_OP_GET_PRINTER_ATTRIBUTES);
 	ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_URI, "printer-uri",
	             NULL,uri1);
  	response = cupsDoRequest(http, request,resource);

  	attr = ippFirstAttribute(response);

  	FILE *out_file = fopen("output.csv","w");


  	while(attr)
  	{
  		ippAttributeString(attr, buffer, sizeof(buffer));
  		fprintf(out_file,"%s,%s\n", ippGetName(attr), buffer);
  		attr = ippNextAttribute(response);
  	}

  	fclose(out_file);
	return 0;
	}