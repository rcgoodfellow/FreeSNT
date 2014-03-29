/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* FreeSNT - Free Synchrophasor Network Tools
*
* Copyright (c) The FreeSNT project. All rights reserved.
* BSD V3 License, see COPYRIGHT for details
*
* ~ry
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#include "command.h"
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>

float version() { return 0.1; }

struct snt_cmd* 
snt_cr_cmd(uint16_t id_code, uint16_t cmd, size_t data_size, unsigned char *data)
{
  struct snt_cmd *c= (struct snt_cmd*)malloc(sizeof(struct snt_cmd));

  c->baseframe.sync[0] = 0xAA;
  c->baseframe.sync[1] = 0x41;
  unsigned frame_size = sizeof(struct snt_cmd) + data_size - sizeof(char*); 
  c->baseframe.frame_size = htons(frame_size);
  c->baseframe.id_code = htons(id_code);

  struct timespec t;
  clock_gettime(CLOCK_REALTIME, &t);
  c->baseframe.soc = t.tv_sec;
  c->baseframe.soc = htonl(0x44856030);
  c->baseframe.fracsec = htonl(0x0f0bbfd0);

  c->baseframe.fracsec = (t.tv_nsec & 0xffffffffffffff00)
                                    | 0x0000000000000006; // within 10^-6 s of UTC
  c->cmd = htons(cmd);
  c->extra_frame = data;
  unsigned sz = frame_size - sizeof(uint16_t);
  c->chk = htons(crc_ccitt((unsigned char*)c, sz));
  
  return c;
}

void
hexdump(unsigned char *elem, size_t sz)
{
  for(unsigned i=0; i<sz; i+=2)
  {
    printf("%02x%02x ", elem[i], elem[i+1]);
  }
}

void
snt_print_baseframe(struct snt_baseframe *b)
{
  printf("sync: %x%x\n", (unsigned)b->sync[0], (unsigned)b->sync[1]);
  printf("frame size: %x\n", ntohs(b->frame_size));
  printf("id_code: %x\n", ntohs(b->id_code));
  printf("soc: %x\n", ntohl(b->soc));
  printf("fracsec: %x\n", ntohl(b->fracsec));
}

void
snt_print_cmd(struct snt_cmd *c)
{
  printf("Command Frame:\n");
  printf("-------------------------------\n");
  snt_print_baseframe(&c->baseframe);
  printf("cmd: %x\n", ntohs(c->cmd));
  printf("chk: %x\n", ntohs(c->chk));
}

uint16_t 
crc_ccitt(unsigned char *msg, size_t len)
{
  uint16_t crc=0xffff, tmp, quick;
  for(size_t i=0; i<len; ++i)
  {
    tmp = (crc>>8) ^ msg[i];
    crc <<= 8;
    quick = tmp ^ (tmp >> 4);
    crc ^= quick;
    quick <<= 5;
    crc ^= quick;
    quick <<= 7;
    crc ^= quick;
  }
  return crc;
}
