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

  for (int i = 0; temp_source[i] != '\0'; i++) {
    char *loc = strstr(&temp_source[i], find);
    if (loc != NULL) {
      strncpy(ptr, temp_source, (loc - temp_source) / sizeof(char));
      ptr += (loc - temp_source) / sizeof(char);
      strncpy(ptr, replace, 1);
      ptr += 1;
      temp_source = loc + 3;
    }
  }
  strcpy(ptr, temp_source);
  strcpy(dest, proc_source);
}

int response(const Request *client_request, char *server_msg) {
  char *uri = client_request->request_uri;

  // change %20 to white spaces and %25 to % in filename
  char proc_uri[2048];
  find_and_replace(uri, proc_uri, "%20", " ");
  find_and_replace(proc_uri, proc_uri, "%25", "%");
  puts(proc_uri);
  char *filename = proc_uri + 1;

  // open file
  // TODO: behavior when no file given?
  FILE *fp;
  fp = fopen(filename, "rb");
  char *file_content = 0;
  long length;
  if (fp == NULL) {
    puts("Can't open file with the given uri");
    return -1;
  } else {
    fseek(fp, 0, SEEK_END);
    length = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    file_content = malloc(length);
    if (file_content == NULL) {
      puts("Cannot allocate memory to read file");
      return -1;
    }
    fread(file_content, 1, length, fp);
    fclose(fp);
  }

  // Content of the response
  // where's this upper limit from? (1MB?) yeah
  char response[1024000];
  memset(response, '\0', sizeof(char) * 1024000);

  // TODO: set status based on content result
  char *status = SUCCESS;

  // TODO: is it always close? idk
  char *connection = CONNECTION_KEEP_ALIVE;

  time_t t = time(NULL);
  char *time = asctime(gmtime(&t));
  time[strlen(time) - 1] = '\0';
  char date[50] = "Date: ";
  strcat(date, time);
  strcat(date, " GMT\r\n");

  char *server = SERVER;

  struct stat attr;
  if (stat(filename, &attr) == -1) {
    puts("stat error");
    return -1;
  }
  char *lmtime = asctime(gmtime(&(attr.st_mtime)));
  lmtime[strlen(lmtime) - 1] = '\0';
  char last_modified[50] = "Last-Modified: ";
  strcat(last_modified, lmtime);
  strcat(last_modified, " GMT\r\n");

  long long content_length;
  content_length = (long long)attr.st_size;
  char content_length_str[256];
  sprintf(content_length_str, "Content-Length: %lld\r\n", content_length);

  char content_type[50];
  if (strstr(filename, ".html")) {
    strcpy(content_type, HTML);
  } else if (strstr(filename, ".txt")) {
    strcpy(content_type, TXT);
  } else if (strstr(filename, ".jpg")) {
    strcpy(content_type, JPG);
  } else if (strstr(filename, ".png")) {
    strcpy(content_type, PNG);
  } else if (strstr(filename, ".pdf")) {
    strcpy(content_type, PDF);
  } else {
    strcpy(content_type, BIN);
  }

  strcat(response, status);
  strcat(response, connection);
  strcat(response, date);
  strcat(response, server);
  strcat(response, last_modified);
  strcat(response, content_length_str);
  strcat(response, content_type);
  strcat(response, "\r\n");

  int header = strlen(response);
  memcpy(response + header, file_content, content_length);
  memcpy(server_msg, response, header + content_length);

  free(file_content); // memory leaks bad
  return header + content_length;
}