#include "gameclass.h"
#include "state_level_paused.h"
#include <nds.h>

CLEVELPAUSEDSTATE::CLEVELPAUSEDSTATE(CGAME *game) 
{
  m_game = game ;
}

bool CLEVELPAUSEDSTATE::OnEnter() 
{
  m_game->GetLevelManager()->PauseLevelTime() ;
  m_game->GetSubText()->SetText((256 - 8*8) / 16, (192 - 16) / 32, (char *)"Paused !") ;
  m_game->GetSubText()->SetText(5, 7, (char *)"\x11 Main menu");
  m_game->GetSubText()->SetText(18, 7, (char *)"Resume \x12");
  m_game->GetSubText()->SetText(21, 10, (char *)"        ");
  m_game->GetSubText()->EnableTextWindow(32, 64, 192, 80) ;  
  m_game->GetMainText()->SetText(4, 2, (char *)"   In level controls:") ;
  m_game->GetMainText()->SetText(4, 3, (char *)"   \x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02") ;
  m_game->GetMainText()->SetText(6, 4, (char *)"\x10\x13\x10  Move selection") ;
  m_game->GetMainText()->SetText(6, 6, (char *)" \x12   Pause") ;
  m_game->GetMainText()->SetText(6, 8, (char *)"\x14\x15\x16  Move car") ;
  m_game->GetMainText()->EnableTextWindow(32, 32, 192, 128) ;  
  return true ;
}

bool CLEVELPAUSEDSTATE::OnLeave()
{
  m_game->GetMainText()->Clear() ;
  m_game->GetSubText()->Clear() ;  
  m_game->GetSubSprites()->DisableAll() ;
  return true ;
}

bool CLEVELPAUSEDSTATE::OnTick()  
{
  uint16_t keys = m_game->GetInputManager()->GetKeysDown() ;
  if (keys & KEY_TOUCH)
  {
    if ((m_game->GetInputManager()->GetLastTouchPosition().py >= 6*16+8) && (m_game->GetInputManager()->GetLastTouchPosition().py < 8*16+8))
    {
      if ((m_game->GetInputManager()->GetLastTouchPosition().px >= 4*8) && (m_game->GetInputManager()->GetLastTouchPosition().px < 28*8))
      {
        if (m_game->GetInputManager()->GetLastTouchPosition().px < 128)
        {
          // Return
          m_game->GetLevelManager()->ResetLevelTime() ;
          m_game->GetLevelManager()->UnloadLevel() ;
          m_sm->ChangeState(GAMESTATE_MAINMENU) ;   
          return true ;                 
        } else
        {
          // Resume
          m_sm->ChangeState(GAMESTATE_LEVELRUNNING) ;
        }
      }
    }
  }
  if (m_game->GetInputManager()->IsKeyForAlias(keys, KEYALIAS_RETURN))
  { 
    m_game->GetLevelManager()->ResetLevelTime() ;
    m_game->GetLevelManager()->UnloadLevel() ;
    m_game->ChangeState(GAMESTATE_MAINMENU) ;  
    return true;
  }
  if (m_game->GetInputManager()->IsKeyForAlias(keys, KEYALIAS_PAUSE))
  {
    m_sm->ChangeState(GAMESTATE_LEVELRUNNING) ;
    return true;
  }
  m_game->GetLevelManager()->UpdateCarsOnScreen();
  m_game->GetLevelManager()->UpdateLevelStats() ;
  return true ;
}