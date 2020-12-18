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

#define CONCAT(a,b,c,d) a##b##c##d

#define LOADSPRITE(id, width, height, offset) \
  m_spriteContent[id] = oamAllocateGfx(&oamSub, CONCAT(SpriteSize_,width,x,height), SpriteColorFormat_256Color);\
	dmaCopy((uint8_t*)spriteTiles + (offset), m_spriteContent[id], width*height)

void CGAME::LoadCarSprites()
{
  // Cars, Length 2
  LOADSPRITE(SPRITE_CAR_A,16,32,0) ;
  LOADSPRITE(SPRITE_CAR_TARGET,16,32,16*32) ;
  LOADSPRITE(SPRITE_CAR_B,16,32,6*16*32) ;
  LOADSPRITE(SPRITE_CAR_C,16,32,7*16*32) ;
  // Trucks, Length 3 in 2 parts
  LOADSPRITE(SPRITE_TRUCK_A_1,16,32,2*16*32) ;
  LOADSPRITE(SPRITE_TRUCK_A_2,16,16,8*16*32) ;
  LOADSPRITE(SPRITE_TRUCK_B_1,16,32,3*16*32) ;
  LOADSPRITE(SPRITE_TRUCK_B_2,16,16,9*16*32) ;
  // Bus, Length 4, Needs 2 Parts
  LOADSPRITE(SPRITE_BUS_1,16,32,4*16*32) ;
  LOADSPRITE(SPRITE_BUS_2,16,32,10*16*32) ;
  // Tank Truck, Length 4, Needs 2 Parts
  LOADSPRITE(SPRITE_TANKTRUCK_1,16,32,5*16*32) ;
  LOADSPRITE(SPRITE_TANKTRUCK_2,16,32,11*16*32) ;
  // Car Select Cursor
  LOADSPRITE(SPRITE_SELECTOR,16,16,8*16*32 + 1*16*16) ;
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
         m_spriteContent[SPRITE_SELECTOR], 1, true, false, true, true, false);
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
              dataA =  m_spriteContent[SPRITE_TRUCK_1(m_carData[i].specials)];
              dataB =  m_spriteContent[SPRITE_TRUCK_2(m_carData[i].specials)];
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
              data = m_spriteContent[SPRITE_CAR(m_carData[i].specials)];
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

 