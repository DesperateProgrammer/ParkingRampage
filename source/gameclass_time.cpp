#include "gameclass.h"
#include <nds.h>

volatile uint32_t CGAME::m_timeTicks = 0 ;

uint32_t CGAME::GetTimerTicks() 
{
  return m_timeTicks;
}

void CGAME::TimerTick()
{
  m_timeTicks++ ;
}

void CGAME::InitializeTime() 
{
  m_timeTicks = 0 ;
	timerStart(1, ClockDivider_64, TIMER_FREQ_64(1000), TimerTick);
}

uint32_t CGAME::GetCurrentLevelTime()
{
  if (m_timeOfResume)
    return m_levelTime + (GetTimerTicks() - m_timeOfResume) ;
  else
    return m_levelTime ;
}

void CGAME::ResetLevelTime() 
{
  m_timeOfResume = 0 ;
  m_levelTime = 0 ;
}

void CGAME::PauseLevelTime() 
{
  uint32_t curTime = GetCurrentLevelTime() ;
  m_timeOfResume = 0 ;
  m_levelTime = curTime ;
}

void CGAME::ContinueLevelTime() 
{
  m_timeOfResume = GetTimerTicks() ;
}
