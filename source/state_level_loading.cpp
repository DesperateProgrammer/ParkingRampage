#include "gameclass.h"
#include <nds.h>

void CGAME::LevelLoading_EnterState()
{
  m_selectedCar = 0 ;
  m_moves = 0 ;
  ResetLevelTime() ;
  m_subText->Clear() ;
}

void CGAME::LevelLoading_LeaveState()
{
}

bool CGAME::LevelLoading_Tick()
{
  if (!IsFading(SCREEN_BOTTOM))
  {
    if (GetFadeMode(SCREEN_BOTTOM) == eFADEOUT)
    {
      m_levelTiles->Initialize() ;
      /* find a level */
      srand(GetTimerTicks()) ;
      uint16_t diffCnt = GetLevelCountForDifficulty(m_difficulty) ;
      if (diffCnt)
      {
        uint16_t level = GetLevel(m_difficulty, rand() % diffCnt) ;
        LoadLevel(level)  ;
      } else
      {
        LoadLevel(0) ;
      }
      StartFade(SCREEN_BOTTOM, eFADEIN, LEVEL_FADETIME) ;
    } else
    {
      ChangeState(GAMESTATE_LEVELRUNNING) ;
    }
    return true ;
  }
  return true ;
}


