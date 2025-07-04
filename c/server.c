#include <stdio.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "sockets.h"
#include "socks.h"

int main()
{
  server_socket srv;
  int tmp = create_server_socket("127.0.0.1", PORT, &srv);
  if (tmp < 0)
  {
    perror("crete_socket");
    return -1;
  }

  if (listen(srv.sfd, QUEUE_LENGTH) == -1)
  {
    perror("listen");
    return -1;
  }

  printf("listenting on port %d\n", PORT);
  req *inp = malloc(sizeof(req));

  int cfd;
  while (1)
  {
    printf("starting new iteration\n");
    memset(inp, 0, sizeof(req));
    cfd = recv_req(srv.sfd, (struct sockaddr *)&srv.sock, inp);
    if (cfd < 0)
    {
      perror("recv_req");
      continue;
    }
    printf("successfully read data : %d \n", cfd);
    printf("UserID: %s\n", inp->userid);
    printf("dest ip : %d \n", ntohl(inp->dest_ip));
    printf("dest port : %d \n", ntohs(inp->dest_port));

    int pid = fork();
    if (pid < 0)
    {
      perror("fork");
      continue;
    }
    else if (pid > 0)
      continue;

    break; // child process to handle the request
  }

  int destfd, result;
  switch (inp->cd)
  {
  case 1:
    printf("connect\n");

    destfd = create_client_socket(inp->dest_ip, inp->dest_port);
    if (destfd < 0)
    {
      perror("create client socket");
      res response = make_connect_res(91, inp->dest_port, inp->dest_ip);
      send(cfd, &response, sizeof(res), 0);
      return -1;
    }

    res response = make_connect_res(90, inp->dest_port, inp->dest_ip);
    send(cfd, &response, sizeof(res), 0);
    printf("sent response connect \n");

    result = bi_proxy(cfd, destfd);

    break;
  case 2:
    printf("bind\n");
    break;
  default:
    printf("invalid cd");
    break;
  }

  close(destfd);
  close(cfd);
  return result;
}
