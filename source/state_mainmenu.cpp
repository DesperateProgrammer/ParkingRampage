#include "gameclass.h"
#include <nds.h>
#include <stdio.h>
#include "state_mainmenu.h"
#include "mainmenu.h"
#include "menu_withcarselector.h"
#include "menuitem_coloredenumeration.h"
#include "menuitem_statechange.h"
#include "menuitem_slider.h"
#include "storage.h"

CMAINMENUSTATE::CMAINMENUSTATE(CGAME *game) 
{
  m_game = game ;
  m_mainMenuTiles = new CTILEMAP(1, (uint16_t *)mainmenuTiles, mainmenuTilesLen, (uint16_t *)mainmenuPal, mainmenuPalLen, (uint16_t *)mainmenuMap, mainmenuMapLen) ;
}

bool CMAINMENUSTATE::OnEnter()
{
  m_game->StartFade(1, eFADEIN, MAINMENU_FADETIME) ;
  m_mainMenuTiles->Initialize() ;          
  m_menu = new CMENUWITHCARSELECTOR(m_game) ;
  m_menuitems[0] = new CMENUITEMGAMESTATE(m_game, 0, "Options", GAMESTATE_CONFIG) ;
//  m_menuitems[0] = new CMENUITEMSLIDER(m_game, MAINMENUITEM_VOLUME, "\x0D\x0D", 0, 1024, m_game->GetAudio()->GetMusicVolume()) ;
  m_menuitems[1] = new CMENUITEMCOLOREDENUMERATION(m_game, MAINMENUITEM_DIFFICULTY, "Difficulty") ;
  ((CMENUITEMCOLOREDENUMERATION *)m_menuitems[1])->AddEntry((uint32_t)eBEGINNER, "Beginner", 3) ;
  ((CMENUITEMCOLOREDENUMERATION *)m_menuitems[1])->AddEntry((uint32_t)eINTERMEDIATE, "Intermediate", 5) ;
  ((CMENUITEMCOLOREDENUMERATION *)m_menuitems[1])->AddEntry((uint32_t)eADVANCED, "Advanced", 7) ;
  ((CMENUITEMCOLOREDENUMERATION *)m_menuitems[1])->AddEntry((uint32_t)eEXPERT, "Expert", 1) ;
  ((CMENUITEMCOLOREDENUMERATION *)m_menuitems[1])->AddEntry((uint32_t)eANY, "Any", 9) ;
  ((CMENUITEMCOLOREDENUMERATION *)m_menuitems[1])->SetValue((uint32_t)m_game->GetLevelManager()->GetDifficulty()) ;
  m_menuitems[2] = new CMENUITEMGAMESTATE(m_game, MAINMENUITEM_START, "Random Level", GAMESTATE_LEVELLOADING) ;
  m_menuitems[3] = new CMENUITEMGAMESTATE(m_game, MAINMENUITEM_SELECT, "Select Level", GAMESTATE_LEVELSELECT) ;
  for (int i=0;i<4;i++)
    m_menu->AddItem(m_menuitems[i]) ;
  m_menu->SelectItem(m_menuitems[2]) ;
  return true ;
}

bool CMAINMENUSTATE::OnLeave()
{
  m_game->GetSubText()->Clear() ;
  m_game->GetMainText()->Clear() ;
  m_game->GetSubSprites()->DisableAll() ;
  delete m_menu ;
  for (unsigned int i = 0; i< sizeof(m_menuitems) / sizeof(m_menuitems[0]);i++)
    delete m_menuitems[i] ;
    
  return true ;
}

bool CMAINMENUSTATE::OnTick()
{
  m_menu->OnTick() ;
  m_game->GetLevelManager()->SetDifficulty((EDIFFICULTY)((CMENUITEMCOLOREDENUMERATION *)m_menuitems[1])->GetValue()) ;
  return true ;
}
