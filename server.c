#include "http.h"
#include "tcp.h"

#define SERVER_MSG_SIZE 2000
#define CLIENT_MSG_SIZE 2000

int main() {
  int socket_fd, client_sock, client_size;
  struct sockaddr_in client_addr;
  char server_msg[SERVER_MSG_SIZE], client_msg[CLIENT_MSG_SIZE];

  // Clean buffers
  memset(server_msg, '\0', sizeof(char) * SERVER_MSG_SIZE);
  memset(client_msg, '\0', sizeof(char) * CLIENT_MSG_SIZE);

  socket_fd = establish_server_port();
  if (socket_fd == -1) {
    printf("Could not establish tcp port");
    return -1;
  }

  // Listen for clients:
  if (listen(socket_fd, 1) == -1) {
    printf("Error while listening\n");
    return -1;
  }
  printf("\nListening for incoming connections.....\n");

  // Accept an incoming connection:
  client_size = sizeof(client_addr);
  client_sock =
      accept(socket_fd, (struct sockaddr *)&client_addr, &client_size);

  if (client_sock == -1) {
    printf("Can't accept\n");
    return -1;
  }
  printf("Client connected at IP: %s and port: %i\n",
         inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

  // TODO: Clean up this part of the code to process the request
  // Receive client's message:
  if (recv(client_sock, client_msg, sizeof(client_msg), 0) < 0) {
    printf("Couldn't receive\n");
    return -1;
  }
  printf("Msg from client: %s\n", client_msg);

  Request *client_request = parse_request(client_msg);

  // Respond to client:
  strcpy(server_msg, "This is the server's message.");

  if (send(client_sock, server_msg, strlen(server_msg), 0) < 0) {
    printf("Can't send\n");
    return -1;
  }

  // Closing the socket:
  close(client_sock);
  close(socket_fd);

  return 0;
}
