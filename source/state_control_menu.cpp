#include "state_control_menu.h"

void CCONTROLMENUSTATE::ShowMenuSelector() 
{
  int32_t scale = sinLerp(m_game->GetTimeManager()->GetTimerTicks()*70);
  m_game->GetSubSprites()->SetSprite(0, SPRITE_CAR_TARGET, 2*2*8 + scale * 8 / 4096, 2*2*8 + 4*8*m_menuItem, ORIENTATION_RIGHT) ;
  m_game->GetSubSprites()->SetSprite(1, SPRITE_CAR_TARGET, 12*2*8 - scale * 8 / 4096, 2*2*8 + 4*8*m_menuItem, ORIENTATION_LEFT) ;
  m_game->GetSubSprites()->Update() ;
}

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
