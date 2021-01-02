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

mm_word CAUDIOSTREAMING::MusicCallback(mm_word msg, mm_word param) 
{
  if (msg == MMCB_SONGFINISHED)
  {
    if (!m_self->m_playing)
      return 0;
    m_self->playingIndex = (m_self->playingIndex + 1) % m_self->m_playlist.size() ;
    mmStart( m_self->m_playlist[m_self->playingIndex], MM_PLAY_ONCE );    
  }
  return 0 ;
}

CAUDIOSTREAMING::CAUDIOSTREAMING()
{

  mmInitDefaultMem( (mm_addr)soundbank_bin );
  
  mmSetModuleVolume(m_musicVolume) ;
  mmSetEffectsVolume(m_effectVolume) ;
  
  mmSetEventHandler(MusicCallback) ;
}

void CAUDIOSTREAMING::LoadEffects(uint32_t *ids, uint32_t count)
{
  for (uint32_t i=0;i<count;i++)
  {
    mmLoadEffect(ids[i]) ;
  }
}

void CAUDIOSTREAMING::ClearPlaylist() 
{
  m_playing = false ;
  mmStop() ;
  uint32_t ime = REG_IME ;
  REG_IME = 0 ;
  m_playlist.clear() ;
  REG_IME = ime ;
}

void CAUDIOSTREAMING::AddToPlaylist(uint32_t music)
{
  mmLoad(music) ;
  uint32_t ime = REG_IME ;
  REG_IME = 0 ;
  m_playlist.push_back(music) ;
  REG_IME = ime ;
  if (m_playlist.size() == 1)
  {
    playingIndex = 0 ;
    m_playing = true ;
    mmStart( music, MM_PLAY_ONCE );
  }
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


