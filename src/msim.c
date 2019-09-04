
#include "msim.h"

/** \brief Attempt to perform a resource conversion.
 *  @return MSIM_OK if successful.
 */
msim_result msim_convert(
   struct msim_cvt* cvt, struct msim_res* input, struct msim_res* output
) {
   msim_result result = MSIM_OK;

   if( cvt->input_count > input->count ) {
      /* Not enough input to convert. */
      result = MSIM_RESULT_NOT_ENOUGH_INPUT;
      goto cleanup;
   }

   if( output->count + cvt->output_count < output->count ) {
      /* Output bin is full. */
      result = MSIM_RESULT_OUTPUT_OVERFLOW;
      goto cleanup;
   }

   input->count -= cvt->input_count;
   output->count += cvt->output_count;

cleanup:
   return result;
}

