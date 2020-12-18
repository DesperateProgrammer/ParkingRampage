#pragma once

#include <stdint.h>

typedef class CSTATEMACHINE
{
  protected:
    uint16_t  m_state ;
  public:
    CSTATEMACHINE() ;
    
    bool Tick() ;
    bool ChangeState(uint16_t newState) ;
    
    virtual bool OnStateTick() ;
    virtual bool OnStateEnter() ;
    virtual bool OnStateLeave() ;
    
} CSTATEMACHINE ;