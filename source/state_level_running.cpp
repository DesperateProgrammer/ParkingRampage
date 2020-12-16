#include "gameclass.h"
#include <nds.h>

#include <stdio.h>

void CGAME::LevelRunning_EnterState()
{
  ContinueLevelTime() ;
  m_subText->Clear() ;  
  m_mainText->Clear() ;  
  m_subText->SetText(21, 10, (char *)"Pause \x12");
  m_subText->EnableTextWindow(20*8, 9*16+8, 10*8, 2*16) ; 
  
}

bool CGAME::LevelRunning_Tick()
{
  if (IsGameWon())
  {
    // TODO: Show win
    ChangeState(GAMESTATE_LEVELWON) ;
    return true ;
  }
  uint16_t keys = m_input.GetKeysDown() ; ;
  for (unsigned int key=0;key<sizeof(keys)*8;key++)
  {
    if (keys & (1 << key))
    {
      switch (1 << key)
      {
        case KEY_TOUCH:
          {
            uint8_t row = m_input.GetLastTouchDownPosition().py / 8 ;
            uint8_t col = m_input.GetLastTouchDownPosition().px / 8 ;
            uint8_t selectY = (row - (2 + levelOffsetY*2)) / 2 ;
            uint8_t selectX = (col - (6 + levelOffsetX*2)) / 2 ;               
            if ((row >= 2 + levelOffsetY * 2) && (row < 2 + (levelOffsetY + levelHeight) * 2))
            {
              if ((col >= 6 + levelOffsetX * 2) && (col < 6 + (levelOffsetX + levelWidth) * 2))
              {
                uint8_t newSelect = GetOccupyingCar(selectX, selectY) ;
                if (newSelect != 0xff)
                {
                  m_selectedCar = newSelect ;
                }
              }
            }
            if ((m_input.GetLastTouchDownPosition().py >= 9*16+8) && (m_input.GetLastTouchDownPosition().py < 11*16+8))
            {
              if ((m_input.GetLastTouchDownPosition().px >= 21*8) && (m_input.GetLastTouchDownPosition().px < 30*8))
              {
                ChangeState(GAMESTATE_LEVELPAUSED) ;
              }
            }            
          }
          break;
        case KEY_LEFT:
          if (CanMoveLeft())
          {
            m_carData[m_selectedCar].x-- ;
            m_moves++ ;
          }
          break;
        case KEY_RIGHT:
          if (CanMoveRight())
          {
            m_carData[m_selectedCar].x++ ;
            m_moves++ ;
          }
          break;
        case KEY_DOWN:
          if (CanMoveDown())
          {
            m_carData[m_selectedCar].y++ ;
            m_moves++ ;
          }
          break;
        case KEY_UP:
          if (CanMoveUp())
          {
            m_carData[m_selectedCar].y-- ;
            m_moves++ ;
          }
          break;
          break;
        case KEY_Y: // Left
          SelectNextCarLeft() ;
          break;
        case KEY_A: // Right
          SelectNextCarRight() ;
          break;
        case KEY_B: // Down
          SelectNextCarDown() ;
          break;
        case KEY_X: // Up
          SelectNextCarUp() ;
          break;          
        case KEY_START:
//        case KEY_SELECT:
          ChangeState(GAMESTATE_LEVELPAUSED) ;
          break;

      }
    }
  }
  keys = m_input.GetKeysHeld() ;
  for (unsigned int key=0;key<sizeof(keys)*8;key++)
  {
    if (keys & (1 << key))
    {
      switch (1 << key)
      {
        case KEY_TOUCH:
          {
            // this might be drag and drop
            int16_t moveX = (int16_t)m_input.GetLastTouchDownPosition().px - m_input.GetLastTouchPosition().px ;
            int16_t moveY = (int16_t)m_input.GetLastTouchDownPosition().py - m_input.GetLastTouchPosition().py ;
            bool handled = false ;
            if (moveX <= -16)
            {
              // try to move right
              if (CanMoveRight())
              {
                m_carData[m_selectedCar].x++ ;
                m_moves++ ;
              }     
              handled = true ;
            } else if (moveX >= 16)
            {              
              // try to move left
              if (CanMoveLeft())
              {
                m_carData[m_selectedCar].x-- ;
                m_moves++ ;
              }     
              handled = true ;
            }
            if (moveY >= 16)
            {
              // try to move up
              if (CanMoveUp())
              {
                m_carData[m_selectedCar].y-- ;
                m_moves++ ;
              }     
              handled = true ;
            } else if (moveY <= -16)
            {              
              if (CanMoveDown())
              {
                m_carData[m_selectedCar].y++ ;
                m_moves++ ;
              }     
              handled = true ;
            }
            if (handled)
            {
              // reset point of touch down, so we will not move it instantly again but use
              // the new position as the start of movement
              m_input.AccountDrag() ;
            }
          } 
      }
    }
  }
  UpdateCarsOnScreen();
  UpdateLevelStats() ;
  return true ;
}

