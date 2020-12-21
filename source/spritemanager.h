#pragma once

#include <stdint.h>
#include <map>
#include <nds.h>

typedef struct CSPRITEDATA
{
  bool        m_inUse ;
  uint32_t    m_lastUsed ;
  uint16_t *  m_dataSource ;
  SpriteSize  m_spriteSize ;
} CSPRITEDATA ;


typedef class CSPRITEMANAGER
{
  protected:
    uint8_t m_screen ;
    
    std::map<uint16_t, CSPRITEDATA> m_knownSprites ;
    uint16_t m_mapping[128] ;
    
    void UnloadSlot(uint8_t slot) {} ;
  public:
    CSPRITEMANAGER(uint8_t screen) ;
    
    void LoadSprite(uint16_t id, uint8_t *src, uint16_t length, SpriteSize spriteSize) ;
    void SetSprite(uint8_t slot, uint16_t sprite, uint16_t x, uint16_t y, uint8_t orientation) ;
    void Update() ;
    
    uint16_t *GetSpriteSource(uint16_t id) { return m_knownSprites[id].m_dataSource; };
    
    void DisableAll() ;
} CSPRITEMANAGER ;