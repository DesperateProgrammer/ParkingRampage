#include "audiostreaming.h"

#include "soundbank.h"
#include "soundbank_bin.h"

class CAUDIOSTREAMING *CAUDIOSTREAMING::m_self = 0;

class CAUDIOSTREAMING *CAUDIOSTREAMING::GetInstance() 
{
  if (!m_self)
    m_self = new CAUDIOSTREAMING() ;
  return m_self ;
}

CAUDIOSTREAMING::CAUDIOSTREAMING()
{

  mmInitDefaultMem( (mm_addr)soundbank_bin );
  
  mmSetModuleVolume(m_musicVolume) ;
  mmSetEffectsVolume(m_effectVolume) ;
  

  // Start playing the music
	mmLoad( MOD_MUSIC );
	mmStart( MOD_MUSIC, MM_PLAY_LOOP );
  
  mmLoadEffect( SFX_START );
  mmLoadEffect( SFX_APPLAUSE );
  mmLoadEffect( SFX_TICK );
  
}


#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

void CAUDIOSTREAMING::SetMusicVolume(uint16_t volume)
{
  volume = max(0, min(volume, 1024));
  if (m_musicVolume != volume)  
    mmSetModuleVolume(volume) ;
  m_musicVolume = volume ;
}

void CAUDIOSTREAMING::SetEffectVolume(uint16_t volume)
{
  volume = max(0, min(volume, 1024));
  if (m_effectVolume != volume)
    mmSetEffectsVolume(volume) ;
  m_effectVolume = volume ;
}

void CAUDIOSTREAMING::PlayEffect(unsigned int id) 
{
  mmEffect((mm_word)id);
}


