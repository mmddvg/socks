#ifndef SOCKETS_C
#include "sockets.h"

#include <arpa/inet.h>
#include <sys/socket.h>

int create_socket(char *ip, int port, server_socket *res)
{
  int sfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sfd == -1)
    return -1;

  res->sfd = sfd;

  struct sockaddr_in sock;

  sock.sin_addr.s_addr = inet_addr(ip);
  sock.sin_port = htons(port);
  sock.sin_family = AF_INET;

  res->sock = sock;

  if (bind(sfd, (struct sockaddr *)&sock, sizeof(sock)) == -1)
    return -2;

  return 0;
}

#endif // ifndef SOCKETS_C
