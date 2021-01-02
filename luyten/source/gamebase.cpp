#include "gamebase.h"
#include "sprite.h"
#include "storage.h"
#include <stdio.h>
#include <nds.h>

class CGAMEBASE *CGAMEBASE::m_instance = 0 ;

void CGAMEBASE::Run()
{
  Initialize();
  while (Tick());
  Destruct();
}

void CGAMEBASE::Initialize()
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
  

  m_audio = CAUDIOSTREAMING::GetInstance() ;

  for (uint32_t i=0;i<PersonalData->nameLen;i++)
  {
    m_nickname += (char)(PersonalData->name[i]) ;
  }
} 

void CGAMEBASE::Destruct()
{
}

void CGAMEBASE::OnEveryTick()
{
  swiWaitForVBlank();
  m_input.Tick() ;
  
  for (int screen=0;screen<2;screen++)
  {  
    UpdateFading(screen);
    UpdateRotScale(screen) ;
  }
}


 