void CGAME::UpdateLevelStats()
{
  char buffer[20] ;
  sprintf(buffer, "%i Moves", m_moves) ;
  m_subText->SetText(0,0,buffer) ;
  uint32_t lTime = GetCurrentLevelTime() ;
  sprintf(buffer, "%li.%li s", lTime / 1000, (lTime / 100) % 10) ;
  m_subText->SetText(16,0,buffer) ;
#if DEBUG
  sprintf(buffer, "%li", GetTimerTicks()) ;
  m_subText->SetText(0,1,buffer) ;
  sprintf(buffer, "%li", m_levelTime) ;
  m_subText->SetText(16,1,buffer) ;
  sprintf(buffer, "%li", m_timeOfResume) ;
  m_subText->SetText(0,2,buffer) ;
#endif
}

void CGAME::SelectNextCarLeft() 
{
  uint8_t row = m_carData[m_selectedCar].y ;
  uint8_t col = m_carData[m_selectedCar].x ;
  unsigned int candidate = m_selectedCar ;
  uint8_t c_row = m_carData[candidate].y ;
  uint8_t c_col = m_carData[candidate].x ;
  
  for (unsigned int car = 0; car < m_carCount; car++)
  {
    if (m_selectedCar == car)
      continue;
    uint8_t t_row = m_carData[car].y ;
    uint8_t t_col = m_carData[car].x ;
    if (t_col >= col)
      continue ;
    if (candidate != m_selectedCar)
    {
      if (t_col == c_col)
      {
          // candidate in same col, depends on distance to current in rows
          unsigned int c_d = ((int)c_row - row) * ((int)c_row - row) ;
          unsigned int t_d = ((int)t_row - row) * ((int)t_row - row) ;
          if (t_d < c_d)
          {
            candidate = car ; 
            c_col = t_col ;
            c_row = t_row ;
          }
      } else if (t_col > c_col)
      {
        // this car is nearer in columns, so select this
        candidate = car ; 
        c_col = t_col ;
        c_row = t_row ;
      }      
    } else
    {
      candidate = car ; 
      c_col = t_col ;
      c_row = t_row ;
    }
  }
  m_selectedCar = candidate ;
}

void CGAME::SelectNextCarRight() 
{
  uint8_t row = m_carData[m_selectedCar].y ;
  uint8_t col = m_carData[m_selectedCar].x ;
  unsigned int candidate = m_selectedCar ;
  uint8_t c_row = m_carData[candidate].y ;
  uint8_t c_col = m_carData[candidate].x ;
  
  for (unsigned int car = 0; car < m_carCount; car++)
  {
    if (m_selectedCar == car)
      continue;
    uint8_t t_row = m_carData[car].y ;
    uint8_t t_col = m_carData[car].x ;
    if (t_col <= col)
      continue ;
    if (candidate != m_selectedCar)
    {
      if (t_col == c_col)
      {
          // candidate in same col, depends on distance to current in rows
          unsigned int c_d = ((int)c_row - row) * ((int)c_row - row) ;
          unsigned int t_d = ((int)t_row - row) * ((int)t_row - row) ;
          if (t_d < c_d)
          {
            candidate = car ; 
            c_col = t_col ;
            c_row = t_row ;
          }
      } else if (t_col < c_col)
      {
        // this car is nearer in columns, so select this
        candidate = car ; 
        c_col = t_col ;
        c_row = t_row ;
      }      
    } else
    {
      candidate = car ; 
      c_col = t_col ;
      c_row = t_row ;
    }
  }
  m_selectedCar = candidate ;
}

void CGAME::SelectNextCarUp() 
{
  uint8_t row = m_carData[m_selectedCar].y ;
  uint8_t col = m_carData[m_selectedCar].x ;
  unsigned int candidate = m_selectedCar ;
  uint8_t c_row = m_carData[candidate].y ;
  uint8_t c_col = m_carData[candidate].x ;
  
  for (unsigned int car = 0; car < m_carCount; car++)
  {
    if (m_selectedCar == car)
      continue;
    uint8_t t_row = m_carData[car].y ;
    uint8_t t_col = m_carData[car].x ;
    if (t_row >= row)
      continue ;
    if (candidate != m_selectedCar)
    {
      if (t_row == c_row)
      {
          // candidate in same row, depends on distance to current in cols
          unsigned int c_d = ((int)c_col - col) * ((int)c_col - col) ;
          unsigned int t_d = ((int)t_col - col) * ((int)t_col - col) ;
          if (t_d < c_d)
          {
            candidate = car ; 
            c_col = t_col ;
            c_row = t_row ;
          }
      } else if (t_row > c_row)
      {
        // this car is nearer in columns, so select this
        candidate = car ; 
        c_col = t_col ;
        c_row = t_row ;
      }      
    } else
    {
      candidate = car ; 
      c_col = t_col ;
      c_row = t_row ;
    }
  }
  m_selectedCar = candidate ;
}

