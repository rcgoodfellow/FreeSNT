/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* FreeSNT - Free Synchrophasor Network Tools
*
* Copyright (c) The FreeSNT project. All rights reserved.
* BSD V3 License, see COPYRIGHT for details
*
* ~ry
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#include <sys/types.h>
#include <time.h>
#include <stdio.h>

/*
Command Frame Packet Format
---------------------------

 1             8               16              24              32
|0 1 2 3 4 5 6 7|0 1 2 3 4 5 6 7|0 1 2 3 4 5 6 7|0 1 2 3 4 5 6 7|
-----------------------------------------------------------------
|sync...........................|frame_size.....................|
|id_code........................|soc............................
 ...............................|fracsec........................
 ...............................|cmd............................|
|extra_frame[0-65518B] ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? 
|chk............................|


*/

struct snt_baseframe
{
  unsigned char       sync[2]; 
  uint16_t            frame_size; 
  uint16_t            id_code;
  uint32_t            soc; 
  uint32_t            fracsec;
}__attribute__((packed));

struct snt_cmd
{
  struct snt_baseframe  baseframe;
  uint16_t              cmd; 
  unsigned char         *extra_frame;
  uint16_t              chk;
}__attribute__((packed));

struct snt_cmd* 
snt_cr_cmd(uint16_t id_code, uint16_t cmd, size_t data_size, unsigned char *data);

uint16_t 
crc_ccitt(unsigned char *msg, size_t len);

void
hexdump(unsigned char *elem, size_t sz);

void
snt_print_baseframe(struct snt_baseframe *b);

void
snt_print_cmd(struct snt_cmd *c);
