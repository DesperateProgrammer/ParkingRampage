/******************************************************************************
*
*   
*
*
*
 ******************************************************************************/

#pragma once

#include "textoverlay.h"
#include "audiostreaming.h"
#include "levelmanager.h"
#include "inputmanager.h"
#include "timemanager.h"
#include "spritemanager.h"
#include "statemachine.h"
#include "parkingrampage_defines.h"
#include "gamebase.h"
#include <string>
#include <stdint.h>

#define GAMESTATE_LOADING         0
#define GAMESTATE_MAINMENU        1
#define GAMESTATE_LEVELLOADING    2
#define GAMESTATE_LEVELRUNNING    3
#define GAMESTATE_LEVELPAUSED     4
#define GAMESTATE_LEVELWON        5
#define GAMESTATE_LEVELSELECT     6
#define GAMESTATE_CONFIG          7

#define SCREEN_TOP                0
#define SCREEN_BOTTOM             1

#define TITLE_FADETIME                  3000
#define MAINMENU_FADETIME               1000
#define LEVEL_FADETIME                  500

#define SPRITE_CAR_A              0
#define SPRITE_CAR_TARGET         1
#define SPRITE_CAR_B              2
#define SPRITE_CAR_C              3
#define SPRITE_CAR(n)             ((n) & 3)
#define SPRITE_TRUCK_A_1          4
#define SPRITE_TRUCK_A_2          5
#define SPRITE_TRUCK_B_1          6
#define SPRITE_TRUCK_B_2          7
#define SPRITE_TRUCK_1(n)         (((n) & 1) * 2 + 4)
#define SPRITE_TRUCK_2(n)         (((n) & 1) * 2 + 5)
#define SPRITE_BUS_1              8
#define SPRITE_BUS_2              9
#define SPRITE_TANKTRUCK_1       10
#define SPRITE_TANKTRUCK_2       11
#define SPRITE_SELECTOR          12

typedef class CGAME : public CGAMEBASE
{
  protected:
    /* Singleton */
    static class CGAME *m_instance ;

    /* Level */
    class CLEVELMANAGER *m_levelManager ;
    
    /* Sprites */
    void LoadCarSprites() ;

  public:
    /* Singleton */
    static class CGAME *GetInstance() { return m_instance; };
    
    void Initialize() ;

    CLEVELMANAGER *GetLevelManager() { return m_levelManager; } ;

} CGAME, LPCGAME;