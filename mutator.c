#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "alloc-inl.h"



uint8_t fuzz_one(char *in_buf, int len) {

  signed int stage_cur, stage_max, stage_cur_byte, stage_cur_val;
  uint8_t i;
  uint8_t *stage_name = "init";
  uint8_t *stage_short = "init";
  uint8_t *out_buf;
  
  uint8_t  ret_val = 1;


  out_buf = malloc(len*sizeof(len));

 
  memcpy(out_buf, in_buf, len);


  /*********************************************
   * SIMPLE BITFLIP (+dictionary construction) *
   *********************************************/

#define FLIP_BIT(_ar, _b) do { \
    uint8_t* _arf = (uint8_t*)(_ar); \
    uint32_t _bf = (_b); \
    _arf[(_bf) >> 3] ^= (128 >> ((_bf) & 7)); \
  } while (0)

  /* Single walking bit. */

  stage_short = "flip1";
  stage_max   = len << 4;
  stage_name  = "bitflip 1/1";

  
  for (stage_cur = 0; stage_cur < stage_max; stage_cur++) {

    stage_cur_byte = stage_cur >> 1;

    FLIP_BIT(out_buf, stage_cur);
    
    printf("%d, %s\n", stage_cur, out_buf);	
    
    FLIP_BIT(out_buf, stage_cur);
  }

   
  stage_name  = "bitflip 2/1";
  stage_short = "flip2";
  stage_max   = (len << 3) - 1;


  for (stage_cur = 0; stage_cur < stage_max; stage_cur++) {

    stage_cur_byte = stage_cur >> 1;

    FLIP_BIT(out_buf, stage_cur);
    FLIP_BIT(out_buf, stage_cur + 1);

    printf("%d, %s\n", stage_cur, out_buf);

    FLIP_BIT(out_buf, stage_cur);
    FLIP_BIT(out_buf, stage_cur + 1);

  }

  stage_name  = "bitflip 4/1";
  stage_short = "flip4";
  stage_max   = (len << 3) - 3;

  for (stage_cur = 0; stage_cur < stage_max; stage_cur++) {

    stage_cur_byte = stage_cur >> 3;

    FLIP_BIT(out_buf, stage_cur);
    FLIP_BIT(out_buf, stage_cur + 1);
    FLIP_BIT(out_buf, stage_cur + 2);
    FLIP_BIT(out_buf, stage_cur + 3);

    printf("%d, %s\n", stage_cur, out_buf);

    FLIP_BIT(out_buf, stage_cur);
    FLIP_BIT(out_buf, stage_cur + 1);
    FLIP_BIT(out_buf, stage_cur + 2);
    FLIP_BIT(out_buf, stage_cur + 3);

  }

  stage_name  = "bitflip 8/8";
  stage_short = "flip8";
  stage_max   = len;

  for (stage_cur = 0; stage_cur < stage_max; stage_cur++) {

    stage_cur_byte = stage_cur;

    out_buf[stage_cur] ^= 0xFF;

    printf("%d, %s\n", stage_cur, out_buf);

    out_buf[stage_cur] ^= 0xFF;

  }

  stage_name  = "bitflip 16/8";
  stage_short = "flip16";
  stage_cur   = 0;
  stage_max   = len - 1;

  for (i = 0; i < len - 1; i++) {

    stage_cur_byte = i;

    *(u16*)(out_buf + i) ^= 0xFFFF;
    
    printf("%d, %s\n", stage_cur, out_buf);
    stage_cur++;

    *(u16*)(out_buf + i) ^= 0xFFFF;


  }


  return ret_val;

}



int main(){
	

	int start_val = 0x11111111;
	char *buf = malloc(4*sizeof(uint8_t));
	sprintf(buf, "%d", start_val);
	int len = strlen(buf);

	uint8_t retval = fuzz_one(buf, len);

}  