void CGAME::SelectNextCarDown() 
{
  uint8_t row = m_carData[m_selectedCar].y ;
  uint8_t col = m_carData[m_selectedCar].x ;
  unsigned int candidate = m_selectedCar ;
  uint8_t c_row = m_carData[candidate].y ;
  uint8_t c_col = m_carData[candidate].x ;
  
  for (unsigned int car = 0; car < m_carCount; car++)
  {
    if (m_selectedCar == car)
      continue;
    uint8_t t_row = m_carData[car].y ;
    uint8_t t_col = m_carData[car].x ;
    if (t_row <= row)
      continue ;
    if (candidate != m_selectedCar)
    {
      if (t_row == c_row)
      {
          // candidate in same row, depends on distance to current in cols
          unsigned int c_d = ((int)c_col - col) * ((int)c_col - col) ;
          unsigned int t_d = ((int)t_col - col) * ((int)t_col - col) ;
          if (t_d < c_d)
          {
            candidate = car ; 
            c_col = t_col ;
            c_row = t_row ;
          }
      } else if (t_row < c_row)
      {
        // this car is nearer in columns, so select this
        candidate = car ; 
        c_col = t_col ;
        c_row = t_row ;
      }      
    } else
    {
      candidate = car ; 
      c_col = t_col ;
      c_row = t_row ;
    }
  }
  m_selectedCar = candidate ;
}

uint8_t CGAME::GetOccupyingCar(uint8_t x, uint8_t y) 
{
  uint8_t tmp = 0xff ;
  for (unsigned int car = 0; car < m_carCount; car++)
  {
    if ((m_carData[car].orientation == 0) || (m_carData[car].orientation == 2))
    {
      // vertical car
      if ((m_carData[car].x == x) && (m_carData[car].y <= y) && (m_carData[car].y + 1 + m_carData[car].size > y))
        return car ;
    } else
    {
      if ((m_carData[car].y == y) && (m_carData[car].x <= x) && (m_carData[car].x + 1 + m_carData[car].size > x))
        return car ;
    }
  }
  return tmp ;
}

bool CGAME::IsGameWon() 
{
 // if there is a car in the exit, the game is won
  return GetOccupyingCar(levelWidth, (levelHeight / 2) - 1) != 0xff ;
}


bool CGAME::CanMoveLeft() 
{
  // cant move vertical car horizontally
  if ((m_carData[m_selectedCar].orientation == 0) || (m_carData[m_selectedCar].orientation == 2))
  {
    return false ;
  }
  // cant move beyond borders
  if (m_carData[m_selectedCar].x <= 0)
  {
    return false ;
  }
  uint8_t occupation = GetOccupyingCar(m_carData[m_selectedCar].x-1, m_carData[m_selectedCar].y) ;
  return (occupation == 0xff) ;  
}

bool CGAME::CanMoveRight() 
{
  // cant move vertical car horizontally
  if ((m_carData[m_selectedCar].orientation == 0) || (m_carData[m_selectedCar].orientation == 2))
  {
    return false ;
  }
  // special case: the wining move. target car is at y = (height / 2)-1 and x = width - 2
  if (m_carData[m_selectedCar].specials == SPECIALS_TARGET)
  {
    if ((m_carData[m_selectedCar].x == levelWidth - 2) && (m_carData[m_selectedCar].y == (levelHeight / 2) - 1))
      return true ;
  }
  // cant move beyond borders
  if (m_carData[m_selectedCar].x+m_carData[m_selectedCar].size+1 >= levelWidth)
  {
    return false ;
  }
  uint8_t occupation = GetOccupyingCar(m_carData[m_selectedCar].x+m_carData[m_selectedCar].size+1, m_carData[m_selectedCar].y) ;
  return (occupation == 0xff) ;  
}

bool CGAME::CanMoveUp() 
{
  // cant move horizontal car vertically
  if ((m_carData[m_selectedCar].orientation == 1) || (m_carData[m_selectedCar].orientation == 3))
  {
    return false ;
  }
  // cant move beyond borders
  if (m_carData[m_selectedCar].y <= 0)
  {
    return false ;
  }
  uint8_t occupation = GetOccupyingCar(m_carData[m_selectedCar].x, m_carData[m_selectedCar].y-1) ;
  return (occupation == 0xff) ;  
}

bool CGAME::CanMoveDown() 
{
  // cant move horizontal car vertically
  if ((m_carData[m_selectedCar].orientation == 1) || (m_carData[m_selectedCar].orientation == 3))
  {
    return false ;
  }
  // cant move beyond borders
  if (m_carData[m_selectedCar].y+m_carData[m_selectedCar].size+1 >= levelHeight)
  {
    return false ;
  }
  uint8_t occupation = GetOccupyingCar(m_carData[m_selectedCar].x, m_carData[m_selectedCar].y+m_carData[m_selectedCar].size+1) ;
  return (occupation == 0xff) ;  
}
