#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Request {
    char *method;
    char *request_uri;
    char *http_version;
    struct Header *headers;
    char *body;
} Request;

typedef struct Header {
	char *key;
	char *value;
	struct Header *next;
} Header;

Request *parse_request(char *request);

