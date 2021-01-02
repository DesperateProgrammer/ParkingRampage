#include "gameclass.h"
#include "state_level_won.h"
#include <nds.h>
#include <stdio.h>
#include <string.h>
#include "soundbank.h"

CLEVELWONSTATE::CLEVELWONSTATE(CGAME *game) 
{
  m_game = game ;
}

bool CLEVELWONSTATE::OnEnter()
{
  m_game->GetLevelManager()->PauseLevelTime() ;
  
  m_isHighscore = false ;
  uint32_t highMoves, highTime ;
  std::string highPlayer = "" ;
  if (!m_game->GetLevelManager()->GetHighScore(m_game->GetLevelManager()->GetLevel(), highTime, highMoves, highPlayer))
  {
    m_isHighscore = true ;
  } else
  {
    if (m_game->GetLevelManager()->GetMoveCount() < highMoves)
    {
      // lesser moves -> highscore
      m_isHighscore = true ;
    } else if (m_game->GetLevelManager()->GetMoveCount() == highMoves)
    {
      // equal moves but faster? -> high score
      if (m_game->GetLevelManager()->GetCurrentLevelTime() < highTime)
      {
        m_isHighscore = true ;
      }
    }
  }
  if (m_isHighscore)
  {
    highMoves = m_game->GetLevelManager()->GetMoveCount() ;
    highTime = m_game->GetLevelManager()->GetCurrentLevelTime() ;
    highPlayer = "Best" ; // TODO: Get name from DS Settings
    m_game->GetLevelManager()->SetHighScore(m_game->GetLevelManager()->GetLevel(), highTime, highMoves, highPlayer) ;
  }
  
  m_game->GetSubText()->Clear() ;
  if (m_isHighscore)
  {
    m_game->GetSubText()->SetText(11, 5, (char *)"HIGHSCORE!", TEXTCOLOR_RAINBOW) ;
  } else
  {
    m_game->GetSubText()->SetText(12, 5, (char *)"You won!", TEXTCOLOR_RAINBOW) ;
  }
  char buffer[32] ;
  uint32_t lTime = m_game->GetLevelManager()->GetCurrentLevelTime() ;
  snprintf(buffer, 32, "%i moves in %li.%li s", m_game->GetLevelManager()->GetMoveCount(), lTime / 1000, (lTime / 100) % 10) ;
  m_game->GetSubText()->SetText((256 - strlen(buffer)*8) / 16, 7, buffer, m_isHighscore?9:0) ;
  m_game->GetSubText()->EnableTextWindow(32, 64, 192, 80) ;
  m_game->GetAudio()->PlayEffect(SFX_APPLAUSE) ;
  return true ;
}

bool CLEVELWONSTATE::OnLeave()
{
  m_game->GetMainText()->Clear() ;
  m_game->GetSubText()->Clear() ;  
  m_game->GetSubSprites()->DisableAll() ;
  return true ;
}

bool CLEVELWONSTATE::OnTick()
{
  if (m_game->GetInputManager()->GetKeysDown())
  {
    m_game->StartFade(SCREEN_BOTTOM, eFADEOUT, 1000) ;
    return true ;
  }
  if (!m_game->IsFading(SCREEN_BOTTOM))
  {
    if (m_game->GetFadeMode(SCREEN_BOTTOM) == eFADEIN)
    {
    } else
    {
      m_sm->ChangeState(GAMESTATE_MAINMENU) ;
    }
  }  return true ;
}


