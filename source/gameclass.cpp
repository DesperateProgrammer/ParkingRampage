#include "gameclass.h"
#include "leveltiles.h"
#include "sprite.h"
#include <stdio.h>
#include <nds.h>

#include "state_loading.h"
#include "state_mainmenu.h"
#include "state_level_select.h"
#include "state_level_loading.h"
#include "state_level_running.h"
#include "state_level_paused.h"
#include "state_level_won.h"

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
    
  
  m_mainText = new CTEXTOVERLAY(0) ;
  m_subText = new CTEXTOVERLAY(1) ;
  
 
  
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
  
  m_time = new CTIMEMANAGER() ;


  videoSetModeSub(MODE_2_2D | DISPLAY_BG0_ACTIVE | DISPLAY_BG3_ACTIVE);
  videoSetMode(MODE_2_2D | DISPLAY_BG0_ACTIVE | DISPLAY_BG3_ACTIVE);
  
  m_mainText->Initialize() ;
  m_subText->Initialize() ;
  
  vramSetBankD(VRAM_D_SUB_SPRITE);
  oamInit(&oamSub, SpriteMapping_1D_128, false);
	dmaCopy(spritePal, SPRITE_PALETTE_SUB, 512);
  
  LoadCarSprites();
  
  /* 90° rotation matrix */
  oamRotateScale(&oamSub, 0, degreesToAngle(90), intToFixed(1, 8), intToFixed(1, 8));
  
  m_levelManager = new CLEVELMANAGER(this) ;

  m_audio = CAUDIOSTREAMING::GetInstance() ;
  RegisterState(new CGAMELOADINGSTATE(this), GAMESTATE_LOADING) ;
  RegisterState(new CMAINMENUSTATE(this), GAMESTATE_MAINMENU) ;
  RegisterState(new CLEVELLOADINGSTATE(this), GAMESTATE_LEVELLOADING) ;
  RegisterState(new CLEVELPAUSEDSTATE(this), GAMESTATE_LEVELPAUSED) ;
  RegisterState(new CLEVELRUNNINGSTATE(this), GAMESTATE_LEVELRUNNING) ;
  RegisterState(new CLEVELSELECTSTATE(this), GAMESTATE_LEVELSELECT) ;
  RegisterState(new CLEVELWONSTATE(this), GAMESTATE_LEVELWON) ;
  ChangeState(GAMESTATE_LOADING);
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

void CGAME::OnEveryTick()
{
  swiWaitForVBlank();
  m_input.Tick() ;
  
  for (int screen=0;screen<2;screen++)
  {  
    UpdateFading(screen);
    UpdateRotScale(screen) ;
  }
}


 