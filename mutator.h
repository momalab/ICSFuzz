#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#define ARITH_MAX 35

#define FLIP_BIT(_ar, _b) do { \
    uint8_t* _arf = (uint8_t*)(_ar); \
    uint32_t _bf = (_b); \
    _arf[(_bf) >> 3] ^= (128 >> ((_bf) & 7)); \
  } while (0)

#define SWAP16(_x) ({ \
    uint16_t _ret = (_x); \
    (uint16_t)((_ret << 8) | (_ret >> 8)); \
  })

#define SWAP32(_x) ({ \
    uint32_t _ret = (_x); \
    (uint32_t)((_ret << 24) | (_ret >> 24) | \
          ((_ret << 8) & 0x00FF0000) | \
          ((_ret >> 8) & 0x0000FF00)); \
  })

uint32_t fuzzing_engine(int fd, unsigned long addr, char *in_buf, int len);
