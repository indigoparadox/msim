
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
   struct msim_res* next;
};

struct msim_cvt {
   struct msim_res* input;
   struct msim_res* output;
   msim_ticks_t interval;
   uint16_t flags;
};

void msim_convert_greedy(
   struct msim_cvt* cvt, struct msim_res* input, struct msim_res* output );
msim_result msim_convert(
   struct msim_cvt* cvt, struct msim_res* input, struct msim_res* output );

#endif /* MSIM_H */

