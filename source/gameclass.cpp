#include "gameclass.h"
#include "leveltiles.h"
#include "sprite.h"
#include "storage.h"
#include <stdio.h>
#include <nds.h>

#include "state_loading.h"
#include "state_mainmenu.h"
#include "state_level_select.h"
#include "state_level_loading.h"
#include "state_level_running.h"
#include "state_level_paused.h"
#include "state_level_won.h"
#include "state_control_menu.h"

class CGAME *CGAME::m_instance = 0 ;

void CGAME::Run()
{
  Initialize();
  while (Tick());
  Destruct();
}

void CGAME::LoadCarSprites()
{
  // Cars, Length 2
  m_subSprites->LoadSprite(SPRITE_CAR_A, (uint8_t*)spriteTiles + 0*16*32, 16*32, SpriteSize_16x32) ;
  m_subSprites->LoadSprite(SPRITE_CAR_TARGET, (uint8_t*)spriteTiles + 1*16*32, 16*32, SpriteSize_16x32) ;
  m_subSprites->LoadSprite(SPRITE_CAR_B, (uint8_t*)spriteTiles + 6*16*32, 16*32, SpriteSize_16x32) ;
  m_subSprites->LoadSprite(SPRITE_CAR_C, (uint8_t*)spriteTiles + 7*16*32, 16*32, SpriteSize_16x32) ;
  // Trucks, Length 3 in 2 parts
  m_subSprites->LoadSprite(SPRITE_TRUCK_A_1, (uint8_t*)spriteTiles + 2*16*32, 16*32, SpriteSize_16x32) ;
  m_subSprites->LoadSprite(SPRITE_TRUCK_A_2, (uint8_t*)spriteTiles + 8*16*32, 16*16, SpriteSize_16x16) ;
  m_subSprites->LoadSprite(SPRITE_TRUCK_B_1, (uint8_t*)spriteTiles + 3*16*32, 16*32, SpriteSize_16x32) ;
  m_subSprites->LoadSprite(SPRITE_TRUCK_B_2, (uint8_t*)spriteTiles + 9*16*32, 16*16, SpriteSize_16x16) ;    
  // Bus, Length 4, Needs 2 Parts
  m_subSprites->LoadSprite(SPRITE_BUS_1, (uint8_t*)spriteTiles + 4*16*32, 16*32, SpriteSize_16x32) ;
  m_subSprites->LoadSprite(SPRITE_BUS_2, (uint8_t*)spriteTiles + 10*16*32, 16*32, SpriteSize_16x32) ;
  // Tank Truck, Length 4, Needs 2 Parts
  m_subSprites->LoadSprite(SPRITE_TANKTRUCK_1, (uint8_t*)spriteTiles + 5*16*32, 16*32, SpriteSize_16x32) ;
  m_subSprites->LoadSprite(SPRITE_TANKTRUCK_2, (uint8_t*)spriteTiles + 11*16*32, 16*32, SpriteSize_16x32) ;
  // Car Select Cursor
  m_subSprites->LoadSprite(SPRITE_SELECTOR, (uint8_t*)spriteTiles + 8*16*32 + 1*16*16, 16*16, SpriteSize_16x16) ;    
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
  
  m_mainSprites = new CSPRITEMANAGER(0) ;
  m_subSprites = new CSPRITEMANAGER(1)  ;
  
  LoadCarSprites();
    
  m_levelManager = new CLEVELMANAGER(this) ;

  m_audio = CAUDIOSTREAMING::GetInstance() ;
  
  uint32_t mVolume = CSTORAGE::GetInstance()->GetConfig(CFGTAG_VOLUME_MUSIC)->GetUInt32() ;
  uint32_t eVolume = CSTORAGE::GetInstance()->GetConfig(CFGTAG_VOLUME_SFX)->GetUInt32() ;
  m_audio->SetMusicVolume(mVolume) ;
  m_audio->SetEffectVolume(eVolume) ;
  
  uint32_t keyMapping = CSTORAGE::GetInstance()->GetConfig(CFGTAG_VOLUME_KEYMAPPING)->GetUInt32() ;
  GetInputManager()->ActivatePreconfiguredMapping((int8_t)keyMapping) ;
  
  for (uint32_t i=0;i<PersonalData->nameLen;i++)
  {
    m_nickname += (char)(PersonalData->name[i]) ;
  }
  
  RegisterState(new CGAMELOADINGSTATE(this), GAMESTATE_LOADING) ;
  RegisterState(new CMAINMENUSTATE(this), GAMESTATE_MAINMENU) ;
  RegisterState(new CLEVELLOADINGSTATE(this), GAMESTATE_LEVELLOADING) ;
  RegisterState(new CLEVELPAUSEDSTATE(this), GAMESTATE_LEVELPAUSED) ;
  RegisterState(new CLEVELRUNNINGSTATE(this), GAMESTATE_LEVELRUNNING) ;
  RegisterState(new CLEVELSELECTSTATE(this), GAMESTATE_LEVELSELECT) ;
  RegisterState(new CLEVELWONSTATE(this), GAMESTATE_LEVELWON) ;
  RegisterState(new CCONTROLMENUSTATE(this), GAMESTATE_CONFIG) ;
  
  ChangeState(GAMESTATE_LOADING);
} 

void CGAME::Destruct()
{
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


 