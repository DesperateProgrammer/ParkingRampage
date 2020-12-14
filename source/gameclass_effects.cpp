#include "gameclass.h"
#include <nds.h>

void CGAME::StartFade(uint8_t screen, EFADE mode, uint32_t timespan) 
{
  if (screen >= 2)
    return ;
  m_fadeStart[screen] = GetTimerTicks() ;
  m_fadeEnd[screen] = m_fadeStart[screen] + timespan ;
  m_fadeMode[screen] = mode ;
}

EFADE CGAME::GetFadeMode(uint8_t screen)
{
  return m_fadeMode[screen] ;
}

bool CGAME::IsFading(uint8_t screen)
{
  uint32_t time = GetTimerTicks() ;
  return ((m_fadeEnd[screen] > time) && (m_fadeStart[screen] < time)) ;
}

void CGAME::StartRotScale(uint8_t screen, uint32_t timespan, int16_t rotations, uint16_t startScale, uint16_t endScale) 
{
  if (screen >= 2)
    return ;
  m_rotscaleStart[screen] = GetTimerTicks() ;
  m_rotscaleEnd[screen] = m_rotscaleStart[screen] + timespan ;
  m_startScale[screen] = startScale ;
  m_endscale[screen] = endScale ;
  m_rotations[screen] += rotations ;
}

bool CGAME::IsRotScaling(uint8_t screen) 
{
  uint32_t time = GetTimerTicks() ;
  return ((m_rotscaleEnd[screen] > time) && (m_rotscaleStart[screen] < time)) ;
}

void CGAME::UpdateFading(uint8_t screen) 
{
  if (screen >= 2)
    return ;
  if (IsFading(screen))
  {
    uint32_t time = GetTimerTicks() ;
    uint8_t progress = (time-m_fadeStart[screen])*16 / (m_fadeEnd[screen]-m_fadeStart[screen]) ;
    *((volatile uint16_t *)(0x4000050 + screen*0x1000)) = 0x20DF ;
    switch (m_fadeMode[screen])
    {
      case eFADEOUT:
        *((volatile uint16_t *)(0x4000054 + screen*0x1000)) = progress & 0x1F ;
        break ;
      case eFADEIN:
        *((volatile uint16_t *)(0x4000054 + screen*0x1000)) = 16 - (progress & 0x1F) ;
        break ;
    }
  } else
  {
    *((volatile uint16_t *)(0x4000050 + screen*0x1000)) = 0x20DF ;  
    switch (m_fadeMode[screen])
    {
      case eFADEOUT:
        *((volatile uint16_t *)(0x4000054 + screen*0x1000)) = 16 ;
        break ;
      case eFADEIN:
        *((volatile uint16_t *)(0x4000054 + screen*0x1000)) = 0 ;
        break ;
    }  }
}

void CGAME::UpdateRotScale(uint8_t screen) 
{
#if 1
  if (screen >= 2)
    return ;
  if (IsRotScaling(screen))
  {
    uint32_t time = GetTimerTicks() ;
    uint16_t progress = ((time-m_rotscaleStart[screen]) * 256) / (m_rotscaleEnd[screen]-m_rotscaleStart[screen]) ;
    int16_t scaling8p8 = m_startScale[screen] / 16 + ((((int32_t)m_endscale[screen] - m_startScale[screen]) * progress)  / 4096) ;
    *((volatile uint16_t *)(0x4000030 + screen*0x1000)) = scaling8p8 ; // DX
    *((volatile uint16_t *)(0x4000032 + screen*0x1000)) = 0 ; // DMX
    *((volatile uint16_t *)(0x4000034 + screen*0x1000)) = 0 ; // DY
    *((volatile uint16_t *)(0x4000036 + screen*0x1000)) = scaling8p8  ; // DMY
  } else
#endif
  {
    uint16_t scaling4p12 = m_endscale[screen] ;
    *((volatile uint16_t *)(0x4000030 + screen*0x1000)) = scaling4p12 / 16 ;// DX
    *((volatile uint16_t *)(0x4000032 + screen*0x1000)) = 0 ; // DMX
    *((volatile uint16_t *)(0x4000034 + screen*0x1000)) = 0 ; // DY
    *((volatile uint16_t *)(0x4000036 + screen*0x1000)) = scaling4p12 / 16  ; // DMY  
  }
}

