#include "levelmanager.h"
#include "leveltiles.h"
#include "murmur3.h"
#include "storage.h"
#include <nds.h>
#include <stdio.h>

CLEVELMANAGER::CLEVELMANAGER(class CGAME *game)
{
  m_game = game ;
  m_levelTiles = new CTILEMAP(1, (uint16_t *)leveltilesTiles, leveltilesTilesLen, (uint16_t *)leveltilesPal, leveltilesPalLen, (uint16_t *)leveltilesMap, leveltilesMapLen) ;

  UnloadLevel() ;
}

uint32_t CLEVELMANAGER::GetLevelHash(uint32_t index) 
{
  CMURMUR3HASH hash = CMURMUR3HASH(0) ;
  hash.Feed(gLevels[index].width) ;
  hash.Feed(gLevels[index].height) ;
  hash.Feed(gLevels[index].entityCount) ;
  for (int i=0;i<gLevels[index].entityCount;i++)
  {
    hash.Feed(gLevels[index].entities[i].orientation & 1) ;
    hash.Feed(gLevels[index].entities[i].size) ;
    hash.Feed(gLevels[index].entities[i].x) ;
    hash.Feed(gLevels[index].entities[i].y) ;
  }
  return hash.Finalize(12 + gLevels[index].entityCount*4*4) ;
}

bool CLEVELMANAGER::GetHighScore(uint32_t index, uint32_t &time, uint32_t &moves, std::string &player) 
{
  time = (uint32_t)-1 ;
  moves = (uint32_t)-1 ;
  player = "";
  uint32_t levelHash = GetLevelHash(index) ;
  CTLV *highscores = CSTORAGE::GetInstance()->GetConfig(CFGTAG_HIGHSCORES) ;
  for (uint32_t i=0;i<highscores->GetChildCount();i++)
  {
    CTLV *child = highscores->GetChild(i) ;
    if (child->GetTag() == CFGTAG_LEVELDATA(levelHash))
    {
      for (uint32_t q=0;q<child->GetChildCount();q++)
      {
        CTLV *entry = child->GetChild(q) ;
        switch (entry->GetTag())
        {
          case CFGTAG_LEVELMOVES:
            moves = entry->GetUInt32() ;
            break ;
          case CFGTAG_LEVELTIME:
            time = entry->GetUInt32() ;
            break ;
          case CFGTAG_PLAYER:
            player = entry->GetString() ;
            break ;
        }
      }
      return true ;
    }
  }
  return false ;
}

void CLEVELMANAGER::SetHighScore(uint32_t index, uint32_t time, uint32_t moves, std::string player) 
{
  uint32_t levelHash = GetLevelHash(index) ;
  CTLV *highscores = CSTORAGE::GetInstance()->GetConfig(CFGTAG_HIGHSCORES) ;
  CTLV *level = 0 ;
  for (uint32_t i=0;i<highscores->GetChildCount();i++)
  {
    CTLV *child = highscores->GetChild(i) ;
    if (child->GetTag() == CFGTAG_LEVELDATA(levelHash))
    {
      level = child ;
      break ;
    }
  }
  if (!level)
  {
    level = new CTLV(CFGTAG_LEVELDATA(levelHash)) ;
    highscores->AddChild(level) ;
  }
  bool playerDone = false, timeDone = false, moveDone = false ;
  for (uint32_t q=0;q<level->GetChildCount();q++)
  {
    CTLV *entry = level->GetChild(q) ;
    switch (entry->GetTag())
    {
      case CFGTAG_LEVELMOVES:
        entry->SetUInt32(moves) ;
        moveDone = true ;
        break ;
      case CFGTAG_LEVELTIME:
        entry->SetUInt32(time) ;
        timeDone = true ;
        break ;
      case CFGTAG_PLAYER:
        playerDone = true ;
        entry->SetString(player) ;
        break ;
    }
  }
  if (!moveDone)
  {
    CTLV *entry = new CTLV(CFGTAG_LEVELMOVES) ;
    entry->SetUInt32(moves) ;
    level->AddChild(entry) ;
  }
  if (!timeDone)
  {
    CTLV *entry = new CTLV(CFGTAG_LEVELTIME) ;
    entry->SetUInt32(time) ;
    level->AddChild(entry) ;
  }
  if (!playerDone)
  {
    CTLV *entry = new CTLV(CFGTAG_PLAYER) ;
    entry->SetString(player) ;
    level->AddChild(entry) ;
  }
  CSTORAGE::GetInstance()->Flush() ;
}

