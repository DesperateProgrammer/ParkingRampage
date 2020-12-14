#pragma once

#include <stdint.h>

typedef class CTILEMAP
{
  protected:
    const uint8_t m_screen ;
    const uint16_t *m_tileDataSource ;
    const uint16_t *m_tilePalSource ;
    const uint16_t *m_tileMapSource ;
    const uint32_t m_dataLength, m_paletteLength, m_mapLength ;
  public:
    CTILEMAP(const uint8_t screen, const uint16_t *tileData, const uint32_t tileDataLength, const uint16_t *tilePalette, const uint32_t tilePalettelength, const uint16_t *initialMap = 0, const uint32_t initialMapLength = 0); 
    
    void Initialize() ;
    void Dispose() ;
    
    void SetMap(uint32_t x, uint32_t y, uint8_t index) ;
} CTILEMAP ;
