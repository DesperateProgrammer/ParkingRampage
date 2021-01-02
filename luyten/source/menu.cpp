#include "menu.h"

CMENU::CMENU(class CGAMEBASE *game)
{
  m_game = game ;
  m_firstVisible = m_selection = 0 ;
}

void CMENU::SelectNext()
{
  CMENUITEM *item = GetSelectedItem() ;
  if (item)
    item->OnDeselect() ;
  m_selection++ ;
  if (m_selection >= m_items.size())
    m_selection = 0;
  OnItemSelected(m_items[m_selection]) ;
}

void CMENU::SelectPrevious() 
{
  CMENUITEM *item = GetSelectedItem() ;
  if (item)
    item->OnDeselect() ;
  if (m_selection == 0)
    m_selection = m_items.size() - 1 ;
  else
    m_selection-- ;
  OnItemSelected(m_items[m_selection]) ;
}

void CMENU::SelectItem(CMENUITEM *item) 
{
  for (unsigned int i=0;i<m_items.size();i++)
  {
    if (m_items[i] == item)
    {
      CMENUITEM *itm = GetSelectedItem() ;
      if (itm)
        itm->OnDeselect() ;
      m_selection = i ;
      OnItemSelected(item) ;
      return ;
    }
  }
}

void CMENU::AddItem(CMENUITEM *item)
{
  m_items.push_back(item) ;
}

void CMENU::RemoveItem(CMENUITEM *item)
{
  // TODO
}

CMENUITEM *CMENU::GetSelectedItem()
{
  if (m_selection >= m_items.size())
    return 0 ;
  return m_items[m_selection] ;
}

void CMENU::OnTick() 
{
  // Drawing
  if (m_selection < m_firstVisible)
    m_firstVisible = m_selection ;
  else if (m_selection >= m_firstVisible + 4)
    m_firstVisible = m_selection - 3 ;
  for (uint32_t entry = m_firstVisible ; entry < m_firstVisible + 4; entry++)
  {
    // line 2 + 2*(entry - m_firstVisible)
    m_items[entry]->OnTick(2 + 2 * (entry - m_firstVisible)) ;
  }
  OnDrawItemSelection(2 + 2 * (m_selection - m_firstVisible)) ;
  // Event handling
  uint16_t keys = m_game->GetInputManager()->GetKeysDown() ; ;
  if (keys & KEY_TOUCH)
  {
    CMENUITEM *touchedItem = GetItemFromCoordinates(m_game->GetInputManager()->GetLastTouchDownPosition().px, m_game->GetInputManager()->GetLastTouchDownPosition().py) ;
    if (touchedItem == GetSelectedItem())
    {
      // Do touch event on selected item
      touchedItem->OnTouch(m_game->GetInputManager()->GetLastTouchDownPosition()) ;
    } else
    {
      // Item is to be selected by touch
      SelectItem(touchedItem) ;
    }
  }  
  if (m_game->GetInputManager()->IsKeyForAlias(keys, KEYALIAS_MENU_MODIFY_MINUS))
  {  
    CMENUITEM *item = GetSelectedItem() ;
    if (item)
    {
      item->OnModifyMinus() ;
      OnItemModified(item) ;
    }
  }
  if (m_game->GetInputManager()->IsKeyForAlias(keys, KEYALIAS_MENU_MODIFY_PLUS))
  {  
    CMENUITEM *item = GetSelectedItem() ;
    if (item)
    {
      item->OnModifyPlus() ;
      OnItemModified(item) ;
    }
  }  
  if (m_game->GetInputManager()->IsKeyForAlias(keys, KEYALIAS_MENU_UP))
  {  
    SelectPrevious() ;
  }  
  if (m_game->GetInputManager()->IsKeyForAlias(keys, KEYALIAS_MENU_DOWN))
  {  
    SelectNext() ;
  }  
}

void CMENU::OnItemSelected(CMENUITEM *item)
{
  if (item)
    item->OnSelect() ;
}

void CMENU::OnItemModified(CMENUITEM *item)
{
}

void CMENU::OnDrawItemSelection(uint8_t y)
{
}

CMENUITEM *CMENU::GetItemFromCoordinates(uint8_t x, uint8_t y)
{
  uint8_t lineTimes2 = y / 8 ;
  int maxVisibleEntries = 4 ;
  if (m_items.size() < 4)
    maxVisibleEntries = m_items.size() ;
  if ((lineTimes2 > 3) && (lineTimes2 < 3 + maxVisibleEntries*4))
  {
    uint8_t itemNum = (lineTimes2-3) / 4  + m_firstVisible ;
    return m_items[itemNum] ;
  }
  return 0 ;
}
