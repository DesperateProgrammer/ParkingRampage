#pragma once

#include <nds.h>
#include <maxmod9.h>
#include <vector>

typedef class CAUDIOSTREAMING
{
  protected:
    static class CAUDIOSTREAMING *m_self ;    
    
    uint16_t m_musicVolume = 0;
    uint32_t m_effectVolume = 768;
    
    uint32_t playingIndex = 0 ;
    std::vector<uint32_t> m_playlist ;
    bool m_playing = false ;
    
    static mm_word MusicCallback(mm_word msg, mm_word param) ;
    
  public:
    CAUDIOSTREAMING();
  
    static class CAUDIOSTREAMING *GetInstance() ;
    
    void LoadEffects(uint32_t *ids, uint32_t count) ;
    
    void ClearPlaylist() ;
    void AddToPlaylist(uint32_t music) ;
    
    void SetMusicVolume(uint16_t volume);
    uint16_t GetMusicVolume() { return m_musicVolume; };
    void SetEffectVolume(uint16_t volume);
    uint16_t GetEffectVolume() { return m_effectVolume; };
    
    void PlayEffect(unsigned int id) ;
    
} CAUDIOSTREAMING ;