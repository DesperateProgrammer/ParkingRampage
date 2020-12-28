#pragma once

#include "menu.h"
#include <stdint.h>

typedef class CMENUWITHCARSELECTOR : public CMENU
{
  public:
    CMENUWITHCARSELECTOR(class CGAME *game) : CMENU(game) {};
  
    virtual void OnDrawItemSelection(uint8_t y) 
    {
      int32_t scale = sinLerp(m_game->GetTimeManager()->GetTimerTicks()*70);
      m_game->GetSubSprites()->SetSprite(0, SPRITE_CAR_TARGET, 2*2*8 + scale * 8 / 4096, y * 16, ORIENTATION_RIGHT) ;
      m_game->GetSubSprites()->SetSprite(1, SPRITE_CAR_TARGET, 12*2*8 - scale * 8 / 4096, y * 16, ORIENTATION_LEFT) ;
      m_game->GetSubSprites()->Update() ;
    }
} CMENUWITHCARSELECTOR ;