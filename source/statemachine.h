#pragma once

#include <stdint.h>
#include <map>

typedef class CSTATE
{
  protected:
    class CSTATEMACHINE * m_sm = 0;
    uint16_t              m_stateNum ;
  public:
    virtual bool OnEnter() { return true ;} ;
    virtual bool OnLeave() { return true ;} ;
    virtual bool OnTick() { return true ;} ;
    
    void OnRegister(uint16_t state, class CSTATEMACHINE *sm) ;
    
    bool Activate() ;
} CSTATE ;

typedef class CSTATEMACHINE
{
  protected:
    uint16_t  m_state ;
    std::map<uint16_t, CSTATE *> m_stateClasses;
    
  public:
    CSTATEMACHINE() ;
    
    bool Tick() ;
    bool ChangeState(uint16_t newState) ;
    
    virtual bool OnStateTick() ;
    virtual bool OnStateEnter() ;
    virtual bool OnStateLeave() ;
    virtual void OnEveryTick() {} ;
    
    void RegisterState(CSTATE *state, uint16_t stateNum) ;
    
} CSTATEMACHINE ;