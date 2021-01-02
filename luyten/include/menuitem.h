#pragma once

#include <stdint.h>
#include <string>
#include <nds.h>

typedef class CMENUITEM
{
  protected:
    uint32_t      m_id ;
    std::string   m_caption ;
    class CGAMEBASE * m_game ;
  public:
    CMENUITEM(class CGAMEBASE *game, uint32_t id, std::string caption) ;
    
    virtual ~CMENUITEM() {} ;
    
    virtual void OnTick(uint8_t y) ;    
    virtual void OnSelect() ;
    virtual void OnDeselect() ;
    virtual void OnTouch(touchPosition pos) ;
    virtual void OnModifyPlus() ;
    virtual void OnModifyMinus() ;
    
    
} CMENUITEM ;