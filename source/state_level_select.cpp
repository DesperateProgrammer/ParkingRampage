#include "gameclass.h"
#include "state_level_select.h"
#include "parkingrampage_defines.h"
#include <nds.h>
#include <stdio.h>

CLEVELSELECTSTATE::CLEVELSELECTSTATE(CGAME *game) 
{
  m_game = game ;
}


bool CLEVELSELECTSTATE::OnEnter()
{
  m_game->StartFade(SCREEN_BOTTOM, eFADEOUT, 0) ;
  m_game->GetSubText()->Clear() ;
  m_game->GetSubSprites()->DisableAll() ;
  return true ;
}

bool CLEVELSELECTSTATE::OnLeave()
{
  m_game->GetMainText()->Clear() ;
  m_game->GetSubText()->Clear() ;  
  m_game->GetSubSprites()->DisableAll() ;
  return true ;
}

void CLEVELSELECTSTATE::UpdateLevelSelectionInfo()
{
  char buffer[32] ;
  char *diffText = 0;
  uint8_t pal = 0 ;
  switch (m_game->GetLevelManager()->GetLoadedDifficulty())
  {
    case eBEGINNER:
      diffText = (char *)"BEGINNER" ;
      pal = 3 ;
      break ;
    case eINTERMEDIATE:
      diffText = (char *)"INTERMEDIATE" ;
      pal = 5 ;
      break ;
    case eADVANCED:
      diffText = (char *)"ADVANCED" ;
      pal = 7 ;
      break ;
    case eEXPERT:
      diffText = (char *)"EXPERT" ;
      pal = 1 ;
      break ;
    case eANY:
      diffText = (char *)"ANY" ;
      pal = 9 ;
      break ;
  }  
  sprintf(buffer, "  %s - %i  ", diffText, (m_levelIndexInDifficulty+1));
  m_game->GetSubText()->EnableTextWindow(4*8, 9*16+8, 24*8, 2*16) ; 
  m_game->GetMainText()->SetBiColorText(16 - strlen(buffer) / 2, 2, buffer, pal, pal+1);
  
  uint32_t highMoves, highTime ;
  std::string highPlayer = "" ;
  if (m_game->GetLevelManager()->GetHighScore(m_game->GetLevelManager()->GetLevel(), highTime, highMoves, highPlayer))
  {
    sprintf(buffer, "%s: %li moves", highPlayer.c_str(), highMoves);
    m_game->GetMainText()->SetText(16 - strlen(buffer) / 2, 4, buffer);    
    uint32_t lTime = highTime ;
    sprintf(buffer, "%li.%li s", lTime / 1000, (lTime / 100) % 10) ;
    m_game->GetMainText()->SetText(16 - strlen(buffer) / 2, 5, buffer);
  } else
  {
    m_game->GetMainText()->SetText(10, 5, (char *)"No highscore");
  }

  m_game->GetSubText()->SetText(5, 10, (char *)"\x11 Return");
  m_game->GetSubText()->SetText(19, 10, (char *)"Start \x12");
  
  m_game->GetMainText()->SetText(7, 7, (char *)"\x14\x15\x16 Change level");
  m_game->GetMainText()->SetText(7, 9, (char *)"\x10\x13\x10 Start level");
  m_game->GetMainText()->EnableTextWindow(32, 24, 192, 144) ; 
}

