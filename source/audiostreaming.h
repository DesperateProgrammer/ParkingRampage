#pragma once

#include <nds.h>
#include <maxmod9.h>

typedef class CAUDIOSTREAMING
{
  protected:
    static class CAUDIOSTREAMING *m_self ;    
    
    uint16_t m_musicVolume ;
    
  public:
    CAUDIOSTREAMING();
  
    static class CAUDIOSTREAMING *GetInstance() ;
    void SetMusicVolume(uint16_t volume);
    uint16_t GetMusicVolume() { return m_musicVolume; };
    
} CAUDIOSTREAMING ;