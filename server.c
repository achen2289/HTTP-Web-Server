#include "http.h"
#include "tcp.h"

#define SERVER_MSG_SIZE 2000
#define CLIENT_MSG_SIZE 2000

int main() {
  int socket_fd, client_sock, client_size;
  struct sockaddr_in client_addr;
  char server_msg[SERVER_MSG_SIZE], client_msg[CLIENT_MSG_SIZE];

  // clean buffers for the server and client messages
  memset(server_msg, '\0', sizeof(char) * SERVER_MSG_SIZE);
  memset(client_msg, '\0', sizeof(char) * CLIENT_MSG_SIZE);

  socket_fd = establish_server_port();
  if (socket_fd == -1) {
    printf("Could not establish tcp port");
    return -1;
  }

  // listen to clients
  if (listen(socket_fd, 1) == -1) {
    printf("Error while listening\n");
    return -1;
  }
  printf("\nListening for incoming connections.....\n");

  // accept an incoming connection from client
  client_size = sizeof(client_addr);
  client_sock =
      accept(socket_fd, (struct sockaddr *)&client_addr, &client_size);

  if (client_sock == -1) {
    printf("Can't accept\n");
    return -1;
  }
  printf("Client connected at IP: %s and port: %i\n",
         inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

  // receive client's message
  if (recv(client_sock, client_msg, sizeof(client_msg), 0) < 0) {
    printf("Couldn't receive\n");
    return -1;
  }
  printf("Msg from client: %s\n", client_msg);

  // parse the client request
  Request *client_request = parse_request(client_msg);
  // This works
  printf("Client request url: %s\n", client_request->request_url);
  // But this does not
  // response(client_request);

  // response to client
  if (send(client_sock, server_msg, strlen(server_msg), 0) < 0) {
    printf("Can't send\n");
    return -1;
  }

  free(client_request);
  // close sockets for both server and client
  close(client_sock);
  close(socket_fd);

  return 0;
}
