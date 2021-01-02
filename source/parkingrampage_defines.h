#pragma once

/* KEY Mappings */

#define KEYALIAS_MENU_UP              1
#define KEYALIAS_MENU_DOWN            2
#define KEYALIAS_MENU_MODIFY_MINUS    3
#define KEYALIAS_MENU_MODIFY_PLUS     4
#define KEYALIAS_PAUSE                6
#define KEYALIAS_RETURN               7
#define KEYALIAS_MOVE_LEFT            8
#define KEYALIAS_MOVE_RIGHT           9
#define KEYALIAS_MOVE_UP              10
#define KEYALIAS_MOVE_DOWN            11
#define KEYALIAS_SELECT_LEFT          12
#define KEYALIAS_SELECT_RIGHT         13
#define KEYALIAS_SELECT_UP            14
#define KEYALIAS_SELECT_DOWN          15
#define KEYALIAS_SELECT_NEXT          16
#define KEYALIAS_SELECT_PREVIOUS      17
#define KEYALIAS_SELECT_START         18

#define KEYMAPPING_DPADTOMOVE         0
#define KEYMAPPING_DPADTOSELECT       1

/* Tags for configuration storage */

#define CFGTAG_VOLUME_MUSIC           0
#define CFGTAG_VOLUME_SFX             1
#define CFGTAG_VOLUME_KEYMAPPING      2
#define CFGTAG_HIGHSCORES             (TAG_FLAG_RECURSIVE | 3)
#define CFGTAG_LEVELDATA(hash)        (TAG_FLAG_RECURSIVE | hash)
#define CFGTAG_LEVELMOVES             0
#define CFGTAG_LEVELTIME              1
#define CFGTAG_PLAYER                 2