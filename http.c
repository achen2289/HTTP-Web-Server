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

char *response(const Request *client_request) {
  char *uri = client_request->request_uri;

  // change %20 to white spaces in filename
  char proc_uri[2048];
  char *ptr = &proc_uri[0];
  const char *temp_uri = uri;
  memset(proc_uri, '\0', sizeof(char) * 2048);

  for (char *find = strstr(uri, "%20"); find != NULL;
       find = strstr(temp_uri, "%20")) {
    memcpy(ptr, temp_uri, find - temp_uri);
    ptr += (find - temp_uri);
    memcpy(ptr, " ", 1);
    ptr += 1;
    temp_uri = find + 3;
  }
  strcpy(ptr, temp_uri);

  // open file
  FILE *fp;
  if ((fp = fopen(proc_uri + 1, "r")) == NULL) {
    puts("Can't open file with the given uri");
    return NULL;
  }

  fclose(fp);

  return NULL;
}