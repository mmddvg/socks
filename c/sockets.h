#ifndef SOCKETS_H
#define SOCKETS_H

#include <sys/socket.h>
#include <netinet/in.h>

typedef struct
{
  int sfd;
  struct sockaddr_in sock;
} server_socket;

int create_server_socket(char *ip, int port, server_socket *res);
int create_client_socket(uint32_t ip, uint16_t port);

#endif // !SOCKETS_H
