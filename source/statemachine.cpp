    #include "statemachine.h"
    
    CSTATEMACHINE::CSTATEMACHINE() 
    {
      m_state = 0xffff ;
    }
    
    bool CSTATEMACHINE::Tick()
    {
      return OnStateTick() ;
    }
    
    bool CSTATEMACHINE::ChangeState(uint16_t newState) 
    {
      if (m_state != 0xffff) 
        OnStateLeave() ;
      m_state = newState ;
      return OnStateEnter() ;
    }
    
    bool CSTATEMACHINE::OnStateTick() 
    {
      return true ;
    }
    
    bool CSTATEMACHINE::OnStateEnter()
    {
      return true ;
    }
    
    bool CSTATEMACHINE::OnStateLeave()
    {
      return true ;
    }
    
    