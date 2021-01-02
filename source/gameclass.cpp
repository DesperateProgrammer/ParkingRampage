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

#include "soundbank.h"

class CGAME *CGAME::m_instance = 0 ;

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
  
  CGAMEBASE::Initialize() ;
    
  
  LoadCarSprites();
    
  m_levelManager = new CLEVELMANAGER(this) ;

  uint32_t mVolume = CSTORAGE::GetInstance()->GetConfig(CFGTAG_VOLUME_MUSIC)->GetUInt32() ;
  uint32_t eVolume = CSTORAGE::GetInstance()->GetConfig(CFGTAG_VOLUME_SFX)->GetUInt32() ;
  m_audio->SetMusicVolume(mVolume) ;
  m_audio->SetEffectVolume(eVolume) ;


  uint32_t effects[] =  { SFX_START, SFX_APPLAUSE, SFX_TICK} ;
  m_audio->LoadEffects(effects, 3);
  m_audio->AddToPlaylist( MOD_MUSIC ) ;
  
  // Start playing the music
	mmLoad( MOD_MUSIC );
	mmStart( MOD_MUSIC, MM_PLAY_LOOP );
    
  uint32_t keyMapping = CSTORAGE::GetInstance()->GetConfig(CFGTAG_VOLUME_KEYMAPPING)->GetUInt32() ;
  GetInputManager()->ActivatePreconfiguredMapping((int8_t)keyMapping) ;
  
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

 