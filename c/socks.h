#ifndef STRUCTS_H

#include <stdint.h>
#pragma pack(push, 1)

typedef struct
{
  uint8_t vn;
  uint8_t cd;
  uint16_t dest_port;
  uint32_t dest_ip;
} req;

typedef struct
{
  uint8_t vn;
  uint8_t cd;
  uint16_t dest_port;
  uint32_t dest_ip;
} res;

#pragma pack(pop)

#endif // !STRUCTS_H
