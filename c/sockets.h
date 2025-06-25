#ifndef SOCKETS_H

#include <sys/socket.h>
#include <netinet/in.h>

typedef struct
{
  int sfd;
  struct sockaddr_in sock;
} server_socket;

int create_socket(char *, int, server_socket *);

#endif // !SOCKETS_H
