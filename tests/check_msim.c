
#include <check.h>
#include <stdint.h>
#include <stdlib.h>

#include "../src/msim.h"

#define RES_MONEY 1
#define RES_CORN 2

struct msim_res money;
struct msim_res corn;
struct msim_cvt market;

static void setup_msim() {
   money.id = RES_MONEY;
   money.count = 1000;

   corn.id =  RES_CORN;
   corn.count = 500;

   market.input = calloc( 1, sizeof( struct msim_res ) );
   market.input->id = RES_MONEY;
   market.input->count = 10;
   market.flags = 0;
   market.output = calloc( 1, sizeof( struct msim_res ) );
   market.output->id = RES_CORN;
   market.output->count = 30;
}

static void teardown_msim() {
   free( market.input );
   free( market.output );
}

START_TEST( test_msim_convert ) {
   msim_result result = MSIM_OK;

   result = msim_convert( &market, &money, &corn );

   ck_assert_int_eq( result, MSIM_OK );
   ck_assert_int_eq( money.count, 990 );
   ck_assert_int_eq( corn.count, 530 );
}
END_TEST

START_TEST( test_msim_convert_insufficient ) {
   money.count = 5;
   msim_result result = MSIM_OK;

   result = msim_convert( &market, &money, &corn );

   ck_assert_int_eq( result, MSIM_RESULT_NOT_ENOUGH_INPUT );
   ck_assert_int_eq( money.count, 5 );
   ck_assert_int_eq( corn.count, 500 );
}
END_TEST

START_TEST( test_msim_convert_greedy ) {
   msim_convert_greedy( &market, &money, &corn );

   ck_assert_int_eq( money.count, 0 );
   ck_assert_int_eq( corn.count, 3500 );
}
END_TEST

START_TEST( test_msim_convert_rollover ) {
   msim_result result = MSIM_OK;
   corn.count = UINT16_MAX - 5;

   result = msim_convert( &market, &money, &corn );

   ck_assert_int_eq( result, MSIM_RESULT_OUTPUT_OVERFLOW );
   ck_assert_int_eq( corn.count, UINT16_MAX - 5 );
   ck_assert_int_eq( money.count, 1000 );
}
END_TEST

Suite* msim_suite( void ) {
   Suite* s = NULL;
   TCase* tc_msim = NULL;

   s = suite_create( "msim" );

   tc_msim = tcase_create( "msim" );
   tcase_add_checked_fixture( tc_msim, setup_msim, teardown_msim );
   tcase_add_test( tc_msim, test_msim_convert );
   tcase_add_test( tc_msim, test_msim_convert_insufficient );
   tcase_add_test( tc_msim, test_msim_convert_greedy );
   tcase_add_test( tc_msim, test_msim_convert_rollover );

   suite_add_tcase( s, tc_msim );

   return s;
}