void CLEVELMANAGER::LoadLevel(uint16_t level) 
{
  UnloadLevel() ;
  m_levelTiles->Initialize() ;
  levelNumber = level ;
  levelOffsetX = (10 - gLevels[level].width) / 2 ;
  levelOffsetY = (10 - gLevels[level].height) / 2 ;
  levelWidth = gLevels[level].width ;
  levelHeight = gLevels[level].height ;
  // prepare background
  for (int i=0;i<10;i++)
  {
    for (int q=0;q<10;q++)
    {
      if ((i < levelOffsetX) || (i >= 10-levelOffsetX) || (q < levelOffsetY) || (q >= 10-levelOffsetY))
      {
        m_levelTiles->SetMap((i*2)+6,(q*2)+2, 1) ;
        m_levelTiles->SetMap((i*2)+7,(q*2)+2, 2) ;
        m_levelTiles->SetMap((i*2)+6,(q*2)+3, 3) ;
        m_levelTiles->SetMap((i*2)+7,(q*2)+3, 4) ;
      } else
      {
        m_levelTiles->SetMap((i*2)+6,(q*2)+2, 5) ;
        m_levelTiles->SetMap((i*2)+7,(q*2)+2, 6) ;
        m_levelTiles->SetMap((i*2)+6,(q*2)+3, 7) ;
        m_levelTiles->SetMap((i*2)+7,(q*2)+3, 8) ;
      }
    }
  }
  // prepare exit
  for (int i=(13-levelOffsetX);i<16;i++)
  {
    m_levelTiles->SetMap(i*2, 10, 9) ;
    m_levelTiles->SetMap(i*2+1, 10, 10) ;
    m_levelTiles->SetMap(i*2, 11, 11) ;
    m_levelTiles->SetMap(i*2+1, 11, 12) ;
  }
  // and now place the cars
  for (int i=0;i<gLevels[level].entityCount;i++)
  {
    memcpy(m_carData+i, gLevels[level].entities+i, sizeof(SLEVELENTITY)) ;
  }
  m_carCount = gLevels[level].entityCount ;
}

void CLEVELMANAGER::UnloadLevel() 
{
  for (int i=0;i<CARCOUNT_MAX;i++)
  {
    memset(m_carData+i, 0, sizeof(SLEVELENTITY)) ;
  }
}

uint16_t CLEVELMANAGER::GetLevelCountForDifficulty(const EDIFFICULTY targetDiff)
{
  uint16_t i=0;
  uint16_t cnt = 0 ;
  while (gLevels[i].width)
  {
    if ((gLevels[i].difficulty == targetDiff) || (targetDiff == eANY))
      cnt++ ;
    i++;
  }
  return cnt;
}

uint16_t CLEVELMANAGER::GetLevel(const EDIFFICULTY targetDiff, uint16_t index)
{
  if (targetDiff == eANY)
  {
    return index ;
  }
  uint16_t i=0;
  uint16_t cnt = 0 ;
  while (gLevels[i].width)
  {
    if (gLevels[i].difficulty == targetDiff)
    {
      if (cnt == index)
        return i ;
      cnt++ ;
    }
    i++;
  }
  return 0 ;
}

