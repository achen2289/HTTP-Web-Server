#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int establish_tcp_port(char *server_msg, char* client_msg,
                             int server_msg_size, int client_msg_size);