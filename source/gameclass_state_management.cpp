#include "gameclass.h"
#include <nds.h>

/** 
    Tick() is called continiously and shall return
    as fast as possible. 
    
    For the game engine, we will wait for a frame
    and scan the keys. So subsequent gamestate ticks will
    be called wonce for every frame
    
**/
bool CGAME::Tick()
{
  swiWaitForVBlank();
  m_input.Tick() ;
  
  for (int screen=0;screen<2;screen++)
  {  
    UpdateFading(screen);
    UpdateRotScale(screen) ;
  }

  switch (m_state)
  {
    case GAMESTATE_LEVELRUNNING:
      return LevelRunning_Tick() ;
    case GAMESTATE_LEVELLOADING:
      return LevelLoading_Tick() ;
    case GAMESTATE_LEVELPAUSED:
      return LevelPaused_Tick() ;
    case GAMESTATE_LEVELWON:
      return LevelWon_Tick() ;
    case GAMESTATE_LEVELSELECT:
      return LevelSelect_Tick() ;
    case GAMESTATE_LOADING:
      return Loading_Tick() ;
    case GAMESTATE_MAINMENU:
      return MainMenu_Tick() ;
    default:
      break;
  }
  
  return true;
}

/**

  ChangeState will is called to change the current state of the game
  and is e

**/

bool CGAME::ChangeState(uint8_t newState) 
{
  LeaveState();
  m_state = newState ;
  switch (m_state)
  {
    case GAMESTATE_LOADING:
      Loading_EnterState() ;
      break;
    case GAMESTATE_MAINMENU:
      MainMenu_EnterState() ;
      break;
    case GAMESTATE_LEVELLOADING:
      LevelLoading_EnterState() ;
      break;
    case GAMESTATE_LEVELRUNNING:
      LevelRunning_EnterState() ;
      break;
    case GAMESTATE_LEVELPAUSED:
      LevelPaused_EnterState() ;
      break ;
    case GAMESTATE_LEVELSELECT:
      LevelSelect_EnterState() ;
      break ;
    case GAMESTATE_LEVELWON:
      LevelWon_EnterState() ;
      break ;
    default:
      break;
  }
  return true ;
}

bool CGAME::LeaveState() 
{
  switch (m_state)
  {
    case GAMESTATE_LOADING:
      Loading_LeaveState() ;
      break;
    case GAMESTATE_MAINMENU:
      MainMenu_LeaveState() ;
      break;
    case GAMESTATE_LEVELLOADING:
      LevelLoading_LeaveState() ;
      break;
    case GAMESTATE_LEVELRUNNING:
      LevelRunning_LeaveState() ;
      break;
    case GAMESTATE_LEVELPAUSED:
      LevelPaused_LeaveState() ;
      break ;
    case GAMESTATE_LEVELSELECT:
      LevelSelect_LeaveState() ;
      break ;
    case GAMESTATE_LEVELWON:
      LevelWon_LeaveState() ;
      break ;
    default:
      break;
  }
  return true;
}



