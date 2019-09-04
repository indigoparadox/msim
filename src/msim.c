
#include "msim.h"

void msim_convert_greedy(
   struct msim_cvt* cvt, struct msim_res* input, struct msim_res* output
) {
   msim_result result = MSIM_OK;

   while( MSIM_OK == result ) {
      result = msim_convert( cvt, input, output );
   }
}

#include <stdio.h>
/** \brief Attempt to perform a resource conversion.
 *  @return MSIM_OK if successful.
 */
msim_result msim_convert(
   struct msim_cvt* cvt, struct msim_res* input, struct msim_res* output
) {
   msim_result result = MSIM_OK;
   uint16_t test_count;
   struct msim_res* cvt_iter = cvt->input;
   struct msim_res* res_iter = input;

   while( NULL != cvt_iter ) {
      while( NULL != res_iter ) {
         if( cvt_iter->id != res_iter->id ) {
            res_iter = res_iter->next;
            continue;
         }
         if( cvt_iter->count > res_iter->count ) {
            /* Not enough input to convert. */
            result = MSIM_RESULT_NOT_ENOUGH_INPUT;
            goto cleanup;
         }
         break;
      }
      cvt_iter = cvt_iter->next;
   }

   cvt_iter = cvt->output;
   res_iter = output;
   while( NULL != cvt_iter ) {
      while( NULL != res_iter ) {
         if( cvt_iter->id != res_iter->id ) {
            res_iter = res_iter->next;
            continue;
         }
         test_count = res_iter->count + cvt_iter->count;
         if( test_count < res_iter->count ) {
            /* Output bin is full. */
            result = MSIM_RESULT_OUTPUT_OVERFLOW;
            goto cleanup;
         }
         break;
      }
      cvt_iter = cvt_iter->next;
   }

   cvt_iter = cvt->input;
   while( NULL != cvt_iter ) {
      res_iter = input;
      while( NULL != res_iter ) {
         if( cvt_iter->id != res_iter->id ) {
            res_iter = res_iter->next;
            continue;
         }
         res_iter->count -= cvt_iter->count;
         break;
      }
      cvt_iter = cvt_iter->next;
   }

   cvt_iter = cvt->output;
   while( NULL != cvt_iter ) {
      res_iter = output;
      while( NULL != res_iter ) {
         if( cvt_iter->id != res_iter->id ) {
            res_iter = res_iter->next;
            continue;
         }
         res_iter->count += cvt_iter->count;
         break;
      }
      cvt_iter = cvt_iter->next;
   }

cleanup:
   return result;
}

