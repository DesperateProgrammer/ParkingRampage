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
