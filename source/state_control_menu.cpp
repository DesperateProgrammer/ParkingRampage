#include "state_control_menu.h"
#include "gameclass.h"
#include <nds.h>
#include <stdio.h>
#include "mainmenu.h"
#include "menu_withcarselector.h"
#include "menuitem_coloredenumeration.h"
#include "menuitem_statechange.h"
#include "menuitem_slider.h"

CCONTROLMENUSTATE::CCONTROLMENUSTATE(CGAME *game) 
{
  m_game = game ;
}
  
bool CCONTROLMENUSTATE::OnEnter() 
{
  return true ;
}

bool CCONTROLMENUSTATE::OnLeave() 
{
  return true ;
}

bool CCONTROLMENUSTATE::OnTick()
{
  return true ;
}