bool CLEVELSELECTSTATE::OnTick()
{
  m_game->GetLevelManager()->UpdateCarsOnScreen(false);
  UpdateLevelSelectionInfo() ;
  if (!m_game->IsFading(SCREEN_BOTTOM))
  {
    if (m_game->GetFadeMode(SCREEN_BOTTOM) == eFADEOUT)
    {
      /* find a level */
      srand(m_game->GetTimeManager()->GetTimerTicks()) ;
      uint16_t diffCnt = m_game->GetLevelManager()->GetLevelCountForDifficulty(m_game->GetLevelManager()->GetDifficulty()) ;
      m_levelIndexInDifficulty = 0 ;
      if (diffCnt)
      {
        uint16_t level = m_game->GetLevelManager()->GetLevel(m_game->GetLevelManager()->GetDifficulty(), m_levelIndexInDifficulty) ;
        m_game->GetLevelManager()->LoadLevel(level)  ;
      } else
      {
        m_game->GetLevelManager()->LoadLevel(0) ;
      }
      m_game->StartFade(SCREEN_BOTTOM, eFADEIN, LEVEL_FADETIME) ;
    } else
    {
      uint16_t keys = m_game->GetInputManager()->GetKeysDown() ; ;
      if (keys & KEY_TOUCH)
      {
        if ((m_game->GetInputManager()->GetLastTouchDownPosition().py >= 9*16+8) && (m_game->GetInputManager()->GetLastTouchDownPosition().py < 11*16+8))
        {
          if ((m_game->GetInputManager()->GetLastTouchDownPosition().px >= 4*8) && (m_game->GetInputManager()->GetLastTouchDownPosition().px < 28*8))
          {
            if (m_game->GetInputManager()->GetLastTouchDownPosition().px < 128)
            {
              // Return
              m_game->GetLevelManager()->ResetLevelTime() ;
              m_sm->ChangeState(GAMESTATE_MAINMENU) ;                    
            } else
            {
              // Start
              m_game->GetLevelManager()->ResetLevelTime() ;
              m_game->GetLevelManager()->ResetLevel() ;
              m_sm->ChangeState(GAMESTATE_LEVELRUNNING) ;
            }
          }
        }
      }
      if (m_game->GetInputManager()->IsKeyForAlias(keys, KEYALIAS_SELECT_NEXT))
      {  
        m_game->GetSubSprites()->DisableAll() ;
        m_levelIndexInDifficulty = (m_levelIndexInDifficulty + 1) % m_game->GetLevelManager()->GetLevelCountForDifficulty(m_game->GetLevelManager()->GetDifficulty()) ;
        m_game->GetLevelManager()->LoadLevel(m_game->GetLevelManager()->GetLevel(m_game->GetLevelManager()->GetDifficulty(), m_levelIndexInDifficulty)) ;
      }
      if (m_game->GetInputManager()->IsKeyForAlias(keys, KEYALIAS_SELECT_PREVIOUS))
      {  
        m_game->GetSubSprites()->DisableAll() ;
        if (m_levelIndexInDifficulty)
          m_levelIndexInDifficulty = (m_levelIndexInDifficulty - 1) % m_game->GetLevelManager()->GetLevelCountForDifficulty(m_game->GetLevelManager()->GetDifficulty()) ;
        else
          m_levelIndexInDifficulty = m_game->GetLevelManager()->GetLevelCountForDifficulty(m_game->GetLevelManager()->GetDifficulty()) - 1;
        m_game->GetLevelManager()->LoadLevel(m_game->GetLevelManager()->GetLevel(m_game->GetLevelManager()->GetDifficulty(), m_levelIndexInDifficulty)) ;
      }
      if (m_game->GetInputManager()->IsKeyForAlias(keys, KEYALIAS_RETURN))
      {  
        m_game->GetLevelManager()->ResetLevelTime() ;
        m_sm->ChangeState(GAMESTATE_MAINMENU) ;
      }
      if (m_game->GetInputManager()->IsKeyForAlias(keys, KEYALIAS_SELECT_START))
      {
        m_game->GetLevelManager()->ResetLevelTime() ;
        m_game->GetMainText()->Clear();
        m_game->GetLevelManager()->ResetLevel();
        m_sm->ChangeState(GAMESTATE_LEVELRUNNING) ;
      }
      keys = m_game->GetInputManager()->GetKeysHeld() ;
      if (keys & KEY_TOUCH)
      {
        // Swipe Left & right?
        int16_t moveX = (int16_t)m_game->GetInputManager()->GetLastTouchDownPosition().px - m_game->GetInputManager()->GetLastTouchPosition().px ;
        bool handled = false ;
        if (moveX > 32)
        {
          m_game->GetSubSprites()->DisableAll() ;
          if (m_levelIndexInDifficulty)
            m_levelIndexInDifficulty = (m_levelIndexInDifficulty - 1) % m_game->GetLevelManager()->GetLevelCountForDifficulty(m_game->GetLevelManager()->GetDifficulty()) ;
          else
            m_levelIndexInDifficulty = m_game->GetLevelManager()->GetLevelCountForDifficulty(m_game->GetLevelManager()->GetDifficulty()) - 1;
          m_game->GetLevelManager()->LoadLevel(m_game->GetLevelManager()->GetLevel(m_game->GetLevelManager()->GetDifficulty(), m_levelIndexInDifficulty)) ;
          handled = true ;
        } else if (moveX < -32)
        {
          m_game->GetSubSprites()->DisableAll() ;
          m_levelIndexInDifficulty = (m_levelIndexInDifficulty + 1) % m_game->GetLevelManager()->GetLevelCountForDifficulty(m_game->GetLevelManager()->GetDifficulty()) ;
          m_game->GetLevelManager()->LoadLevel(m_game->GetLevelManager()->GetLevel(m_game->GetLevelManager()->GetDifficulty(), m_levelIndexInDifficulty)) ;                  
          handled = true ;
        }
        if (handled)
        {
          // reset point of touch down, so we will not move it instantly again but use
          // the new position as the start of movement
          m_game->GetInputManager()->AccountDrag();
        }
      }
    }
    return true ;
  }
  return true ;
}


