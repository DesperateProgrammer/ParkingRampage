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

  // Start playing the music
	mmLoad( MOD_MUSIC );
	mmStart( MOD_MUSIC, MM_PLAY_LOOP );
  
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