void CLEVELMANAGER::UpdateLevelStats()
{
  char buffer[20] ;
  sprintf(buffer, "%i Moves", GetMoveCount()) ;
  m_game->GetSubText()->SetText(0,0,buffer) ;
  uint32_t lTime = GetCurrentLevelTime() ;
  sprintf(buffer, "%li.%li s", lTime / 1000, (lTime / 100) % 10) ;
  m_game->GetSubText()->SetText(16,0,buffer) ;
}


void CLEVELMANAGER::UpdateCarsOnScreen(bool selectorShown) 
{
  if (selectorShown)
  {
    // and draw the selector
    uint16_t selectorX = 40 + levelOffsetX * 16 + m_carData[m_selectedCar].x * 16, selectorY = 8 + levelOffsetY * 16 + m_carData[m_selectedCar].y * 16;
    selectorX += m_selectedMoveOffsetX ;
    selectorY += m_selectedMoveOffsetY ;
    switch (m_carData[m_selectedCar].orientation)
    {
      case ORIENTATION_LEFT:
      case ORIENTATION_RIGHT:
        selectorX += m_carData[m_selectedCar].size * 8 ;
        break;
      case ORIENTATION_UP:
      case ORIENTATION_DOWN:
        selectorY += m_carData[m_selectedCar].size * 8 ;
        break;
    }
    // the selector will have some heartbeat scaling
    uint32_t time = m_game->GetTimeManager()->GetTimerTicks() ;
    uint32_t scale = (1 << (12 + 8)) / (2*sinLerp(time *50)) ;
    oamRotateScale(&oamSub, 1, 0, scale, scale) ;
    
    oamSet(&oamSub, 0, selectorX, selectorY, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color, 
         m_game->GetSubSprites()->GetSpriteSource(SPRITE_SELECTOR), 1, true, false, true, true, false);
  }


  for (int i=0;i<16;i++)
  {
    int moveOffsetX = 0 ;
    int moveOffsetY = 0 ;
    if (i == m_selectedCar)
    {
      moveOffsetX = m_selectedMoveOffsetX ;
      moveOffsetY = m_selectedMoveOffsetY ;
    }
    switch (m_carData[i].size)
    {
      case 2:
        {
          switch (m_carData[i].orientation)
          {
            case ORIENTATION_DOWN:
              m_game->GetSubSprites()->SetSprite(i*2+1, SPRITE_TRUCK_1(m_carData[i].specials), (m_carData[i].x+levelOffsetX)*16 + 48 + moveOffsetX,  (m_carData[i].y+levelOffsetY)*16 + 16 + moveOffsetY, m_carData[i].orientation) ;
              m_game->GetSubSprites()->SetSprite(i*2+2, SPRITE_TRUCK_2(m_carData[i].specials), (m_carData[i].x+levelOffsetX)*16 + 48 + moveOffsetX,  (m_carData[i].y+levelOffsetY+2)*16 + 16 + moveOffsetY, m_carData[i].orientation) ;
              break;
            case ORIENTATION_UP:
              m_game->GetSubSprites()->SetSprite(i*2+1, SPRITE_TRUCK_1(m_carData[i].specials), (m_carData[i].x+levelOffsetX)*16 + 48 + moveOffsetX,  (m_carData[i].y+levelOffsetY+1)*16 + 16 + moveOffsetY, m_carData[i].orientation) ;
              m_game->GetSubSprites()->SetSprite(i*2+2, SPRITE_TRUCK_2(m_carData[i].specials), (m_carData[i].x+levelOffsetX)*16 + 48 + moveOffsetX,  (m_carData[i].y+levelOffsetY)*16 + 16 + moveOffsetY, m_carData[i].orientation) ;
              break;
            case ORIENTATION_RIGHT:
            case ORIENTATION_LEFT:
              m_game->GetSubSprites()->SetSprite(i*2+1, SPRITE_TRUCK_1(m_carData[i].specials), (m_carData[i].x+levelOffsetX)*16 + 48 + moveOffsetX,  (m_carData[i].y+levelOffsetY)*16 + 16 + moveOffsetY, m_carData[i].orientation) ;
              m_game->GetSubSprites()->SetSprite(i*2+2, SPRITE_TRUCK_2(m_carData[i].specials), (m_carData[i].x+levelOffsetX+2)*16 + 48 + moveOffsetX,  (m_carData[i].y+levelOffsetY)*16 + 16 + moveOffsetY, m_carData[i].orientation) ;
              break;
              
          }
        }
        break;
      case 1:
        {
          m_game->GetSubSprites()->SetSprite(i*2+1, SPRITE_CAR(m_carData[i].specials), (m_carData[i].x+levelOffsetX)*16 + 48 + moveOffsetX,  (m_carData[i].y+levelOffsetY)*16 + 16 + moveOffsetY, m_carData[i].orientation) ;
          break;
        }
    }
  }  
  m_game->GetSubSprites()->Update() ; 
}

