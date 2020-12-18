#include "gameclass.h"
#include <nds.h>
#include <stdio.h>

void CGAME::LevelSelect_EnterState()
{
  StartFade(SCREEN_BOTTOM, eFADEOUT, 0) ;
  m_levelTiles->Initialize() ;
  m_subText->Clear() ;
  DisableAllSprites() ;
}

void CGAME::LevelSelect_LeaveState()
{
  m_mainText->Clear() ;
  m_subText->Clear() ;  
  DisableAllSprites() ;
}

void CGAME::UpdateLevelSelectionInfo()
{
  char buffer[32] ;
  char *diffText = 0;
  uint8_t pal = 0 ;
  switch (m_difficulty)
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
  }  
  sprintf(buffer, "  %s - %i  ", diffText, (m_levelIndexInDifficulty+1));
  m_subText->EnableTextWindow(4*8, 9*16+8, 24*8, 2*16) ; 
  m_mainText->SetBiColorText(16 - strlen(buffer) / 2, 2, buffer, pal, pal+1);
  m_subText->SetText(5, 10, (char *)"\x11 Return");
  m_subText->SetText(19, 10, (char *)"Start \x12");
  
  m_mainText->SetText(7, 5, (char *)"\x14\x15\x16 Change level");
  m_mainText->SetText(7, 7, (char *)"\x10\x13\x10 Start level");
  m_mainText->EnableTextWindow(32, 24, 192, 128) ; 
}

bool CGAME::LevelSelect_Tick()
{
  UpdateCarsOnScreen(false);
  UpdateLevelSelectionInfo() ;
  if (!IsFading(SCREEN_BOTTOM))
  {
    if (GetFadeMode(SCREEN_BOTTOM) == eFADEOUT)
    {
      m_levelTiles->Initialize() ;
      /* find a level */
      srand(GetTimerTicks()) ;
      uint16_t diffCnt = GetLevelCountForDifficulty(m_difficulty) ;
      m_levelIndexInDifficulty = 0 ;
      if (diffCnt)
      {
        uint16_t level = GetLevel(m_difficulty, m_levelIndexInDifficulty) ;
        LoadLevel(level)  ;
      } else
      {
        LoadLevel(0) ;
      }
      StartFade(SCREEN_BOTTOM, eFADEIN, LEVEL_FADETIME) ;
    } else
    {
      uint16_t keys = m_input.GetKeysDown() ; ;
      if (keys & KEY_TOUCH)
      {
        if ((m_input.GetLastTouchDownPosition().py >= 9*16+8) && (m_input.GetLastTouchDownPosition().py < 11*16+8))
        {
          if ((m_input.GetLastTouchDownPosition().px >= 4*8) && (m_input.GetLastTouchDownPosition().px < 28*8))
          {
            if (m_input.GetLastTouchDownPosition().px < 128)
            {
              // Return
              ResetLevelTime() ;
              ChangeState(GAMESTATE_MAINMENU) ;                    
            } else
            {
              // Start
              ResetLevelTime() ;
              m_moves = 0 ;
              ChangeState(GAMESTATE_LEVELRUNNING) ;
            }
          }
        }
      }
      if (m_input.IsKeyForAlias(keys, KEYALIAS_SELECT_NEXT))
      {  
        DisableAllSprites() ;
        m_levelIndexInDifficulty = (m_levelIndexInDifficulty + 1) % GetLevelCountForDifficulty(m_difficulty) ;
        LoadLevel(GetLevel(m_difficulty, m_levelIndexInDifficulty)) ;
      }
      if (m_input.IsKeyForAlias(keys, KEYALIAS_SELECT_PREVIOUS))
      {  
        DisableAllSprites() ;
        if (m_levelIndexInDifficulty)
          m_levelIndexInDifficulty = (m_levelIndexInDifficulty - 1) % GetLevelCountForDifficulty(m_difficulty) ;
        else
          m_levelIndexInDifficulty = GetLevelCountForDifficulty(m_difficulty) - 1;
        LoadLevel(GetLevel(m_difficulty, m_levelIndexInDifficulty)) ;
      }
      if (m_input.IsKeyForAlias(keys, KEYALIAS_RETURN))
      {  
        ResetLevelTime() ;
        ChangeState(GAMESTATE_MAINMENU) ;
      }
      if (m_input.IsKeyForAlias(keys, KEYALIAS_SELECT_START))
      {
        ResetLevelTime() ;
        m_mainText->Clear();
        m_moves = 0 ;
        ChangeState(GAMESTATE_LEVELRUNNING) ;
      }
      keys = m_input.GetKeysHeld() ;
      if (keys & KEY_TOUCH)
      {
        // Swipe Left & right?
        int16_t moveX = (int16_t)m_input.GetLastTouchDownPosition().px - m_input.GetLastTouchPosition().px ;
        bool handled = false ;
        if (moveX > 32)
        {
          DisableAllSprites() ;
          if (m_levelIndexInDifficulty)
            m_levelIndexInDifficulty = (m_levelIndexInDifficulty - 1) % GetLevelCountForDifficulty(m_difficulty) ;
          else
            m_levelIndexInDifficulty = GetLevelCountForDifficulty(m_difficulty) - 1;
          LoadLevel(GetLevel(m_difficulty, m_levelIndexInDifficulty)) ;
          handled = true ;
        } else if (moveX < -32)
        {
          DisableAllSprites() ;
          m_levelIndexInDifficulty = (m_levelIndexInDifficulty + 1) % GetLevelCountForDifficulty(m_difficulty) ;
          LoadLevel(GetLevel(m_difficulty, m_levelIndexInDifficulty)) ;                  
          handled = true ;
        }
        if (handled)
        {
          // reset point of touch down, so we will not move it instantly again but use
          // the new position as the start of movement
          m_input.AccountDrag();
        }
      }
    }
    return true ;
  }
  return true ;
}


