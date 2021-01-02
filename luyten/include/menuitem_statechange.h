#pragma once

#include "menuitem.h"
#include "gamebase.h"

typedef class CMENUITEMGAMESTATE : public CMENUITEM
{
  protected:
    uint32_t      m_targetState ;
  public:
    CMENUITEMGAMESTATE(class CGAMEBASE *game, uint32_t id, std::string caption, uint32_t targetState) 
      : CMENUITEM(game, id, caption), m_targetState(targetState) 
      {};
    
    virtual void OnTouch(touchPosition pos) 
    {
      m_game->ChangeStateOnNextTick(m_targetState) ;
    }
    
    virtual void OnModifyPlus() 
    {
      m_game->ChangeStateOnNextTick(m_targetState) ;
    }
    
    virtual void OnModifyMinus() 
    {
      m_game->ChangeStateOnNextTick(m_targetState) ;
    }
    
} CMENUITEMGAMESTATE ;