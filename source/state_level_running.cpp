#include "gameclass.h"
#include "state_level_running.h"
#include <nds.h>

#include <stdio.h>

CLEVELRUNNINGSTATE::CLEVELRUNNINGSTATE(CGAME *game) 
{
  m_game = game ;
}


bool CLEVELRUNNINGSTATE::OnEnter()
{
  m_game->GetLevelManager()->ContinueLevelTime() ;
  m_game->GetSubText()->Clear() ;  
  m_game->GetMainText()->Clear() ;  
  m_game->GetSubText()->SetText(21, 10, (char *)"Pause \x12");
  m_game->GetSubText()->EnableTextWindow(20*8, 9*16+8, 10*8, 2*16) ; 
  return true ;
}

bool CLEVELRUNNINGSTATE::OnTick()
{
  if (m_game->GetLevelManager()->IsLevelWon())
  {
    // TODO: Show win
    m_sm->ChangeState(GAMESTATE_LEVELWON) ;
    return true ;
  }
  uint16_t keys = m_game->GetInputManager()->GetKeysDown() ; ;
  if (keys & KEY_TOUCH)
  {
    m_game->GetLevelManager()->SelectCarFromTouch(m_game->GetInputManager()->GetLastTouchDownPosition()) ;
    if ((m_game->GetInputManager()->GetLastTouchDownPosition().py >= 9*16+8) && (m_game->GetInputManager()->GetLastTouchDownPosition().py < 11*16+8))
    {
      if ((m_game->GetInputManager()->GetLastTouchDownPosition().px >= 21*8) && (m_game->GetInputManager()->GetLastTouchDownPosition().px < 30*8))
      {
        m_sm-> ChangeState(GAMESTATE_LEVELPAUSED) ;
      }
    }            
  }
  if (m_game->GetInputManager()->IsKeyForAlias(keys, KEYALIAS_MOVE_LEFT))
  { 
    if (m_game->GetLevelManager()->CanMoveLeft())
        m_game->GetLevelManager()->MoveLeft() ;
  }
  if (m_game->GetInputManager()->IsKeyForAlias(keys, KEYALIAS_MOVE_RIGHT))
  { 
    if (m_game->GetLevelManager()->CanMoveRight())
        m_game->GetLevelManager()->MoveRight() ;
  }
  if (m_game->GetInputManager()->IsKeyForAlias(keys, KEYALIAS_MOVE_UP))
  { 
    if (m_game->GetLevelManager()->CanMoveUp())
        m_game->GetLevelManager()->MoveUp() ;
  }
  if (m_game->GetInputManager()->IsKeyForAlias(keys, KEYALIAS_MOVE_DOWN))
  { 
    if (m_game->GetLevelManager()->CanMoveDown())
        m_game->GetLevelManager()->MoveDown() ;
  }
  if (m_game->GetInputManager()->IsKeyForAlias(keys, KEYALIAS_SELECT_LEFT))
    m_game->GetLevelManager()->SelectNextCarLeft() ;
  if (m_game->GetInputManager()->IsKeyForAlias(keys, KEYALIAS_SELECT_RIGHT))
    m_game->GetLevelManager()->SelectNextCarRight() ;
  if (m_game->GetInputManager()->IsKeyForAlias(keys, KEYALIAS_SELECT_DOWN))
    m_game->GetLevelManager()->SelectNextCarDown() ;
  if (m_game->GetInputManager()->IsKeyForAlias(keys, KEYALIAS_SELECT_UP))
    m_game->GetLevelManager()->SelectNextCarUp() ;
  if (m_game->GetInputManager()->IsKeyForAlias(keys, KEYALIAS_PAUSE))
    m_sm->ChangeState(GAMESTATE_LEVELPAUSED) ;
    
  keys = m_game->GetInputManager()->GetKeysHeld() ;
  if (keys & KEY_TOUCH)
  {
    // this might be drag and drop
    int16_t moveX = (int16_t)m_game->GetInputManager()->GetLastTouchDownPosition().px - m_game->GetInputManager()->GetLastTouchPosition().px ;
    int16_t moveY = (int16_t)m_game->GetInputManager()->GetLastTouchDownPosition().py - m_game->GetInputManager()->GetLastTouchPosition().py ;
    bool handled = false ;
    if (moveX <= -16)
    {
      // try to move right
      if (m_game->GetLevelManager()->CanMoveRight())
        m_game->GetLevelManager()->MoveRight() ;
      handled = true ;
    } else if (moveX >= 16)
    {              
      // try to move left
      if (m_game->GetLevelManager()->CanMoveLeft())
        m_game->GetLevelManager()->MoveLeft() ;
      handled = true ;
    }
    if (moveY >= 16)
    {
      // try to move up
      if (m_game->GetLevelManager()->CanMoveUp())
        m_game->GetLevelManager()->MoveUp() ;
      handled = true ;
    } else if (moveY <= -16)
    {              
      if (m_game->GetLevelManager()->CanMoveDown())
        m_game->GetLevelManager()->MoveDown() ;
      handled = true ;
    }
    if (handled)
    {
      // reset point of touch down, so we will not move it instantly again but use
      // the new position as the start of movement
      m_game->GetInputManager()->AccountDrag() ;
      m_game->GetLevelManager()->SetPartialMove(0,0) ;
    } else
    {
      m_game->GetLevelManager()->SetPartialMove(-moveX, -moveY) ;
    }
  } else
  {
    m_game->GetLevelManager()->SetPartialMove(0, 0) ;
  }
  m_game->GetLevelManager()->UpdateCarsOnScreen();
  m_game->GetLevelManager()->UpdateLevelStats() ;
  return true ;
}
