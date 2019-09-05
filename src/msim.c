
#include "msim.h"

void msim_convert_greedy(
   const struct msim_converter* cvt, struct msim_res res[], uint8_t res_ct
) {
   msim_result result = MSIM_OK;

   while( MSIM_OK == result ) {
      result = msim_convert( cvt, res, res_ct );
   }
}

/** \brief Attempt to perform a resource conversion.
 *  @return MSIM_OK if successful.
 */
msim_result msim_convert(
   const struct msim_converter* cvt, struct msim_res res[], uint8_t res_ct
) {
   msim_result result = MSIM_OK;
   uint16_t test_count;
   uint8_t i = 0, j = 0;

   for( i = 0 ; cvt->input_ct > i ; i++ ) {
      for( j = 0 ; res_ct > j ; j++ ) {
         if( cvt->input[i].id == res[j].id ) {
            if( cvt->input[i].count > res[j].count ) {
               /* Not enough input to convert. */
               result = MSIM_RESULT_NOT_ENOUGH_INPUT;
               goto cleanup;
            }
            /* We found the resource, so go to the next converter. */
            break;
         }
      }
   }

   for( i = 0 ; cvt->output_ct > i ; i++ ) {
      for( j = 0 ; res_ct > j ; j++ ) {
         if( cvt->output[i].id == res[j].id ) {
            test_count = res[j].count + cvt->output[i].count;
            if( test_count < res[j].count ) {
               /* Output bin is full. */
               result = MSIM_RESULT_OUTPUT_OVERFLOW;
               goto cleanup;
            }
            /* We found the resource, so go to the next converter. */
            break;
         }
      }
   }

   for( i = 0 ; cvt->input_ct > i ; i++ ) {
      for( j = 0 ; res_ct > j ; j++ ) {
         if( cvt->input[i].id == res[j].id ) {
            res[j].count -= cvt->input[i].count;
            break;
         }
      }
   }

   for( i = 0 ; cvt->output_ct > i ; i++ ) {
      for( j = 0 ; res_ct > j ; j++ ) {
         if( cvt->output[i].id == res[j].id ) {
            res[j].count += cvt->output[i].count;
            break;
         }
      }
   }

cleanup:
   return result;
}

