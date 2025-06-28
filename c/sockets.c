#include "sockets.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>

int create_server_socket(char *ip, int port, server_socket *res)
{
  int sfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sfd == -1)
  {
    perror("socket");
    return -1;
  }

  res->sfd = sfd;

  struct sockaddr_in sock;

  sock.sin_addr.s_addr = inet_addr(ip);
  sock.sin_port = htons(port);
  sock.sin_family = AF_INET;

  res->sock = sock;

  int opt = 1;
  setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  if (bind(sfd, (struct sockaddr *)&sock, sizeof(sock)) == -1)
  {
    perror("bind");

    close(sfd);
    return -2;
  }

  return 0;
}

int create_client_socket(uint32_t ip, uint16_t port)
{
  int cfd = socket(AF_INET, SOCK_STREAM, 0);
  if (cfd < 0)
    return -1;

  struct sockaddr_in sock;
  sock.sin_addr.s_addr = ip;
  sock.sin_port = port;
  sock.sin_family = AF_INET;

  if (connect(cfd, (struct sockaddr *)&sock, sizeof(struct sockaddr_in)) != 0)
  {
    close(cfd);
    return -2;
  }

  return cfd;
}