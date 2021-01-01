#include "inputmanager.h"
#include <nds.h>

CINPUTMANAGER::CINPUTMANAGER()
{
  m_keysDown = m_keysHeld = m_keysReleased = 0 ;
  ActivatePreconfiguredMapping(0) ;
}

int8_t CINPUTMANAGER::GetActivePreconfiguredMapping() 
{
  int slot = 0 ;
  while (gPredefinedKeyMappings[slot])
  {
    bool equal = true;
    for (int i=0;gPredefinedKeyMappings[slot][i];i+=2)
    {
      if (GetKeyMapping(gPredefinedKeyMappings[slot][i]) != gPredefinedKeyMappings[slot][i+1])
      {
        equal = false ;
        break;
      }
    }
    if (equal)
      return slot ;
    slot++ ;
  }
  return -1 ;
}

void CINPUTMANAGER::ActivatePreconfiguredMapping(int8_t slot) 
{
  if (slot < 0)
    return ;
  for (int i=0;gPredefinedKeyMappings[slot][i];i+=2)
  {
    SetKeyMapping(gPredefinedKeyMappings[slot][i], gPredefinedKeyMappings[slot][i+1]) ;
  }
}

void CINPUTMANAGER::Tick() 
{
  scanKeys();
  m_keysDown = keysDown() ;
  m_keysReleased = keysUp() ;
  m_keysHeld = keysHeld() ;
  touchRead(&m_touch);
  
  if (m_keysDown & KEY_TOUCH)
  {
    // if touch just occure, do not mark it as held
    m_keysHeld &= ~KEY_TOUCH ;
    memcpy(&m_touchDown, &m_touch, sizeof(m_touch)) ;
  }

  for (unsigned int i=0;i<sizeof(m_keysDown)*8;i++)
  {
    if (m_keysDown & (1 << i))
      OnKeyDown(1 << i) ;
    if (m_keysReleased & (1 << i))
      OnKeyReleased(1 << i) ;
  }
}

void CINPUTMANAGER::AccountDrag() 
{
    memcpy(&m_touchDown, &m_touch, sizeof(m_touch)) ;
}
    
uint16_t CINPUTMANAGER::GetKeysDown() 
{
  return m_keysDown ;
}

uint16_t CINPUTMANAGER::GetKeysReleased() 
{
  return m_keysReleased ;
}

uint16_t CINPUTMANAGER::GetKeysHeld() 
{
  return m_keysHeld ;
}

touchPosition CINPUTMANAGER::GetLastTouchPosition() 
{
  return m_touch ;
}

touchPosition CINPUTMANAGER::GetLastTouchDownPosition() 
{
  return m_touchDown ;
}

void CINPUTMANAGER::OnKeyDown(uint16_t key) 
{
  for (unsigned int i=0;i<m_registeredDownHandlers.size();i++)
  {
    SINPUTHANDLER ih = m_registeredDownHandlers[i] ;
    if (key & ih.filter)
    {
      ih.handler(key, ih.context, INPUTEVENT_DOWN) ;
    }
  }
}
    
void CINPUTMANAGER::OnKeyReleased(uint16_t key) 
{
  for (unsigned int i=0;i<m_registeredUpHandlers.size();i++)
  {
    SINPUTHANDLER ih = m_registeredDownHandlers[i] ;
    if (key & ih.filter)
    {
      ih.handler(key, ih.context, INPUTEVENT_RELEASED) ;
    }
  }
}
    
void CINPUTMANAGER::EnumerateKeysDown(keyhandler handler, void *context) 
{
  for (unsigned int i=0;i<sizeof(m_keysDown)*8;i++)
  {
    if (m_keysDown & (1 << i))
      handler(1<<i, context, INPUTEVENT_DOWN) ;
  }
}

void CINPUTMANAGER::EnumerateKeysRelease(keyhandler handler, void *context)
{
  for (unsigned int i=0;i<sizeof(m_keysReleased)*8;i++)
  {
    if (m_keysReleased & (1 << i))
      handler(1<<i, context, INPUTEVENT_RELEASED) ;
  }
}

void CINPUTMANAGER::EnumerateKeysHeld(keyhandler handler, void *context)
{
  for (unsigned int i=0;i<sizeof(m_keysHeld)*8;i++)
  {
    if (m_keysHeld & (1 << i))
      handler(1<<i, context, INPUTEVENT_HELD) ;
  }
}

void CINPUTMANAGER::AddHandler(uint8_t eventType, uint16_t filter, keyhandler handler, void *context) 
{
  // make sure we have no doublettes, so remove any first before adding
  RemoveHandler(eventType, filter, handler, context) ;
  SINPUTHANDLER ih ;
  ih.filter = filter ;
  ih.handler = handler ;
  ih.context = context ;
  if (eventType & INPUTEVENT_DOWN)
  {  
    m_registeredDownHandlers.push_back(ih) ;
  }
  if (eventType & INPUTEVENT_RELEASED)
  {  
    m_registeredUpHandlers.push_back(ih) ;
  }
}

void CINPUTMANAGER::RemoveHandler(uint8_t eventType, uint16_t filter, keyhandler handler, void *context) 
{
  if (eventType & INPUTEVENT_DOWN)
  {
    for (unsigned int i=0;i<m_registeredDownHandlers.size();i++)
    {
      SINPUTHANDLER ih = m_registeredDownHandlers[i] ;
      if ((ih.context == context) && (ih.handler == handler))
      {
        if (ih.filter & ~filter)
        {
          // no entry left in filter, so completely remove
          m_registeredDownHandlers.erase(m_registeredDownHandlers.begin()+i) ;
          i-- ;
          continue ;
        } else
        {
          m_registeredDownHandlers[i].filter &= ~filter ;
        }
      }
    }  
  }
  if (eventType & INPUTEVENT_RELEASED)
  {
    for (unsigned int i=0;i<m_registeredUpHandlers.size();i++)
    {
      SINPUTHANDLER ih = m_registeredUpHandlers[i] ;
      if ((ih.context == context) && (ih.handler == handler))
      {
        if (ih.filter & ~filter)
        {
          // no entry left in filter, so completely remove
          m_registeredUpHandlers.erase(m_registeredUpHandlers.begin()+i) ;
          i-- ;
          continue ;
        } else
        {
          m_registeredUpHandlers[i].filter &= ~filter ;
        }
      }
    }  
  }
}
    
void CINPUTMANAGER::SetKeyMapping(uint16_t alias, uint16_t keys)
{
  m_mapping[alias] = keys ;
}

uint16_t CINPUTMANAGER::GetKeyMapping(uint16_t alias) 
{
 if (m_mapping.find(alias) == m_mapping.end())
    return 0 ;
 return m_mapping[alias];
}

bool CINPUTMANAGER::IsKeyForAlias(uint16_t key, uint16_t alias) 
{
  return key & GetKeyMapping(alias) ;
}
