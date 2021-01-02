#pragma once

#include "menuitem.h"
#include "gamebase.h"
#include <vector>

typedef class CMENUITEMSLIDER : public CMENUITEM
{
  protected:
    uint32_t      m_min, m_max, m_value;
  public:
    CMENUITEMSLIDER(class CGAMEBASE *game, uint32_t id, std::string caption, uint32_t min, uint32_t max, uint32_t value) ;
    
    virtual void OnTouch(touchPosition pos) ;    
    virtual void OnModifyPlus() ;    
    virtual void OnModifyMinus() ;
    
    virtual void OnTick(uint8_t y) ;       
    
    uint32_t GetValue() ;
    void SetValue(uint32_t value) ;
    
} CMENUITEMSLIDER ;