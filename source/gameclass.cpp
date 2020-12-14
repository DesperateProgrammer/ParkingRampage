#include "gameclass.h"
#include "leveltiles.h"
#include "mainmenu.h"
#include "title.h"
#include "sprite.h"
#include <stdio.h>
#include <nds.h>

class CGAME *CGAME::m_instance = 0 ;

void CGAME::Run()
{
  Initialize();
  while (Tick());
  Destruct();
}

void CGAME::LoadCarSprites()
{
  // Car A, Length 2
  m_spriteContent[0] = oamAllocateGfx(&oamSub, SpriteSize_16x32, SpriteColorFormat_256Color);
	dmaCopy(spriteTiles, m_spriteContent[0], 16*32);
  // Target Car, length 2
  m_spriteContent[1] = oamAllocateGfx(&oamSub, SpriteSize_16x32, SpriteColorFormat_256Color);
	dmaCopy((uint8_t*)spriteTiles + 16*32, m_spriteContent[1], 16*32);
  // Car B, Length 2
  m_spriteContent[2] = oamAllocateGfx(&oamSub, SpriteSize_16x32, SpriteColorFormat_256Color);
	dmaCopy((uint8_t*)spriteTiles + 6*16*32, m_spriteContent[2], 16*32);
  // Car C, Length 2
  m_spriteContent[3] = oamAllocateGfx(&oamSub, SpriteSize_16x32, SpriteColorFormat_256Color);
	dmaCopy((uint8_t*)spriteTiles + 7*16*32, m_spriteContent[3], 16*32);
  // Truck A, Length 3, Needs 2 Parts
  m_spriteContent[4] = oamAllocateGfx(&oamSub, SpriteSize_16x32, SpriteColorFormat_256Color);
	dmaCopy((uint8_t*)spriteTiles + 2*16*32, m_spriteContent[4], 16*32);
  m_spriteContent[5] = oamAllocateGfx(&oamSub, SpriteSize_16x16, SpriteColorFormat_256Color);
	dmaCopy((uint8_t*)spriteTiles + 8*16*32, m_spriteContent[5], 16*16);
  // Truck B, Length 3, Needs 2 Parts
  m_spriteContent[6] = oamAllocateGfx(&oamSub, SpriteSize_16x32, SpriteColorFormat_256Color);
	dmaCopy((uint8_t*)spriteTiles + 3*16*32, m_spriteContent[6], 16*32);
  m_spriteContent[7] = oamAllocateGfx(&oamSub, SpriteSize_16x16, SpriteColorFormat_256Color);
	dmaCopy((uint8_t*)spriteTiles + 9*16*32, m_spriteContent[7], 16*16);
  // Bus, Length 4, Needs 2 Parts
  m_spriteContent[8] = oamAllocateGfx(&oamSub, SpriteSize_16x32, SpriteColorFormat_256Color);
	dmaCopy((uint8_t*)spriteTiles + 2*16*32, m_spriteContent[8], 16*32);
  m_spriteContent[9] = oamAllocateGfx(&oamSub, SpriteSize_16x32, SpriteColorFormat_256Color);
	dmaCopy((uint8_t*)spriteTiles + 8*16*32, m_spriteContent[9], 16*32);
  // Tank Truck, Length 4, Needs 2 Parts
  m_spriteContent[10] = oamAllocateGfx(&oamSub, SpriteSize_16x32, SpriteColorFormat_256Color);
	dmaCopy((uint8_t*)spriteTiles + 2*16*32, m_spriteContent[10], 16*32);
  m_spriteContent[11] = oamAllocateGfx(&oamSub, SpriteSize_16x32, SpriteColorFormat_256Color);
	dmaCopy((uint8_t*)spriteTiles + 8*16*32, m_spriteContent[11], 16*32);
  // Car Select Cursor
  m_spriteContent[12] = oamAllocateGfx(&oamSub, SpriteSize_16x16, SpriteColorFormat_256Color);
  dmaCopy((uint8_t*)spriteTiles + 8*16*32 + 1*16*16, m_spriteContent[12], 16*16);
}


