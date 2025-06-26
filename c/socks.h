#ifndef SOCKS_H
#define SOCKS_H

#include <unistd.h>
#include <stdint.h>
#include <sys/socket.h>

#include "sockets.h"
#include "consts.h"

#pragma pack(push, 1)

typedef struct
{
  uint8_t vn;
  uint8_t cd;
  uint16_t dest_port;
  uint32_t dest_ip;
  char userid[MAX_USERID_LEN];
} req;

typedef struct
{
  uint8_t vn;
  uint8_t cd;
  uint16_t dest_port;
  uint32_t dest_ip;
} res;

#pragma pack(pop)

int recv_req(int sfd, struct sockaddr *, req *);

#endif // !SOCKS_H
