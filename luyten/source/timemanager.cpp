#include "timemanager.h"
#include <nds.h>

volatile uint32_t CTIMEMANAGER::m_timeTicks = 0 ;


CTIMEMANAGER::CTIMEMANAGER() 
{
  InitializeTime() ;
}

uint32_t CTIMEMANAGER::GetTimerTicks() 
{
  return m_timeTicks;
}

void CTIMEMANAGER::TimerTick()
{
  m_timeTicks++ ;
}

void CTIMEMANAGER::InitializeTime() 
{
  m_timeTicks = 0 ;
	timerStart(1, ClockDivider_64, TIMER_FREQ_64(1000), TimerTick);
}
