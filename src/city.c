
#include "msim.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#define MCITY_RES_MONEY 0
#define MCITY_RES_WATER 1
#define MCITY_RES_POWER 2
#define MCITY_RES_DATA  3
#define MCITY_RES_POP   4
#define MCITY_RES_CRIME 5
#define MCITY_RES_MAX   6

#define MCITY_BLDG_PPLANT  0
#define MCITY_BLDG_RES     1
#define MCITY_BLDG_COM     2
#define MCITY_BLDG_IND     3
#define MCITY_BLDG_PARK    4

struct mcity_building {
   const struct msim_converter* engine;
   uint8_t id;
   uint8_t bhs; /* business (C), happiness (R), shipping (I) */
   uint8_t x;
   uint8_t y;
};

const struct msim_res_def res_defs[MCITY_RES_MAX] = {
/* id                   decay */
   { MCITY_RES_MONEY,   0 },
   { MCITY_RES_WATER,   0 },
   { MCITY_RES_POWER,   0 },
   { MCITY_RES_DATA,    0 },
   { MCITY_RES_POP,     0 },
   { MCITY_RES_CRIME,   0 } 
};

const struct msim_res pplant_input_money = { &(res_defs[MCITY_RES_MONEY]), 20 };
const struct msim_res pplant_output_power =
   { &(res_defs[MCITY_RES_POWER]), 10 };
const struct msim_converter pplant_engine =
   { &pplant_input_money, 1, &pplant_output_power, 1, 100, 0 };

int main( int argc, char** argv ) {
   struct mcity_building pplant = { &pplant_engine, 10, 10 };
   uint8_t day = 0;
   uint8_t month = 0;
   uint8_t year = 0;
   bool running = true;
   struct msim_res resources[MCITY_RES_MAX] = {
   /* definition                       count */
      { &(res_defs[MCITY_RES_MONEY]),  10000 },
      { &(res_defs[MCITY_RES_WATER]),  0 },
      { &(res_defs[MCITY_RES_POWER]),  0 },
      { &(res_defs[MCITY_RES_DATA]),   0 },
      { &(res_defs[MCITY_RES_POP]),    0 },
      { &(res_defs[MCITY_RES_CRIME]),  0 }
   };

   while( running ) {
      msim_convert( pplant.engine, resources, MCITY_RES_MAX );
      printf( "Year: %d, Money: %d, Power: %d\n", year,
         resources[MCITY_RES_MONEY].count, resources[MCITY_RES_POWER].count );

      day++;
      if( 30 <= day ) {
         month++;
      }
      if( 12 <= month ) {
         month = 0;
         year++;
      }
      if( 200 <= year ) {
         running = false;
      }
   }

   return 0;
}

