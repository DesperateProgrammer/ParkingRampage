/******************************************************************************
*
*   
*
*
*
 ******************************************************************************/

#pragma once

#include "leveldata.h"
#include "tilemap.h"
#include "textoverlay.h"
#include "audiostreaming.h"
#include "levelmanager.h"
#include "inputmanager.h"
#include "timemanager.h"
#include "statemachine.h"
#include <stdint.h>

#define GAMESTATE_LOADING         0
#define GAMESTATE_MAINMENU        1
#define GAMESTATE_LEVELLOADING    2
#define GAMESTATE_LEVELRUNNING    3
#define GAMESTATE_LEVELPAUSED     4
#define GAMESTATE_LEVELWON        5
#define GAMESTATE_LEVELSELECT     6

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

typedef enum EFADE
{
  eFADEIN,
  eFADEOUT,
} EFADE;

typedef class CGAME : public CSTATEMACHINE
{
  protected:
    /* Singleton */
    static class CGAME *m_instance ;

    void Initialize() ;
    void Destruct() ;
    
    virtual void OnEveryTick() ;
            
    
    /* Various managers */
    /* Time */
    CTIMEMANAGER *m_time ;
    /* Input (Key + Touch) */
    CINPUTMANAGER m_input ;
    /* Audio */
    CAUDIOSTREAMING *m_audio ;        
    /* Text background */
    CTEXTOVERLAY *m_mainText ;
    CTEXTOVERLAY *m_subText ;
    /* Level */
    class CLEVELMANAGER *m_levelManager ;
    
    
    
    /* Level Selection & Search */
    EDIFFICULTY m_difficulty = eBEGINNER ;
                
    
    
    /* Sprites */
    uint16_t *m_spriteContent[16];
    void LoadCarSprites() ;

    /* Effects */
    /* Fading state for both screens */
    uint32_t m_fadeStart[2];
    uint32_t m_fadeEnd[2];
    EFADE m_fadeMode[2] ;
    
    void UpdateFading(uint8_t screen) ;

        
    /* Rotate out/Rotate in */
    uint32_t m_rotscaleStart[2];
    uint32_t m_rotscaleEnd[2];
    // The following rotscale values are 4.12 fixed point
    uint16_t m_startScale[2] ;
    uint16_t m_endscale[2] ;
    int16_t m_rotations[2] ;

    void UpdateRotScale(uint8_t screen) ;
    
    /* Level Running/Paused/Won methods */
    
  public:
    void Run();
    
    static class CGAME *GetInstance() { return m_instance; };
    
    CINPUTMANAGER *GetInputManager() { return &m_input ; } ;
    CAUDIOSTREAMING * GetAudio() { return m_audio; };
    CTEXTOVERLAY *GetMainText() { return m_mainText ;} ;
    CTEXTOVERLAY *GetSubText() { return m_subText ;} ;
    class CLEVELMANAGER *GetLevelManager() { return m_levelManager; } ;
    CTIMEMANAGER *GetTimeManager() { return m_time; };

    
    /* Effects */
    /* Fading state for both screens */
    void StartFade(uint8_t screen, EFADE mode, uint32_t timespan) ;
    EFADE GetFadeMode(uint8_t screen) ;
    bool IsFading(uint8_t screen) ;        
    /* Rotate out/Rotate in */
    void StartRotScale(uint8_t screen, uint32_t timespan, int16_t rotations, uint16_t startScale, uint16_t endScale) ;
    bool IsRotScaling(uint8_t screen) ;
    
    void DisableAllSprites() ;
    void IncreaseDifficulty() ;
    void DecreaseDifficulty() ;
    
    EDIFFICULTY GetDifficulty() { return m_difficulty; };

    
    uint16_t *GetSpriteLocation(uint8_t num) { return m_spriteContent[num] ; };
    



} CGAME, LPCGAME;