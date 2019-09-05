
#ifndef MSIM_H
#define MSIM_H

#include <stdint.h>
#include <stddef.h>

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

struct msim_converter {
   const struct msim_res* input;
   uint8_t input_ct;
   const struct msim_res* output;
   uint8_t output_ct;
   msim_ticks_t interval;
   uint16_t flags;
};

void msim_convert_greedy(
   const struct msim_converter* cvt, struct msim_res res[], uint8_t res_ct );
msim_result msim_convert(
   const struct msim_converter* cvt, struct msim_res res[], uint8_t res_ct );

#endif /* MSIM_H */

