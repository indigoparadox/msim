
#include <check.h>
#include <stdint.h>

#include "../src/msim.h"

START_TEST( test_msim ) {
}
END_TEST

Suite* msim_suite( void ) {
   Suite* s = NULL;
   TCase* tc_msim = NULL;

   s = suite_create( "msim" );

   tc_msim = tcase_create( "msim" );
   tcase_add_test( tc_msim, test_msim );

   suite_add_tcase( s, tc_msim );

   return s;
}

