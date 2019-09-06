
#include "msim.h"

#include <assert.h>

/** \brief Perform the resource conversion until it cannot be performed
 *         any longer due to resource constraints.
 */
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
   uint8_t res_id = 0;
   uint8_t i = 0;

   assert( NULL != cvt->input );
   for( i = 0 ; cvt->input_ct > i ; i++ ) {
      assert( NULL != cvt->input[i].def );
      res_id = cvt->input[i].def->id;
      if( cvt->input[i].count > res[res_id].count ) {
         /* Not enough input to convert. */
         result = MSIM_RESULT_NOT_ENOUGH_INPUT;
         goto cleanup;
      }
      /* We found the resource, so go to the next converter. */
      break;
   }

   assert( NULL != cvt->output );
   for( i = 0 ; cvt->output_ct > i ; i++ ) {
      assert( NULL != cvt->output[i].def );
      res_id = cvt->output[i].def->id;
      test_count = res[res_id].count + cvt->output[i].count;
      if( test_count < res[res_id].count ) {
         /* Output bin is full. */
         result = MSIM_RESULT_OUTPUT_OVERFLOW;
         goto cleanup;
      }
      /* We found the resource, so go to the next converter. */
      break;
   }

   /* Perform the input. */
   for( i = 0 ; cvt->input_ct > i ; i++ ) {
      res_id = cvt->input[i].def->id;
      if( cvt->input[i].def->id == res[res_id].def->id ) {
         res[res_id].count -= cvt->input[i].count;
         break;
      }
   }

   /* Perform the output. */
   for( i = 0 ; cvt->output_ct > i ; i++ ) {
      res_id = cvt->output[i].def->id;
      if( cvt->output[i].def->id == res[res_id].def->id ) {
         res[res_id].count += cvt->output[i].count;
         break;
      }
   }

cleanup:
   return result;
}

/** \brief Perform decays on the specified resource stack.
 */
void msim_decay( struct msim_res res[], uint8_t res_ct ) {
   uint8_t i = 0;

   for( i = 0 ; res_ct > i ; i++ ) {
      if( res[i].count > res[i].def->decay ) {
         res[i].count -= res[i].def->decay;
      }
   }
}

