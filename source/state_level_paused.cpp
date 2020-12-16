#include "gameclass.h"
#include <nds.h>

void CGAME::LevelPaused_EnterState()
{
  PauseLevelTime() ;
  m_subText->SetText((256 - 8*8) / 16, (192 - 16) / 32, (char *)"Paused !") ;
  m_subText->SetText(5, 7, (char *)"\x11 Main menu");
  m_subText->SetText(18, 7, (char *)"Resume \x12");
  m_subText->SetText(21, 10, (char *)"        ");
  m_subText->EnableTextWindow(32, 64, 192, 80) ;  
  m_mainText->SetText(4, 2, (char *)"   In level controls:") ;
  m_mainText->SetText(4, 3, (char *)"   \x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02") ;
  m_mainText->SetText(6, 4, (char *)"\x10\x13\x10  Move selection") ;
  m_mainText->SetText(6, 6, (char *)" \x12   Pause") ;
  m_mainText->SetText(6, 8, (char *)"\x14\x15\x16  Move car") ;
  m_mainText->EnableTextWindow(32, 32, 192, 128) ;  
}

bool CGAME::LevelPaused_Tick()
{
  uint16_t keys = m_keysDown ;
  if (IsGameWon())
  {
    // TODO: Show win
    ChangeState(GAMESTATE_LEVELWON) ;
    return true ;
  }
  for (unsigned int key=0;key<sizeof(keys)*8;key++)
  {
    if (keys & (1 << key))
    {
      switch (1 << key)
      {
        case KEY_TOUCH:
          if ((m_touchDown.py >= 6*16+8) && (m_touchDown.py < 8*16+8))
          {
            if ((m_touchDown.px >= 4*8) && (m_touchDown.px < 28*8))
            {
              if (m_touchDown.px < 128)
              {
                // Return
                ResetLevelTime() ;
                m_mainText->Clear();
                UnloadLevel() ;
                DisableAllSprites() ;
                ChangeState(GAMESTATE_MAINMENU) ;   
                return true ;                 
              } else
              {
                // Resume
                m_mainText->Clear();
                ChangeState(GAMESTATE_LEVELRUNNING) ;
              }
            }
          }
          break;
        case KEY_SELECT:
          // Return to main menu
          ResetLevelTime() ;
          m_mainText->Clear();
          UnloadLevel() ;
          DisableAllSprites() ;
          ChangeState(GAMESTATE_MAINMENU) ;                    
          break;
        case KEY_START:
          ChangeState(GAMESTATE_LEVELRUNNING) ;
          return true;
      }
    }
  }
  UpdateCarsOnScreen();
  UpdateLevelStats() ;
  return true ;
}