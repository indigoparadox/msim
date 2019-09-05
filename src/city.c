
#include "msim.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#define MCITY_RES_NONE  0
#define MCITY_RES_MONEY 1
#define MCITY_RES_WATER 2
#define MCITY_RES_POWER 3
#define MCITY_RES_DATA  4
#define MCITY_RES_POP   5
#define MCITY_RES_CRIME 6

#define MCITY_BLDG_PPLANT 0

uint8_t city_res_table[10][10] = {
   /* Power Plant */
   { 0, 50, 0, 0, 0, 0, 0, 0, 0, 0 }
};

struct mcity_building {
   struct msim_cvt engine;
   uint8_t x;
   uint8_t y;
};

int main( int argc, char** argv ) {
   struct msim_res money = { 0 };
   struct msim_res power = { 0 };
   struct mcity_building pplant = { { 0 } };
   struct msim_res res_defs[10] = { { 0 } };
   uint8_t year = 0;
   bool running = true;

   res_defs[0].count = 5;
   res_defs[0].id = MCITY_RES_MONEY;
   res_defs[0].next = NULL;

   res_defs[1].count = 10;
   res_defs[1].id = MCITY_RES_POWER;
   res_defs[1].next = NULL;

   pplant.engine.input = &(res_defs[0]);
   pplant.engine.output = &(res_defs[1]);

   money.count = 10000;
   money.id = MCITY_RES_MONEY;

   power.id = MCITY_RES_POWER;

   msim_convert( &(pplant.engine), &money, &power );

   while( running ) {
      printf( "Year: %d, Money: %d, Power: %d\n",
         year, money.count, power.count );
   }

   return 0;
}

