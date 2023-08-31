#include "state_loading.h"
#include "title.h"
#include "title_new.h"
#include "extendedbg.h"
#include <stdio.h>

CGAMELOADINGSTATE::CGAMELOADINGSTATE(CGAME *game) 
{
  m_game = game ;
  m_titleTiles = new CTILEMAP(1, (uint16_t *)titleTiles, titleTilesLen, (uint16_t *)titlePal, titlePalLen, (uint16_t *)titleMap, titleMapLen) ;
  //m_mainScreenTiles = new CTILEMAP(0, (uint16_t *)titleTiles, titleTilesLen, (uint16_t *)titlePal, titlePalLen, (uint16_t *)titleMap, titleMapLen) ;  
  m_mainScreenBG = new CEXTENDEDBG((uint16_t *)title_newBitmap);
  
}

extern int gArgC ;
extern char **gArgV ;

bool CGAMELOADINGSTATE::OnEnter()
{
  //m_mainScreenTiles->Initialize() ;
  m_titleTiles->Initialize() ; 
  m_mainScreenBG->Initialize() ;
  m_game->StartFade(0, eFADEIN, TITLE_FADETIME) ;
  m_game->StartFade(1, eFADEIN, TITLE_FADETIME) ;
 
#if 0
  char buffer[32] ;
  snprintf(buffer, 32, "%li", *(volatile uint32_t *)0x4004000) ;
  m_game->GetMainText()->SetText(0,0,buffer) ;
  for (int i=0;i<gArgC;i++)
  {
    snprintf(buffer, 32, "%s", gArgV[i]) ;
    m_game->GetMainText()->SetText(0,i+1,buffer) ;
  }
#endif
  return true ;       
}

bool CGAMELOADINGSTATE::OnTick()
{
  // The Title started to fade in in the start. 
  // we will switch to main menu once either the title faded in and then out again, or 
  // any button was pressed
  if (m_game->GetInputManager()->GetKeysDown())
  {
    // Speed up fading out so we exit immediate
    m_game->StartFade(SCREEN_BOTTOM, eFADEOUT, 0) ;
  }
  if (!m_game->IsFading(SCREEN_BOTTOM))
  {
    if (m_game->GetFadeMode(SCREEN_BOTTOM) == eFADEIN)
    {
      m_game->StartFade(SCREEN_BOTTOM, eFADEOUT, TITLE_FADETIME) ;
    } else
    {
      m_sm->ChangeState(GAMESTATE_MAINMENU) ;
    }
  }
  return true ;
}