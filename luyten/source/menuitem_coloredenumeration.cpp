#include "menuitem_coloredenumeration.h"

CMENUITEMCOLOREDENUMERATION::CMENUITEMCOLOREDENUMERATION(class CGAMEBASE *game, uint32_t id, std::string caption) 
: CMENUITEM(game,id,caption)
{
  m_selection = 0 ;
}

void CMENUITEMCOLOREDENUMERATION::OnTouch(touchPosition pos)
{
  OnModifyPlus() ;
}

void CMENUITEMCOLOREDENUMERATION::OnModifyPlus()
{
  m_selection++ ;
  if (m_selection == m_entries.size())
    m_selection = 0 ;
}

void CMENUITEMCOLOREDENUMERATION::OnModifyMinus()
{
  if (m_selection)
    m_selection-- ;
  else
    m_selection = m_entries.size()-1 ;
}

uint32_t CMENUITEMCOLOREDENUMERATION::GetValue() 
{
  return m_entries[m_selection].value ;
}

void CMENUITEMCOLOREDENUMERATION::SetValue(uint32_t value)
{
  for (unsigned int i=0;i<m_entries.size();i++)
  {
    if (m_entries[i].value == value)
    {
      m_selection = i ;
      return ;
    }
  }
}

void CMENUITEMCOLOREDENUMERATION::AddEntry(uint32_t value, std::string name, uint16_t color) 
{
  SMENUITEMCOLOREDENUMERATIONENTRY entry ;
  entry.value = value ;
  entry.name = name ;
  entry.color = color ;
  m_entries.push_back(entry) ;
}

void CMENUITEMCOLOREDENUMERATION::RemoveEntry(uint32_t value)
{
  // TODO
}

void CMENUITEMCOLOREDENUMERATION::OnTick(uint8_t y)
{
  std::string tmp = "" ;
  tmp.append(16 - (m_entries[m_selection].name.length() + 1)/ 2, (char)' ') ;
  tmp += m_entries[m_selection].name ;
  if (tmp.length() < 32)
    tmp.append(32 - tmp.length(), (char)' ') ;
  m_game->GetSubText()->SetText(0, y, (char *)tmp.c_str(), m_entries[m_selection].color) ;
}