void CLEVELMANAGER::ResetLevel() 
{
  m_moves = 0 ;
  m_selectedCar = 0 ;
}

void CLEVELMANAGER::SelectNextCarLeft() 
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

void CLEVELMANAGER::SelectNextCarRight() 
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

void CLEVELMANAGER::SelectNextCarUp() 
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

void CLEVELMANAGER::SelectNextCarDown() 
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

uint8_t CLEVELMANAGER::GetOccupyingCar(uint8_t x, uint8_t y) 
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

bool CLEVELMANAGER::SelectCarFromTouch(touchPosition touch) 
{
  uint8_t row = touch.py / 8 ;
  uint8_t col = touch.px / 8 ;
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
        return true ;
      }
    }
  }
  return false ;
}


bool CLEVELMANAGER::IsLevelWon() 
{
 // if there is a car in the exit, the game is won
  return GetOccupyingCar(levelWidth, (levelHeight / 2) - 1) != 0xff ;
}

void CLEVELMANAGER::SetPartialMove(int mx, int my) 
{
  if (!mx && !my)
  {
    m_selectedMoveOffsetX = m_selectedMoveOffsetY = 0 ;
    return ;
  }
  if ((mx > 0) && (CanMoveRight()))
    m_selectedMoveOffsetX = mx ;
  if ((mx < 0) && (CanMoveLeft()))
    m_selectedMoveOffsetX = mx ;
  if ((my > 0) && (CanMoveDown()))
    m_selectedMoveOffsetY = my ;
  if ((my < 0) && (CanMoveUp()))
    m_selectedMoveOffsetY = my ;
}

bool CLEVELMANAGER::CanMoveLeft() 
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

bool CLEVELMANAGER::CanMoveRight() 
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

bool CLEVELMANAGER::CanMoveUp() 
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

bool CLEVELMANAGER::CanMoveDown() 
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

void CLEVELMANAGER::MoveLeft() 
{
  m_carData[m_selectedCar].x-- ;
  m_moves++ ;
}

void CLEVELMANAGER::MoveRight() 
{
  m_carData[m_selectedCar].x++ ;
  m_moves++ ;
}

void CLEVELMANAGER::MoveUp() 
{
  m_carData[m_selectedCar].y-- ;
  m_moves++ ;
}

void CLEVELMANAGER::MoveDown() 
{
  m_carData[m_selectedCar].y++ ;
  m_moves++ ;
}

uint32_t CLEVELMANAGER::GetCurrentLevelTime()
{
  if (m_timeOfResume)
    return m_levelTime + (m_game->GetTimeManager()->GetTimerTicks() - m_timeOfResume) ;
  else
    return m_levelTime ;
}

void CLEVELMANAGER::ResetLevelTime() 
{
  m_timeOfResume = 0 ;
  m_levelTime = 0 ;
}

void CLEVELMANAGER::PauseLevelTime() 
{
  uint32_t curTime = GetCurrentLevelTime() ;
  m_timeOfResume = 0 ;
  m_levelTime = curTime ;
}

void CLEVELMANAGER::ContinueLevelTime() 
{
  m_timeOfResume = m_game->GetTimeManager()->GetTimerTicks() ;
}
    