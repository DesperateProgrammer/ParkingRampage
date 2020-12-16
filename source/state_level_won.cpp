#include "gameclass.h"
#include <nds.h>
#include <stdio.h>

void CGAME::LevelWon_EnterState()
{
  PauseLevelTime() ;
  m_selectedCar = 0 ;
  m_subText->Clear() ;
  m_subText->SetText(12, 5, (char *)"You won!", TEXTCOLOR_RAINBOW) ;
  char buffer[32] ;
  uint32_t lTime = GetCurrentLevelTime() ;
  snprintf(buffer, 32, "%i moves in %li.%li s", m_moves, lTime / 1000, (lTime / 100) % 10) ;
  m_subText->SetText((256 - strlen(buffer)*8) / 16, 7, buffer) ;
  m_subText->EnableTextWindow(32, 64, 192, 80) ;
}

bool CGAME::LevelWon_Tick()
{
  if (m_input.GetKeysDown())
  {
    StartFade(SCREEN_BOTTOM, eFADEOUT, 1000) ;
    return true ;
  }
  if (!IsFading(SCREEN_BOTTOM))
  {
    if (GetFadeMode(SCREEN_BOTTOM) == eFADEIN)
    {
    } else
    {
      ChangeState(GAMESTATE_MAINMENU) ;
    }
  }  return true ;
}


