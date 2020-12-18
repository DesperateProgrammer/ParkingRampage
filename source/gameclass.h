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
#include "audiostreamig.h"
#include "inputmanager.h"
#include "statemachine.h"
#include <stdint.h>

#define CARCOUNT_MAX      16

#define GAMESTATE_LOADING         0
#define GAMESTATE_MAINMENU        1
#define GAMESTATE_LEVELLOADING    2
#define GAMESTATE_LEVELRUNNING    3
#define GAMESTATE_LEVELPAUSED     4
#define GAMESTATE_LEVELWON        5
#define GAMESTATE_LEVELSELECT     6


#define MAINMENUITEM_VOLUME       0
#define MAINMENUITEM_DIFFICULTY   1
#define MAINMENUITEM_START        2
#define MAINMENUITEM_SELECT       3
#define MAINMENUITEM_COUNT        4

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

    /* gamestate management */
    virtual bool OnStateTick() ;
    virtual bool OnStateEnter() ;
    virtual bool OnStateLeave() ;

    /* individual state code */
    void LevelRunning_EnterState() ;
    void LevelLoading_EnterState() ;
    void LevelPaused_EnterState() ;
    void LevelWon_EnterState() ;
    void LevelSelect_EnterState() ;
    void Loading_EnterState() ;
    void MainMenu_EnterState() ;

    void LevelRunning_LeaveState() ;
    void LevelLoading_LeaveState() ;
    void LevelPaused_LeaveState() ;
    void LevelWon_LeaveState() ;
    void LevelSelect_LeaveState() ;
    void Loading_LeaveState() ;
    void MainMenu_LeaveState() ;
    
    bool LevelRunning_Tick() ;
    bool LevelLoading_Tick() ;
    bool LevelPaused_Tick() ;
    bool LevelWon_Tick() ;
    bool LevelSelect_Tick() ;
    bool Loading_Tick() ;
    bool MainMenu_Tick() ;
    
    /* Time */
    static volatile uint32_t m_timeTicks ;    // time counter roughly 1ms/bit
    uint32_t m_levelTime = 0 ;                // time remembered needed for the current level (updated on pause / win)
    uint32_t m_timeOfResume = 0 ;             // time when the level resumed last (and counts) or 0 if time is not running for a level
    
    static void TimerTick() ;
    void InitializeTime() ;
    uint32_t GetTimerTicks() ;
    
    void ResetLevelTime() ;
    void PauseLevelTime() ;
    void ContinueLevelTime() ;
    
    uint32_t GetCurrentLevelTime() ;
    
    /* Input */
    CINPUTMANAGER m_input ;
    
    /* Level Selection & Search */
    EDIFFICULTY m_difficulty = eBEGINNER ;

    void UpdateLevelSelectionInfo() ;
    uint16_t m_levelIndexInDifficulty ;
    
    uint16_t GetLevelCountForDifficulty(const EDIFFICULTY targetDiff) ;
    uint16_t GetLevel(const EDIFFICULTY targetDiff, uint16_t index);

    void IncreaseDifficulty() ;
    void DecreaseDifficulty() ;
    
    /* running level management */
    uint16_t levelNumber = 0 ;
    uint8_t levelWidth = 10 ;
    uint8_t levelHeight = 10 ;
    
    uint8_t levelOffsetX = 0 ;
    uint8_t levelOffsetY = 0 ;

    uint16_t m_moves = 0 ;
    
    SLEVELENTITY m_carData[CARCOUNT_MAX];
    
    uint8_t m_selectedCar = 0;
    uint8_t m_carCount = 0 ;

    void StartLevel() ;

    void LoadLevel(uint16_t level) ;
    void UnloadLevel() ;
    
    void SelectNextCarLeft() ;
    void SelectNextCarRight() ;
    void SelectNextCarUp() ;
    void SelectNextCarDown() ;
    
    bool CanMoveLeft() ;
    bool CanMoveRight() ;
    bool CanMoveUp() ;
    bool CanMoveDown() ;    

    uint8_t GetOccupyingCar(uint8_t x, uint8_t y) ;
    
    bool IsGameWon() ;
    
    /* Audio */
    uint16_t m_musicVolume = 768 ;
    CAUDIOSTREAMING *m_audio ;
    
    /* Graphics & Text */
    /* Tiled Backgrounds */
    CTILEMAP *m_mainScreenTiles ;
    CTILEMAP *m_mainMenuTiles ;
    CTILEMAP *m_titleTiles ;
    CTILEMAP *m_levelTiles ;
    
    /* Text background */
    CTEXTOVERLAY *m_mainText ;
    CTEXTOVERLAY *m_subText ;
    
    /* Sprites */
    /* Car data */
    uint16_t *m_spriteContent[16];
    void LoadCarSprites() ;
    void DisableAllSprites() ;

    /* Effects */
    /* Fading state for both screens */
    uint32_t m_fadeStart[2];
    uint32_t m_fadeEnd[2];
    EFADE m_fadeMode[2] ;
    
    void UpdateFading(uint8_t screen) ;
    void StartFade(uint8_t screen, EFADE mode, uint32_t timespan) ;
    EFADE GetFadeMode(uint8_t screen) ;
    bool IsFading(uint8_t screen) ;
        
    /* Rotate out/Rotate in */
    uint32_t m_rotscaleStart[2];
    uint32_t m_rotscaleEnd[2];
    // The following rotscale values are 4.12 fixed point
    uint16_t m_startScale[2] ;
    uint16_t m_endscale[2] ;
    int16_t m_rotations[2] ;
    
    void UpdateRotScale(uint8_t screen) ;
    void StartRotScale(uint8_t screen, uint32_t timespan, int16_t rotations, uint16_t startScale, uint16_t endScale) ;
    bool IsRotScaling(uint8_t screen) ;
    
    /* Main Menu special methods */
    uint8_t m_mainMenuItem = MAINMENUITEM_START ;
    void ShowMainMenuDifficulty() ;
    void ShowMainMenuStart() ;
    void ShowMainMenuSelector() ;
    void ShowMainMenuMusicVolume() ;
    
    /* Level Running/Paused/Won methods */
    void UpdateLevelStats() ;
    void UpdateCarsOnScreen(bool selectorShown = true) ;
    
  public:
    void Run();
    
    static class CGAME *GetInstance() { return m_instance; };

} CGAME, LPCGAME;