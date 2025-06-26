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
  int res = create_socket("127.0.0.1", PORT, &srv);
  if (res < 0)
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
  while (1)
  {
    req *inp = malloc(sizeof(req));

    int cfd = recv_req(srv.sfd, (struct sockaddr *)&srv.sock, inp);

    printf("successfully read data : %d \n", cfd);
    printf("UserID: %s\n", inp->userid);
    printf("dest ip : %d \n", ntohl(inp->dest_ip));
    printf("dest port : %d \n", ntohs(inp->dest_port));
  }
}
