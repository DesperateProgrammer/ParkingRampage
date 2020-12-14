#include "tilemap.h"
#include <nds.h>

CTILEMAP::CTILEMAP(const uint8_t screen, const uint16_t *tileData, const uint32_t tileDataLength, const uint16_t *tilePalette, const uint32_t tilePalettelength, const uint16_t *initialMap, const uint32_t initialMapLength)
: m_screen(screen), m_tileDataSource(tileData), m_tilePalSource(tilePalette), m_tileMapSource(initialMap),
  m_dataLength(tileDataLength), m_paletteLength(tilePalettelength), m_mapLength(initialMapLength)
{
}

void CTILEMAP::Initialize() 
{
  // Set 32x32 tile (8x8) map with 256 colors
  if (m_screen == 1)
  {
    vramSetBankC(VRAM_C_SUB_BG);
    BGCTRL_SUB[3] = BG_TILE_BASE(1) | BG_MAP_BASE(0) | BG_COLOR_256 | BG_RS_32x32 ;

    dmaCopy(m_tileDataSource,(void *)CHAR_BASE_BLOCK_SUB(1),m_dataLength);
    dmaCopy(m_tileMapSource,(void *)SCREEN_BASE_BLOCK_SUB(0),m_mapLength);
    dmaCopy(m_tilePalSource,BG_PALETTE_SUB,m_paletteLength);
  } else
  {
    vramSetBankA(VRAM_A_MAIN_BG);
    BGCTRL[3] = BG_TILE_BASE(1) | BG_MAP_BASE(0) | BG_COLOR_256 | BG_RS_32x32 ;

    dmaCopy(m_tileDataSource,(void *)CHAR_BASE_BLOCK(1),m_dataLength);
    dmaCopy(m_tileMapSource,(void *)SCREEN_BASE_BLOCK(0),m_mapLength);
    dmaCopy(m_tilePalSource,BG_PALETTE,m_paletteLength);
  }
}

void CTILEMAP::Dispose()
{
}

void CTILEMAP::SetMap(uint32_t x, uint32_t y, uint8_t index) 
{
  uint16_t *map = (uint16_t *) SCREEN_BASE_BLOCK_SUB(0) ;
  if (m_screen == 0)
    map = (uint16_t *) SCREEN_BASE_BLOCK(0) ;
  map[((x)+(y)*32) / 2] = (map[((x)+(y)*32) / 2] & ~(0xff << (((x) & 1)*8))) | ((uint16_t)(index) << (((x) & 1)*8)) ;
}
