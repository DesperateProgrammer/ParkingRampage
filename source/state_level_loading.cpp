#include "gameclass.h"
#include "state_level_loading.h"
#include <nds.h>

CLEVELLOADINGSTATE::CLEVELLOADINGSTATE(CGAME *game) 
{
  m_game = game ;
}

bool CLEVELLOADINGSTATE::OnEnter() 
{
  m_game->GetLevelManager()->ResetLevel() ;
  m_game->GetLevelManager()->ResetLevelTime() ;
  m_game->GetSubText()->Clear() ;
  return true ;
}

bool CLEVELLOADINGSTATE::OnTick()  
{
  if (!m_game->IsFading(SCREEN_BOTTOM))
  {
    if (m_game->GetFadeMode(SCREEN_BOTTOM) == eFADEOUT)
    {
      
      /* find a level */
      srand(m_game->GetTimeManager()->GetTimerTicks()) ;
      uint16_t diffCnt = m_game->GetLevelManager()->GetLevelCountForDifficulty(m_game->GetLevelManager()->GetDifficulty()) ;
      if (diffCnt)
      {
        uint16_t level = m_game->GetLevelManager()->GetLevel(m_game->GetLevelManager()->GetDifficulty(), rand() % diffCnt) ;
        m_game->GetLevelManager()->LoadLevel(level)  ;
      } else
      {
        m_game->GetLevelManager()->LoadLevel(0) ;
      }
      m_game->StartFade(SCREEN_BOTTOM, eFADEIN, LEVEL_FADETIME) ;
    } else
    {
      m_sm->ChangeState(GAMESTATE_LEVELRUNNING) ;
    }
    return true ;
  }
  return true ;
}


