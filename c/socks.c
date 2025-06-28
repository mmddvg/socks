#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "socks.h"

#define BUF_SIZE 512

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

int bi_proxy(int fd1, int fd2)
{
    struct pollfd fds[2] = {
        {.fd = fd1, .events = POLLIN},
        {.fd = fd2, .events = POLLIN}};

    char buf[BUF_SIZE];

    while (1)
    {
        int poll_res = poll(fds, 2, -1);
        if (poll_res < 0)
        {
            perror("poll");
            return -1;
        }

        if (fds[0].revents & POLLIN)
        {
            ssize_t n = read(fd1, buf, sizeof(buf));
            if (n <= 0)
                break;
            ssize_t total_written = 0;
            while (total_written < n)
            {
                ssize_t w = write(fd2, buf + total_written, n - total_written);
                if (w <= 0)
                    return -1;
                total_written += w;
            }
        }

        if (fds[1].revents & POLLIN)
        {
            ssize_t n = read(fd2, buf, sizeof(buf));
            if (n <= 0)
                break;
            ssize_t total_written = 0;
            while (total_written < n)
            {
                ssize_t w = write(fd1, buf + total_written, n - total_written);
                if (w <= 0)
                    return -1;
                total_written += w;
            }
        }

        // check for hangups or errors
        if (fds[0].revents & (POLLERR | POLLHUP | POLLNVAL) ||
            fds[1].revents & (POLLERR | POLLHUP | POLLNVAL))
        {
            break;
        }
    }

    return 0;
}

void hello()
{
    printf("hello world\n");
}