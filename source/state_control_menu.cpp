#include "state_control_menu.h"
#include "gameclass.h"
#include <nds.h>
#include <stdio.h>
#include "mainmenu.h"
#include "menu_withcarselector.h"
#include "menuitem_coloredenumeration.h"
#include "menuitem_statechange.h"
#include "menuitem_slider.h"
#include "storage.h"

CCONTROLMENUSTATE::CCONTROLMENUSTATE(CGAME *game) 
{
  m_game = game ;
}
  
bool CCONTROLMENUSTATE::OnEnter() 
{
  m_menu = new CMENUWITHCARSELECTOR(m_game) ;
  m_menuitems[0] = new CMENUITEMSLIDER(m_game, 0, "\x0D\x0D", 0, 1024, m_game->GetAudio()->GetMusicVolume()) ;
  m_menuitems[1] = new CMENUITEMSLIDER(m_game, 1, "\x0C", 0, 1024, m_game->GetAudio()->GetEffectVolume()) ;
  m_menuitems[2] = new CMENUITEMCOLOREDENUMERATION(m_game, 2, "Control scheme") ;
  ((CMENUITEMCOLOREDENUMERATION *)m_menuitems[2])->AddEntry(KEYMAPPING_DPADTOSELECT, "D-PAD to select", 3) ;
  ((CMENUITEMCOLOREDENUMERATION *)m_menuitems[2])->AddEntry(KEYMAPPING_DPADTOMOVE, "D-PAD to move", 5) ;
  
  ((CMENUITEMCOLOREDENUMERATION *)m_menuitems[2])->SetValue(m_game->GetInputManager()->GetActivePreconfiguredMapping()) ;

  m_menuitems[3] = new CMENUITEMGAMESTATE(m_game, 3, "Return", GAMESTATE_MAINMENU) ;
  for (int i=0;i<4;i++)
    m_menu->AddItem(m_menuitems[i]) ;
  return true ;
}

bool CCONTROLMENUSTATE::OnLeave() 
{
  m_game->GetSubText()->Clear() ;
  m_game->GetMainText()->Clear() ;
  m_game->GetSubSprites()->DisableAll() ;
  delete m_menu ;
  for (unsigned int i = 0; i< sizeof(m_menuitems) / sizeof(m_menuitems[0]);i++)
    delete m_menuitems[i] ;
    
  CSTORAGE::GetInstance()->GetConfig(CFGTAG_VOLUME_MUSIC)->SetUInt32( m_game->GetAudio()->GetMusicVolume()) ;
  CSTORAGE::GetInstance()->GetConfig(CFGTAG_VOLUME_SFX)->SetUInt32( m_game->GetAudio()->GetEffectVolume()) ;      
  CSTORAGE::GetInstance()->GetConfig(CFGTAG_VOLUME_KEYMAPPING)->SetUInt32(((CMENUITEMCOLOREDENUMERATION *)m_menuitems[2])->GetValue()) ;
  CSTORAGE::GetInstance()->Flush() ;
  
  return true ;
}

bool CCONTROLMENUSTATE::OnTick()
{
  m_menu->OnTick() ;
  m_game->GetAudio()->SetMusicVolume(((CMENUITEMSLIDER *)m_menuitems[0])->GetValue()) ;
  m_game->GetAudio()->SetEffectVolume(((CMENUITEMSLIDER *)m_menuitems[1])->GetValue()) ;
  
  if ((int8_t)((CMENUITEMCOLOREDENUMERATION *)m_menuitems[2])->GetValue() != m_game->GetInputManager()->GetActivePreconfiguredMapping())
  {
    m_game->GetInputManager()->ActivatePreconfiguredMapping((int8_t)((CMENUITEMCOLOREDENUMERATION *)m_menuitems[2])->GetValue()) ;
  }
  return true ;
}
