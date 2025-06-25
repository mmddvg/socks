#include <stdio.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "sockets.h"
#include "socks.h"

#define QUEUE_LENGTH 10
#define PORT 9090
#define MAX_USERID_LEN 512

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
    req inp;
    socklen_t len = sizeof(srv.sock);
    int cfd = accept(srv.sfd, (struct sockaddr *)&srv.sock, &len);
    if (recv(cfd, &inp, sizeof(inp), MSG_WAITALL) != sizeof(inp))
    {
      perror("accept");
      continue;
    }

    char userid[MAX_USERID_LEN];
    size_t idx = 0;
    char c;
    int n;
    while (idx < MAX_USERID_LEN - 1)
    {
      n = recv(cfd, &c, 1, 0);
      if (n <= 0)
      {
        perror("userid recv");
        return -1;
      }
      if (c == '\0')
      {
        break;
      }
      userid[idx++] = c;
    }
    userid[idx] = '\0';

    printf("successfully read data : %d \n", cfd);
    printf("UserID: %s\n", userid);
    printf("dest ip : %d \n", ntohl(inp.dest_ip));
    printf("dest port : %d \n", ntohs(inp.dest_port));
  }
}
