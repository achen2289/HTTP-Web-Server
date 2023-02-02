#include "tcp.h"
#define PORT 15635
/*
Return -1 when there is an error encountered when trying to establish the tcp
connection; otherwise, it will return the socket file descriptor that we want to
listen for http requests
*/
int establish_server_port() {
  int socket_fd;
  struct sockaddr_in server_addr;

  // Create socket file descriptor
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd == -1) {
    puts("Error when creating socket");
    return -1;
  }
  puts("Socket created successfully");

  // Set the ip and the port
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  // Bind the socket to the ip and port
  if (bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) ==
      -1) {
    puts("Could not bind to the port");
    return -1;
  }
  puts("Binding Established");

  return socket_fd;
}