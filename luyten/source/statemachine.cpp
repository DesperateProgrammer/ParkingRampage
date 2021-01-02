#include "statemachine.h"
  
void CSTATE::OnRegister(uint16_t state, class CSTATEMACHINE *sm) 
{
  m_sm = sm ;
  m_stateNum = state ;
}
  
bool CSTATE::Activate() 
{
  // change into own state 
  return m_sm->ChangeState(m_stateNum);
}
  
CSTATEMACHINE::CSTATEMACHINE() 
{
  m_sheduledChange = m_state = 0xffff ;
  
}

bool CSTATEMACHINE::Tick()
{
  if (m_sheduledChange != m_state)
  {
    ChangeState(m_sheduledChange) ;
  }
  OnEveryTick() ;
  if (m_stateClasses.find(m_state) != m_stateClasses.end())
  {
    return m_stateClasses[m_state]->OnTick() ;
  } else
  {
    return OnStateTick() ;
  } 
}

void CSTATEMACHINE::ChangeStateOnNextTick(uint16_t newState) 
{
  m_sheduledChange = newState ;
}

bool CSTATEMACHINE::ChangeState(uint16_t newState) 
{
  if (m_state != 0xffff) 
  {
    if (m_stateClasses.find(m_state) != m_stateClasses.end())
    {
      m_stateClasses[m_state]->OnLeave() ;
    } else
    {
      OnStateLeave() ;
    }
  }
  m_sheduledChange = m_state = newState ;
  if (m_stateClasses.find(m_state) != m_stateClasses.end())
  {
    return m_stateClasses[m_state]->OnEnter() ;
  } else
  {
    return OnStateEnter() ;
  }
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

void CSTATEMACHINE::RegisterState(CSTATE *state, uint16_t stateNum) 
{
  m_stateClasses[stateNum] = state ;
  state->OnRegister(stateNum, this);
}