void CGAME::Initialize()
{
  if (m_instance)
    return ; // Only one instance is allowed to initialize
  m_instance = this ;
    
  m_titleTiles = new CTILEMAP(1, (uint16_t *)titleTiles, titleTilesLen, (uint16_t *)titlePal, titlePalLen, (uint16_t *)titleMap, titleMapLen) ;
  m_mainScreenTiles = new CTILEMAP(0, (uint16_t *)titleTiles, titleTilesLen, (uint16_t *)titlePal, titlePalLen, (uint16_t *)titleMap, titleMapLen) ;
  m_mainMenuTiles = new CTILEMAP(1, (uint16_t *)mainmenuTiles, mainmenuTilesLen, (uint16_t *)mainmenuPal, mainmenuPalLen, (uint16_t *)mainmenuMap, mainmenuMapLen) ;
  m_levelTiles = new CTILEMAP(1, (uint16_t *)leveltilesTiles, leveltilesTilesLen, (uint16_t *)leveltilesPal, leveltilesPalLen, (uint16_t *)leveltilesMap, leveltilesMapLen) ;
  
  m_mainText = new CTEXTOVERLAY(0) ;
  m_subText = new CTEXTOVERLAY(1) ;
  
  m_audio = CAUDIOSTREAMING::GetInstance() ;
  m_audio->SetMusicVolume(m_musicVolume) ;
 
  
  for (int screen=0;screen<2;screen++)
  {
    // initial fading values
    m_fadeStart[screen] = m_fadeEnd[screen] = 0 ;
    m_fadeMode[screen] = eFADEIN ;
    // initial rotscale values
    m_rotscaleStart[screen] = 0 ;
    m_rotscaleEnd[screen] = 0 ;
    m_startScale[screen] = 1 << 12 ;
    m_endscale[screen] = 1 << 12 ;
    m_rotations[screen] = 0 ;
  }
  
  InitializeTime() ;


  videoSetModeSub(MODE_2_2D | DISPLAY_BG0_ACTIVE | DISPLAY_BG3_ACTIVE);
  videoSetMode(MODE_2_2D | DISPLAY_BG0_ACTIVE | DISPLAY_BG3_ACTIVE);
  
  m_levelTiles->Initialize() ;
  m_mainScreenTiles->Initialize() ;
  m_mainText->Initialize() ;
  m_subText->Initialize() ;
  
  vramSetBankD(VRAM_D_SUB_SPRITE);
  oamInit(&oamSub, SpriteMapping_1D_128, false);
	dmaCopy(spritePal, SPRITE_PALETTE_SUB, 512);
  
  LoadCarSprites();
  
  /* 90° rotation matrix */
  oamRotateScale(&oamSub, 0, degreesToAngle(90), intToFixed(1, 8), intToFixed(1, 8));
  
  memset(&m_carData, 0, sizeof(m_carData));
  UnloadLevel() ;

  
  ChangeState(GAMESTATE_LOADING);
} 

