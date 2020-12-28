#pragma once
#include "leveldata.h"
#include "tilemap.h"
#include "gameclass.h"


#define CARCOUNT_MAX      16


typedef class CLEVELMANAGER
{
  protected:
    
    /* running level management */
    uint16_t levelNumber = 0 ;
    uint8_t levelWidth = 10 ;
    uint8_t levelHeight = 10 ;
    
    uint8_t levelOffsetX = 0 ;
    uint8_t levelOffsetY = 0 ;
    
    EDIFFICULTY m_difficulty = eBEGINNER ;
    
    SLEVELENTITY m_carData[CARCOUNT_MAX];
    uint8_t m_carCount = 0 ;

    uint8_t m_selectedCar = 0;
    int8_t  m_selectedMoveOffsetX = 0 ;
    int8_t  m_selectedMoveOffsetY = 0 ;    
    
    uint16_t m_moves = 0 ;
    
    uint32_t m_levelTime = 0 ;                // time remembered needed for the current level (updated on pause / win)
    uint32_t m_timeOfResume = 0 ;             // time when the level resumed last (and counts) or 0 if time is not running for a level
    

    
    class CGAME * m_game ;
    
    CTILEMAP *m_levelTiles ;
    
  public:
    CLEVELMANAGER(class CGAME *game) ;
    
    void LoadLevel(uint16_t level) ;
    void UnloadLevel() ;
    
    void UpdateCarsOnScreen(bool selectorShown = true) ;
    void UpdateLevelStats() ;

    uint16_t GetLevelCountForDifficulty(const EDIFFICULTY targetDiff) ;
    uint16_t GetLevel(const EDIFFICULTY targetDiff, uint16_t index);
    
    void ResetLevel() ;
    
    void SelectNextCarLeft() ;
    void SelectNextCarRight() ;
    void SelectNextCarUp() ;
    void SelectNextCarDown() ;
    
    bool CanMoveLeft() ;
    bool CanMoveRight() ;
    bool CanMoveUp() ;
    bool CanMoveDown() ;    
    
    void MoveLeft() ;
    void MoveRight() ;
    void MoveUp() ;
    void MoveDown() ;
    
    void SetPartialMove(int mx, int my) ;

    uint8_t GetOccupyingCar(uint8_t x, uint8_t y) ;
    bool SelectCarFromTouch(touchPosition touch) ;
    
    bool IsLevelWon() ;
    
    uint16_t GetMoveCount() { return m_moves; };
    
    void ResetLevelTime() ;
    void PauseLevelTime() ;
    void ContinueLevelTime() ;
    
    uint32_t GetCurrentLevelTime() ;
    
    EDIFFICULTY GetDifficulty() { return m_difficulty; };
    EDIFFICULTY GetLoadedDifficulty() { return gLevels[levelNumber].difficulty; };
    void SetDifficulty(EDIFFICULTY diff) { m_difficulty = diff ; };
    

} CLEVELMANAGER ;