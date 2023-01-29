#include "tcp.h"

/*
Return -1 when there is an error encountered when trying to establish the tcp
connection; otherwise, it will return the socket file descriptor that we want to
listen for http requests
*/
int establish_tcp_port(char *server_msg, char *client_msg, int server_msg_size,
                       int client_msg_size) {
  int socket_desc;
  struct sockaddr_in server_addr;

  // Clean buffers
  memset(server_msg, '\0', sizeof(char) * server_msg_size);
  memset(client_msg, '\0', sizeof(char) * client_msg_size);

  // Create socket:
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);

  if (socket_desc == -1) {
    printf("Error while creating socket\n");
    return -1;
  }
  printf("Socket created successfully\n");

  // Set the ip and the port
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(15635);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  // Bind to the set port and IP:
  if (bind(socket_desc, (struct sockaddr *)&server_addr, sizeof(server_addr)) ==
      -1) {
    printf("Couldn't bind to the port\n");
    return -1;
  }
  printf("Done with binding\n");

  return socket_desc;
}