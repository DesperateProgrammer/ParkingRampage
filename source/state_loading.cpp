#include "gameclass.h"
#include <nds.h>

void CGAME::Loading_EnterState()
{
  StartFade(0, eFADEIN, TITLE_FADETIME) ;
  StartFade(1, eFADEIN, TITLE_FADETIME) ;
  m_titleTiles->Initialize() ;        
}

void CGAME::Loading_LeaveState()
{
}

bool CGAME::Loading_Tick()
{
  // The Title started to fade in in the start. 
  // we will switch to main menu once either the title faded in and then out again, or 
  // any button was pressed
  if (m_input.GetKeysDown())
  {
    // Speed up fading out so we exit immediate
    StartFade(SCREEN_BOTTOM, eFADEOUT, 0) ;
  }
  if (!IsFading(SCREEN_BOTTOM))
  {
    if (GetFadeMode(SCREEN_BOTTOM) == eFADEIN)
    {
      StartFade(SCREEN_BOTTOM, eFADEOUT, TITLE_FADETIME) ;
    } else
    {
      ChangeState(GAMESTATE_MAINMENU) ;
    }
  }
  return true ;
}