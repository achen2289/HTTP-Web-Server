#include "http.h"
#include "response.h"

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

// we assume find is 3 chars long and replaced is 1 char long
void find_and_replace(char *source, char *dest, char *find, char *replace) {
	char proc_source[2048];
  char *ptr = &proc_source[0];
  const char *temp_source = source;
  memset(proc_source, '\0', sizeof(char) * 2048);

  for (char *first = strstr(temp_source, find); first != NULL;
       first = strstr(temp_source, find)) {
    memcpy(ptr, temp_source, first - temp_source);
    ptr += (first - temp_source);
    memcpy(ptr, replace, 1);
    ptr += 1;
    temp_source = first + 3;
  }
  strcpy(ptr, temp_source);
	strcpy(dest, proc_source);
}

char *response(const Request *client_request) {
  char *uri = client_request->request_uri;

  // change %20 to white spaces and %25 to % in filename
  char proc_uri[2048];
	find_and_replace(uri, proc_uri, "%20", " ");
	find_and_replace(proc_uri, proc_uri, "%25", "%");
	puts(proc_uri);
  
	// open file
  FILE *fp;
  if ((fp = fopen(proc_uri + 1, "r")) == NULL) {
    puts("Can't open file with the given uri");
    return NULL;
  }

  fclose(fp);

  return NULL;
}