
#include <check.h>
#include <stdint.h>
#include <stdlib.h>

#include "../src/msim.h"

#define RES_MONEY 0
#define RES_CORN 1
#define RES_BERRIES 2
#define RES_TEA 3
#define RES_OLIVES 4
#define RES_COUNT 5

#define STARTING_MONEY     1000
#define STARTING_CORN      500
#define STARTING_BERRIES   400
#define STARTING_TEA       50
#define STARTING_OLIVES    (UINT16_MAX - 5)

#define PRICE_BERRIES      20
#define PRICE_CORN         10
#define PRICE_TEA          100
#define PRICE_OLIVES       20

const struct msim_res_def res_defs[RES_COUNT] = {
/* id             decay */
   { RES_MONEY,   0 },
   { RES_CORN,    0 },
   { RES_BERRIES, 0 },
   { RES_TEA,     0 },
   { RES_OLIVES,  0 }
};

struct msim_res resources[RES_COUNT] = {
/* definition                    count */
   { &(res_defs[RES_MONEY]),     0 },  
   { &(res_defs[RES_CORN]),      0 },  
   { &(res_defs[RES_BERRIES]),   0 },  
   { &(res_defs[RES_TEA]),       0 },  
   { &(res_defs[RES_OLIVES]),    0 }
};

const struct msim_res berry_buyer_in = { &(res_defs[RES_BERRIES]), 20 };
const struct msim_res berry_buyer_out = { &(res_defs[RES_MONEY]), 5 };
const struct msim_converter berry_buyer =
   { &berry_buyer_in, 1, &berry_buyer_out, 1, 0, 0 };

const struct msim_res corn_buyer_in = { &(res_defs[RES_CORN]), 30 };
const struct msim_res corn_buyer_out = { &(res_defs[RES_MONEY]), 10 };
const struct msim_converter corn_buyer =
   { &corn_buyer_in, 1, &corn_buyer_out, 1, 0, 0 };

const struct msim_res tea_buyer_in = { &(res_defs[RES_TEA]), 60 };
const struct msim_res tea_buyer_out = { &(res_defs[RES_MONEY]), 100 };
const struct msim_converter tea_buyer =
   { &tea_buyer_in, 1, &tea_buyer_out, 1, 0, 0 };

const struct msim_res olive_seller_in = { &(res_defs[RES_MONEY]), 50 };
const struct msim_res olive_seller_out = { &(res_defs[RES_OLIVES]), 100 };
const struct msim_converter olive_seller =
   { &olive_seller_in, 1, &olive_seller_out, 1, 0, 0 };

const struct msim_res olive_corn_buyer_in[2] =
   { { &(res_defs[RES_OLIVES]), 100 }, { &(res_defs[RES_CORN]), 30 } };
const struct msim_res olive_corn_buyer_out = { &(res_defs[RES_MONEY]), 300 };
const struct msim_converter olive_corn_buyer =
   { olive_corn_buyer_in, 2, &olive_corn_buyer_out, 1, 0, 0 };

static void setup_msim() {
   resources[RES_MONEY].count = STARTING_MONEY;

   resources[RES_BERRIES].count = STARTING_BERRIES;

   resources[RES_CORN].count = STARTING_CORN;

   resources[RES_TEA].count = STARTING_TEA;
}

static void teardown_msim() {
}

START_TEST( test_msim_convert ) {
   msim_result result = MSIM_OK;

   result = msim_convert( &corn_buyer, resources, RES_COUNT );

   ck_assert_int_eq( result, MSIM_OK );
   ck_assert_int_eq( resources[RES_MONEY].count, STARTING_MONEY + PRICE_CORN );
   ck_assert_int_eq( resources[RES_CORN].count, STARTING_CORN - 30 );
   ck_assert_int_eq( resources[RES_BERRIES].count, STARTING_BERRIES );
   ck_assert_int_eq( resources[RES_TEA].count, STARTING_TEA );
}
END_TEST

START_TEST( test_msim_convert_insufficient ) {
   msim_result result = MSIM_OK;

   result = msim_convert( &tea_buyer, resources, RES_COUNT );

   ck_assert_int_eq( result, MSIM_RESULT_NOT_ENOUGH_INPUT );
   ck_assert_int_eq( resources[RES_MONEY].count, STARTING_MONEY );
   ck_assert_int_eq( resources[RES_CORN].count, STARTING_CORN );
   ck_assert_int_eq( resources[RES_BERRIES].count, STARTING_BERRIES );
   ck_assert_int_eq( resources[RES_TEA].count, STARTING_TEA );
}
END_TEST

START_TEST( test_msim_convert_greedy ) {
   msim_convert_greedy( &corn_buyer, resources, RES_COUNT );

   ck_assert_int_eq( resources[RES_MONEY].count,
      STARTING_MONEY + ((STARTING_CORN / 30) * PRICE_CORN) );
   ck_assert_int_eq( resources[RES_CORN].count, STARTING_CORN % 30 );
   ck_assert_int_eq( resources[RES_BERRIES].count, STARTING_BERRIES );
   ck_assert_int_eq( resources[RES_TEA].count, STARTING_TEA );
}
END_TEST

START_TEST( test_msim_convert_rollover ) {
   msim_result result = MSIM_OK;

   result = msim_convert( &olive_seller, resources, RES_COUNT );

   ck_assert_int_eq( resources[RES_OLIVES].count, STARTING_OLIVES );
   ck_assert_int_eq( resources[RES_MONEY].count, STARTING_MONEY );
   ck_assert_int_eq( resources[RES_BERRIES].count, STARTING_BERRIES );
   ck_assert_int_eq( result, MSIM_RESULT_OUTPUT_OVERFLOW );
   ck_assert_int_eq( resources[RES_TEA].count, STARTING_TEA );
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

