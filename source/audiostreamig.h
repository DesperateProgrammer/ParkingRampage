#pragma once

#include <nds.h>
#include <maxmod9.h>

typedef class CAUDIOSTREAMING
{
  protected:
    static class CAUDIOSTREAMING *m_self ;    
    
  public:
    CAUDIOSTREAMING();
  
    static class CAUDIOSTREAMING *GetInstance() ;
    void SetMusicVolume(uint16_t volume);
    
} CAUDIOSTREAMING ;