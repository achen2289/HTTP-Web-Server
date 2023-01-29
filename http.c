#include "http.h"

Request *parse_request(char *raw_request) {
	// Allocate memory for struct containing Request details
	Request *request = malloc(sizeof(struct Request));
	if (!request) {
		return NULL;
	}

	// Parse request line
	char *method = strtok(raw_request, " ");
	request->method = method;

	char *request_uri = strtok(NULL, " ");
	request->request_uri = request_uri;

	char *http_version = strtok(NULL, "\r\n");
	request->http_version = http_version;

	// Parse request header fields
	Header *prev_header = NULL;
	Header *first_header = NULL;

	char *header_key, *header_value;
	while ((header_key = strtok(NULL, ": ")) != NULL) {
		header_value = strtok(NULL, "\r\n");

		// Allocate memory for struct containing Header details
		Header *header = malloc(sizeof(struct Header));
		if (!header) {
			return NULL;
		}

		// Add header
		header->key = header_key;
		header->value = header_value;
		if (prev_header) {
			prev_header->next = header;
		}

		if (!first_header) {
			first_header = header;
		}

		prev_header = header;
	}
	request->headers = first_header;

	return request;
}

