#pragma once

#include <stdint.h>

typedef class CTIMEMANAGER
{
  protected:
    static volatile uint32_t m_timeTicks ;

    static void TimerTick() ;
  public:
    CTIMEMANAGER() ;
    
    void InitializeTime() ;    
    uint32_t GetTimerTicks() ;
    
} CTIMEMANAGER ;

