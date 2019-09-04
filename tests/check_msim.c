
#include <check.h>
#include <stdint.h>

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

   market.input = RES_MONEY;
   market.input_count = 10;
   market.flags = 0;
   market.output = RES_CORN;
   market.output_count = 30;
}

static void teardown_msim() {
}

START_TEST( test_msim_convert ) {
   msim_convert( &market, &money, &corn );

   ck_assert_int_eq( money.count, 990 );
   ck_assert_int_eq( corn.count, 530 );
}
END_TEST

Suite* msim_suite( void ) {
   Suite* s = NULL;
   TCase* tc_msim = NULL;

   s = suite_create( "msim" );

   tc_msim = tcase_create( "msim" );
   tcase_add_checked_fixture( tc_msim, setup_msim, teardown_msim );
   tcase_add_test( tc_msim, test_msim_convert );

   suite_add_tcase( s, tc_msim );

   return s;
}

