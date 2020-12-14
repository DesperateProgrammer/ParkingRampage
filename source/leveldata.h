#pragma once

#include <stdint.h>

#define SPECIALS_NONE       0
#define SPECIALS_TARGET     1
#define SPECIALS_IMMOVABLE  2

#define ORIENTATION_DOWN    0
#define ORIENTATION_RIGHT   1
#define ORIENTATION_UP      2
#define ORIENTATION_LEFT    3

#define CAR_A               0
#define CAR_TARGET          1
#define CAR_B               2
#define CAR_C               3
#define TRUCK_A             0
#define TRUCK_B             1
#define TRUCK_C             1
#define BUS                 0
#define LONGTRUCK           1

#define LENGTH_2            1
#define LENGTH_3            2
#define LENGTH_4            3

typedef struct SLEVELENTITY
{
  uint16_t orientation:2;
  uint16_t specials:4;
  uint16_t size:2;
  uint16_t x:4;
  uint16_t y:4;
} SLEVELENTITY ;

typedef enum EDIFFICULTY
{
  eBEGINNER,
  eINTERMEDIATE,
  eADVANCED,
  eEXPERT
} EDIFFICULTY ;

typedef struct SLEVELDATA
{
  uint8_t width:4;
  uint8_t height:4;
  EDIFFICULTY difficulty ;
  uint8_t  entityCount ;
  SLEVELENTITY entities[16] ;
} SLEVELDATA ;

extern SLEVELDATA gLevels[] ;