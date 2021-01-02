#include "defaultKeyMapping.h"
#include "parkingrampage_defines.h"
#include <nds.h>

const uint16_t gDefaultKeyMapping[] = 
{
  KEYALIAS_MENU_UP              , KEY_UP | KEY_X,
  KEYALIAS_MENU_DOWN            , KEY_DOWN | KEY_B,
  KEYALIAS_MENU_MODIFY_MINUS    , KEY_LEFT | KEY_Y | KEY_L | KEY_SELECT,
  KEYALIAS_MENU_MODIFY_PLUS     , KEY_RIGHT | KEY_A | KEY_R | KEY_START,
  KEYALIAS_PAUSE                , KEY_START,
  KEYALIAS_RETURN               , KEY_SELECT,
  KEYALIAS_MOVE_LEFT            , KEY_LEFT,
  KEYALIAS_MOVE_RIGHT           , KEY_RIGHT,
  KEYALIAS_MOVE_UP              , KEY_UP,
  KEYALIAS_MOVE_DOWN            , KEY_DOWN,
  KEYALIAS_SELECT_LEFT          , KEY_Y,
  KEYALIAS_SELECT_RIGHT         , KEY_A,
  KEYALIAS_SELECT_UP            , KEY_X,
  KEYALIAS_SELECT_DOWN          , KEY_B,
  KEYALIAS_SELECT_PREVIOUS      , KEY_LEFT | KEY_L,
  KEYALIAS_SELECT_NEXT          , KEY_RIGHT | KEY_R,
  KEYALIAS_SELECT_START         , KEY_START | KEY_B | KEY_A | KEY_X | KEY_Y ,
  0, 0
} ;

const uint16_t gDefaultKeyMapping2[] = 
{
  KEYALIAS_MENU_UP              , KEY_UP | KEY_X,
  KEYALIAS_MENU_DOWN            , KEY_DOWN | KEY_B,
  KEYALIAS_MENU_MODIFY_MINUS    , KEY_LEFT | KEY_Y | KEY_L | KEY_SELECT,
  KEYALIAS_MENU_MODIFY_PLUS     , KEY_RIGHT | KEY_A | KEY_R | KEY_START,
  KEYALIAS_PAUSE                , KEY_START,
  KEYALIAS_RETURN               , KEY_SELECT,
  KEYALIAS_SELECT_LEFT          , KEY_LEFT,
  KEYALIAS_SELECT_RIGHT         , KEY_RIGHT,
  KEYALIAS_SELECT_UP            , KEY_UP,
  KEYALIAS_SELECT_DOWN          , KEY_DOWN,
  KEYALIAS_MOVE_LEFT            , KEY_Y,
  KEYALIAS_MOVE_RIGHT           , KEY_A,
  KEYALIAS_MOVE_UP              , KEY_X,
  KEYALIAS_MOVE_DOWN            , KEY_B,
  KEYALIAS_SELECT_PREVIOUS      , KEY_LEFT | KEY_L,
  KEYALIAS_SELECT_NEXT          , KEY_RIGHT | KEY_R,
  KEYALIAS_SELECT_START         , KEY_START | KEY_B | KEY_A | KEY_X | KEY_Y ,
  0, 0
} ;

const uint16_t *gPredefinedKeyMappings[] = { &gDefaultKeyMapping[0], &gDefaultKeyMapping2[0], 0} ;
