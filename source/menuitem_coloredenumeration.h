#pragma once

#include "menuitem.h"
#include "gameclass.h"
#include <vector>

typedef struct SMENUITEMCOLOREDENUMERATIONENTRY
{
  std::string name ;
  uint32_t    value ;
  uint16_t    color ;
} SMENUITEMCOLOREDENUMERATIONENTRY ;

typedef class CMENUITEMCOLOREDENUMERATION : public CMENUITEM
{
  protected:
    vector<SMENUITEMCOLOREDENUMERATIONENTRY> m_entries ;
    uint32_t      m_selection ;
  public:
    CMENUITEMCOLOREDENUMERATION(class CGAME *game, uint32_t id, std::string caption) ;
    
    virtual void OnTouch(touchPosition pos) ;    
    virtual void OnModifyPlus() ;    
    virtual void OnModifyMinus() ;
    
    virtual void OnTick(uint8_t y) ;       
    
    uint32_t GetValue() ;
    void SetValue(uint32_t value) ;
    void AddEntry(uint32_t value, std::string name, uint16_t color= 0xffff) ;
    void RemoveEntry(uint32_t value) ;
    
} CMENUITEMCOLOREDENUMERATION ;