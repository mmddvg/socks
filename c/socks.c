#include <stdio.h>
#include <stdlib.h>
#include "socks.h"

int recv_req(int sfd, struct sockaddr *sock, req *res)
{

    socklen_t len = sizeof(struct sockaddr);
    int cfd = accept(sfd, sock, &len);
    int fixed_parts_size = sizeof(req) - MAX_USERID_LEN;
    if (recv(cfd, res, fixed_parts_size, MSG_WAITALL) != fixed_parts_size)
    {
        perror("fixed req parts");
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

res make_connect_res(uint8_t cd, uint16_t dset_port, uint32_t dest_ip)
{
    res tmp;
    tmp.vn = 0;
    tmp.cd = cd;
    tmp.dest_ip = dest_ip;
    tmp.dest_port = dset_port;

    return tmp;
}