void CGAME::UpdateCarsOnScreen(bool selectorShown) 
{
  if (selectorShown)
  {
    // and draw the selector
    uint16_t selectorX = 40 + levelOffsetX * 16 + m_carData[m_selectedCar].x * 16, selectorY = 8 + levelOffsetY * 16 + m_carData[m_selectedCar].y * 16;
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
    uint32_t time = GetTimerTicks() ;
    uint32_t scale = (1 << (12 + 8)) / (2*sinLerp(time *50)) ;
    oamRotateScale(&oamSub, 1, 0, scale, scale) ;
    oamSet(&oamSub, 0, selectorX, selectorY, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color, 
         m_spriteContent[12], 1, true, false, true, true, false);
  }

  for (int i=0;i<16;i++)
  {
    switch (m_carData[i].size)
    {
      case 2:
        {
          uint16_t *dataA = 0, *dataB = 0 ;
          switch (m_carData[i].specials)
          {
            case 0:
            case 1:
              dataA =  m_spriteContent[m_carData[i].specials * 2 + 4];
              dataB =  m_spriteContent[m_carData[i].specials * 2 + 4 + 1];
              break;
            default:
              continue;
          }
          switch (m_carData[i].orientation)
          {
            case ORIENTATION_DOWN:
              oamSet(&oamSub, i*2+1, (m_carData[i].x+levelOffsetX)*16 + 48, (m_carData[i].y+levelOffsetY)*16 + 16, 0, 0, SpriteSize_16x32, SpriteColorFormat_256Color, 
                    dataA, -1, false, false, false, false, false);
              oamSet(&oamSub, i*2+2, (m_carData[i].x+levelOffsetX)*16 + 48, (m_carData[i].y+levelOffsetY+2)*16 + 16, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color, 
                    dataB, -1, false, false, false, false, false);
              break;
            case ORIENTATION_UP:
              oamSet(&oamSub, i*2+1, (m_carData[i].x+levelOffsetX)*16 + 48, (m_carData[i].y+levelOffsetY+1)*16 + 16, 0, 0, SpriteSize_16x32, SpriteColorFormat_256Color, 
                    dataA, -1, false, false, true, true, false);
              oamSet(&oamSub, i*2+2, (m_carData[i].x+levelOffsetX)*16 + 48, (m_carData[i].y+levelOffsetY+0)*16 + 16, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color, 
                    dataB, -1, false, false, true, true, false);
              break;
            case ORIENTATION_RIGHT:
            case ORIENTATION_LEFT:
              oamSet(&oamSub, i*2+1, (m_carData[i].x+levelOffsetX)*16 + 48, (m_carData[i].y+levelOffsetY)*16 + 16 - 25, 0, 0, SpriteSize_16x32, SpriteColorFormat_256Color, 
                    dataA, 0, true, false, false, false, false);
              oamSet(&oamSub, i*2+2, (m_carData[i].x+levelOffsetX+2)*16 + 48-8, (m_carData[i].y+levelOffsetY)*16 + 16 - 9, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color, 
                    dataB, 0, true, false, false, false, false);
              break;
              
          }
        }
        break;
      case 1:
        {
          uint16_t *data = 0 ;
          switch (m_carData[i].specials)
          {
            case 0:
            case 1:
            case 2:
            case 3:
              data = m_spriteContent[m_carData[i].specials];
              break;
            default:
              continue;
          }
          switch (m_carData[i].orientation)
          {
            case 0:
              oamSet(&oamSub, i*2+1, (m_carData[i].x+levelOffsetX)*16 + 48, (m_carData[i].y+levelOffsetY)*16 + 16, 0, 0, SpriteSize_16x32, SpriteColorFormat_256Color, 
                    data, -1, false, false, false, false, false);
              break;
            case 1:		
              oamSet(&oamSub, i*2+1, (m_carData[i].x+levelOffsetX)*16 + 48, (m_carData[i].y+levelOffsetY)*16 + 16 - 25, 0, 0, SpriteSize_16x32, SpriteColorFormat_256Color, 
                    data, 0, true, false, false, false, false);
              break;
            case 2:
              oamSet(&oamSub, i*2+1, (m_carData[i].x+levelOffsetX)*16 + 48, (m_carData[i].y+levelOffsetY)*16 + 16, 0, 0, SpriteSize_16x32, SpriteColorFormat_256Color, 
                    data, -1, false, false, true, true, false);
              break;
            case 3:		
              oamSet(&oamSub, i*2+1, (m_carData[i].x+levelOffsetX)*16 + 48, (m_carData[i].y+levelOffsetY)*16 + 16 - 25, 0, 0, SpriteSize_16x32, SpriteColorFormat_256Color, 
                    data, 0, true, false, true, true, false);
              break;
          } 
          break;
        }
    }
  }  
  oamUpdate(&oamSub);  
}

void CGAME::IncreaseDifficulty()
{
  switch (m_difficulty)
  {
    case eBEGINNER:     m_difficulty = eINTERMEDIATE; break;
    case eINTERMEDIATE: m_difficulty = eADVANCED; break;
    case eADVANCED:     m_difficulty = eEXPERT; break;
    case eEXPERT:       m_difficulty = eBEGINNER; break;
  }
}

void CGAME::DecreaseDifficulty()
{
  switch (m_difficulty)
  {
    case eBEGINNER:     m_difficulty = eEXPERT; break;
    case eINTERMEDIATE: m_difficulty = eBEGINNER; break;
    case eADVANCED:     m_difficulty = eINTERMEDIATE; break;
    case eEXPERT:       m_difficulty = eADVANCED; break;
  }
}

void CGAME::Destruct()
{
}


void CGAME::DisableAllSprites() 
{
  for (uint8_t i = 0;i<128;i++)
  {
    oamClearSprite(&oamSub, i);
  }
  oamUpdate(&oamSub);  
}

 