#include "tcp.h"
#define server_msg_size 2000
#define client_msg_size 2000

int main() {
  int socket_desc, client_sock, client_size;
  struct sockaddr_in client_addr;
  char server_message[server_msg_size], client_message[client_msg_size];

  socket_desc = establish_tcp_port(server_message, client_message,
                                   server_msg_size, client_msg_size);

  if (socket_desc == -1) {
    printf("Could not establish tcp port");
    return -1;
  }

  // Listen for clients:
  if (listen(socket_desc, 1) == -1) {
    printf("Error while listening\n");
    return -1;
  }
  printf("\nListening for incoming connections.....\n");

  // Accept an incoming connection:
  client_size = sizeof(client_addr);
  client_sock =
      accept(socket_desc, (struct sockaddr *)&client_addr, &client_size);

  if (client_sock == -1) {
    printf("Can't accept\n");
    return -1;
  }
  printf("Client connected at IP: %s and port: %i\n",
         inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

  // TODO: Clean up this part of the code to process the request
  // Receive client's message:
  if (recv(client_sock, client_message, sizeof(client_message), 0) < 0) {
    printf("Couldn't receive\n");
    return -1;
  }
  printf("Msg from client: %s\n", client_message);

  // Respond to client:
  strcpy(server_message, "This is the server's message.");

  if (send(client_sock, server_message, strlen(server_message), 0) < 0) {
    printf("Can't send\n");
    return -1;
  }

  // Closing the socket:
  close(client_sock);
  close(socket_desc);

  return 0;
}
