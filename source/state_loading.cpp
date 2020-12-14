#include "gameclass.h"
#include <nds.h>

extern int gArgC ;
extern char **gArgV ;

void CGAME::Loading_EnterState()
{
  DisableAllSprites();
  StartFade(0, eFADEIN, TITLE_FADETIME) ;
  StartFade(1, eFADEIN, TITLE_FADETIME) ;
  m_titleTiles->Initialize() ;        
}

bool CGAME::Loading_Tick()
{
  // The Title started to fade in in the start. 
  // we will switch to main menu once either the title faded in and then out again, or 
  // any button was pressed
  if (m_keysDown)
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