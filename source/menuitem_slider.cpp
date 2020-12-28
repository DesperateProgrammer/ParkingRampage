#include "menuitem_slider.h"
#include <stdio.h>

CMENUITEMSLIDER::CMENUITEMSLIDER(class CGAME *game, uint32_t id, std::string caption, uint32_t min, uint32_t max, uint32_t value) 
: CMENUITEM(game, id, caption), m_min(min), m_max(max), m_value(value)
{
}

void CMENUITEMSLIDER::OnTouch(touchPosition pos)
{
  uint32_t sliderMin = ((32 - (11 + m_caption.length())) / 2) * 8 + (m_caption.length() + 2) * 8 ;
  if ((pos.px >= sliderMin) && (pos.px < sliderMin + 8*8))
  {
    SetValue(m_min + ((m_max - m_min) * (pos.px - sliderMin)) / 64) ;
  }
}

void CMENUITEMSLIDER::OnModifyPlus()
{
  SetValue(GetValue() + (m_max-m_min) /8) ;
}

void CMENUITEMSLIDER::OnModifyMinus()
{
  if (GetValue() > m_min + (m_max-m_min) / 8)
    SetValue(GetValue() - (m_max-m_min) / 8) ;
  else
    SetValue(m_min) ;  
}

void CMENUITEMSLIDER::OnTick(uint8_t y)
{
  char buffer[32] ;
  snprintf(buffer, 32,"%s \x17\x18\x18\x18\x18\x18\x18\x18\x18\x0F", m_caption.c_str());
  uint32_t valuePerStep = (m_max - m_min) / 64 ;
  uint32_t valuePerChar = (m_max - m_min) / 8 ;
  for (unsigned int i=0;i<m_value / valuePerChar ; i++)
  {
    buffer[i+4] = 0x0E ;
  }

  if ((m_value % valuePerChar) / valuePerStep)
  {
    if (m_value / valuePerChar >= 1) 
      buffer[m_caption.length() + 2 + (m_value / valuePerChar)-1] = 0x0E ;
    buffer[m_caption.length() + 2 + (m_value / valuePerChar)] = 0x18 + ((m_value % valuePerChar) / valuePerStep) ;
  }

  m_game->GetSubText()->SetText((32 - strlen(buffer)) / 2, y, buffer) ;
}

uint32_t CMENUITEMSLIDER::GetValue()
{
  return m_value ;
}

void CMENUITEMSLIDER::SetValue(uint32_t value)
{
  if (value < m_min)
    m_value = m_min ;
  else if (value > m_max)
    m_value = m_max ;
  else
    m_value = value ;
}
