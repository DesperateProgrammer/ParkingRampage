#include "menuitem.h"
#include "gameclass.h"

CMENUITEM::CMENUITEM(class CGAME *game, uint32_t id, std::string caption) 
{
  m_game = game ;
  m_id = id ;
  m_caption = caption ;
}

void CMENUITEM::OnTick(uint8_t y) 
{
  std::string tmp = "" ;
  tmp.append(16 - (m_caption.length() + 1) / 2, (char)' ') ;
  tmp += m_caption ;
  if (tmp.length() < 32)
    tmp.append(32 - tmp.length(), (char)' ') ;
  m_game->GetSubText()->SetText(0, y, (char *)tmp.c_str()) ;
}    

void CMENUITEM::OnSelect() 
{
}

void CMENUITEM::OnDeselect() 
{
}

void CMENUITEM::OnTouch(touchPosition pos)
{
}

void CMENUITEM::OnModifyPlus()
{
}

void CMENUITEM::OnModifyMinus()
{
}