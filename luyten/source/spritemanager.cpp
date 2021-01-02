#include "spritemanager.h"

CSPRITEMANAGER::CSPRITEMANAGER(uint8_t screen) 
{
  m_screen = screen ;
  memset(&m_mapping[0], 0xff, sizeof(m_mapping));
  /* 90° rotation matrix */
  if (m_screen == 0)
    oamRotateScale(&oamMain, 0, degreesToAngle(90), intToFixed(1, 8), intToFixed(1, 8));
  else
    oamRotateScale(&oamSub, 0, degreesToAngle(90), intToFixed(1, 8), intToFixed(1, 8));  
}
    
void CSPRITEMANAGER::LoadSprite(uint16_t id, uint8_t *src, uint16_t length, SpriteSize spriteSize) 
{
  CSPRITEDATA data ;
  
  data.m_inUse = false ;
  data.m_lastUsed = 0 ;
  
  data.m_spriteSize = spriteSize ;
  if (m_screen == 0)
    data.m_dataSource = oamAllocateGfx(&oamMain, spriteSize, SpriteColorFormat_256Color);
  else
    data.m_dataSource = oamAllocateGfx(&oamSub, spriteSize, SpriteColorFormat_256Color);

	dmaCopy(src, data.m_dataSource, length);
  
  m_knownSprites[id] = data ;  
}

void CSPRITEMANAGER::SetSprite(uint8_t slot, uint16_t sprite, uint16_t x, uint16_t y, uint8_t orientation)
{
  UnloadSlot(slot) ;
  m_mapping[slot] = sprite ;
  OamState *oam = &oamMain ;
  if (m_screen)
    oam = &oamSub ;
  int corX = 0 ;
  int corY = 0 ;
  if (m_knownSprites[sprite].m_spriteSize == SpriteSize_16x32)
  {
    corY = 25 ;
  } else
  {
    corY = 9 ;
    corX = 8 ;
  }
  switch (orientation)
  {
    case SPRITE_ORIENTATION_DOWN:
      oamSet(oam, slot, x, y, 0, 0, m_knownSprites[sprite].m_spriteSize, SpriteColorFormat_256Color, 
            m_knownSprites[sprite].m_dataSource, -1, false, false, false, false, false);
      break;
    case SPRITE_ORIENTATION_UP:
      oamSet(oam, slot, x, y, 0, 0, m_knownSprites[sprite].m_spriteSize, SpriteColorFormat_256Color, 
            m_knownSprites[sprite].m_dataSource, -1, false, false, true, true, false);
      break;
    case SPRITE_ORIENTATION_RIGHT:
    case SPRITE_ORIENTATION_LEFT:
      oamSet(oam, slot, x - corX, y - corY, 0, 0, m_knownSprites[sprite].m_spriteSize, SpriteColorFormat_256Color, 
            m_knownSprites[sprite].m_dataSource, 0, true, false, false, false, false);
      break;
      
  }  
}

void CSPRITEMANAGER::DisableAll() 
{
  for (int i=0;i<128;i++)
  {
    if (m_screen == 0)
      oamClearSprite(&oamMain, i);
    else
      oamClearSprite(&oamSub, i);
    m_knownSprites[m_mapping[i]].m_inUse = false ;
    m_mapping[i] = 0xffff ;
  }
  Update() ;
}

void CSPRITEMANAGER::Update() 
{
  if (m_screen == 0)
    oamUpdate(&oamMain);  
  else
    oamUpdate(&oamSub);  
}