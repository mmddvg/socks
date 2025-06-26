#ifndef SOCKETS_H
#define SOCKETS_H

#include <sys/socket.h>
#include <netinet/in.h>

typedef struct
{
  int sfd;
  struct sockaddr_in sock;
} server_socket;

int create_socket(char *ip, int port, server_socket *res);

#endif // !SOCKETS_H
