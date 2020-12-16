#include "gameclass.h"
#include <nds.h>

void CGAME::LoadLevel(uint16_t level) 
{
  UnloadLevel() ;
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

void CGAME::UnloadLevel() 
{
  for (int i=0;i<CARCOUNT_MAX;i++)
  {
    memset(m_carData+i, 0, sizeof(SLEVELENTITY)) ;
  }
}

void CGAME::StartLevel()
{
  UnloadLevel() ;
  StartFade(SCREEN_BOTTOM, eFADEOUT, LEVEL_FADETIME) ;
  ChangeState(GAMESTATE_LEVELLOADING);
}

uint16_t CGAME::GetLevelCountForDifficulty(const EDIFFICULTY targetDiff) 
{
  uint16_t i=0;
  uint16_t cnt = 0 ;
  while (gLevels[i].width)
  {
    if (gLevels[i].difficulty == targetDiff)
      cnt++ ;
    i++;
  }
  return cnt;
}

uint16_t CGAME::GetLevel(const EDIFFICULTY targetDiff, uint16_t index)
{
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
