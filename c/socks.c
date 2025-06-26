#include <stdio.h>
#include <stdlib.h>
#include "socks.h"

int recv_req(int sfd, struct sockaddr *sock, req *res)
{

    socklen_t len = sizeof(struct sockaddr);
    int cfd = accept(sfd, sock, &len);
    if (recv(cfd, res, sizeof(req) - MAX_USERID_LEN, MSG_WAITALL) != sizeof(req))
    {
        return -1;
    }

    size_t idx = 0;
    char c;
    while (idx < MAX_USERID_LEN - 1)
    {
        if (recv(cfd, &c, 1, 0) <= 0)
        {
            perror("userid recv");
            return -2;
        }
        if (c == '\0')
        {
            break;
        }
        res->userid[idx++] = c;
    }
    res->userid[idx] = '\0';

    return cfd;
}
