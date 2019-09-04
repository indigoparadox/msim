
#ifndef MSIM_H
#define MSIM_H

#include <stdint.h>

typedef uint16_t msim_res_gid;
typedef uint16_t msim_ticks_t;
typedef uint8_t msim_result;

#define MSIM_OK 0
#define MSIM_RESULT_NOT_ENOUGH_INPUT 1
#define MSIM_RESULT_OUTPUT_OVERFLOW 2

struct msim_res {
   uint16_t count;
   msim_res_gid id;
};

struct msim_cvt {
   msim_res_gid input;
   uint8_t input_count;
   msim_res_gid output;
   uint8_t output_count;
   msim_ticks_t interval;
};

msim_result msim_convert(
   struct msim_cvt* cvt, struct msim_res* input, struct msim_res* output );

#endif /* MSIM_H */

