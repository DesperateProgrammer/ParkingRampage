#include "textoverlay.h"
#include "font.h"
#include <nds.h>

CTEXTOVERLAY::CTEXTOVERLAY(const uint8_t screen)
: m_screen(screen), m_tileDataSource((uint16_t *)fontTiles), m_tilePalSource(fontPal), m_tileMapSource(fontMap),
  m_dataLength(fontTilesLen), m_paletteLength(fontPalLen), m_mapLength(fontMapLen)
{
  irqSet(IRQ_VBLANK, (VoidFn)RainbowVSyncHandler) ;
  irqEnable(IRQ_VBLANK) ;
}

void CTEXTOVERLAY::Initialize() 
{
  // Set 32x32 tile (8x8) map with 256 colors
  if (m_screen == 1)
  {
    vramSetBankC(VRAM_C_SUB_BG);
    BGCTRL_SUB[0] = BG_TILE_BASE(2) | BG_MAP_BASE(1) | BG_COLOR_256 | BG_32x32 ;
    
    bgExtPaletteEnableSub();
    dmaCopy(m_tileDataSource,(void *)CHAR_BASE_BLOCK_SUB(2),m_dataLength);
    memset((void *)SCREEN_BASE_BLOCK_SUB(1), 0, 32*32*2) ;
//    dmaCopy(m_tileMapSource,(void *)SCREEN_BASE_BLOCK_SUB(1),m_mapLength);
    // you can only access extended palettes in LCD mode
    vramSetBankH(VRAM_H_LCD); // for sub engine    
    dmaCopy(m_tilePalSource, &VRAM_H_EXT_PALETTE[0][0],m_paletteLength);
    ((uint16_t *)(&VRAM_H_EXT_PALETTE[0][1]))[1] = RGB15(31,0,0) ;
    ((uint16_t *)(&VRAM_H_EXT_PALETTE[0][2]))[1] = RGB15(24,0,0) ;
    ((uint16_t *)(&VRAM_H_EXT_PALETTE[0][3]))[1] = RGB15(0,31,0) ;
    ((uint16_t *)(&VRAM_H_EXT_PALETTE[0][4]))[1] = RGB15(0,24,0) ;
    ((uint16_t *)(&VRAM_H_EXT_PALETTE[0][5]))[1] = RGB15(0,0,31) ;
    ((uint16_t *)(&VRAM_H_EXT_PALETTE[0][6]))[1] = RGB15(0,0,24) ;
    ((uint16_t *)(&VRAM_H_EXT_PALETTE[0][7]))[1] = RGB15(31,31,0) ;
    ((uint16_t *)(&VRAM_H_EXT_PALETTE[0][8]))[1] = RGB15(24,24,0) ;
    vramSetBankH(VRAM_H_SUB_BG_EXT_PALETTE); // for sub engine
  } else
  {
    vramSetBankA(VRAM_A_MAIN_BG);
    BGCTRL[0] = BG_TILE_BASE(2) | BG_MAP_BASE(1) | BG_COLOR_256 | BG_32x32 ;

    bgExtPaletteEnable();
    dmaCopy(m_tileDataSource,(void *)CHAR_BASE_BLOCK(2),m_dataLength);
    memset((void *)SCREEN_BASE_BLOCK(1), 0, 32*32*2) ;
//    dmaCopy(m_tileMapSource,(void *)SCREEN_BASE_BLOCK(1),m_mapLength);
    // you can only access extended palettes in LCD mode
    vramSetBankE(VRAM_E_LCD); // for main engine
    dmaCopy(m_tilePalSource ,&VRAM_E_EXT_PALETTE[0][0],m_paletteLength);
    ((uint16_t *)(&VRAM_E_EXT_PALETTE[0][1]))[1] = RGB15(31,0,0) ;
    ((uint16_t *)(&VRAM_E_EXT_PALETTE[0][2]))[1] = RGB15(24,0,0) ;
    ((uint16_t *)(&VRAM_E_EXT_PALETTE[0][3]))[1] = RGB15(0,31,0) ;
    ((uint16_t *)(&VRAM_E_EXT_PALETTE[0][4]))[1] = RGB15(0,24,0) ;
    ((uint16_t *)(&VRAM_E_EXT_PALETTE[0][5]))[1] = RGB15(0,0,31) ;
    ((uint16_t *)(&VRAM_E_EXT_PALETTE[0][6]))[1] = RGB15(0,0,24) ;
    ((uint16_t *)(&VRAM_E_EXT_PALETTE[0][7]))[1] = RGB15(31,31,0) ;
    ((uint16_t *)(&VRAM_E_EXT_PALETTE[0][8]))[1] = RGB15(24,24,0) ;
    vramSetBankE(VRAM_E_BG_EXT_PALETTE);     // for main engine
  }
}

void CTEXTOVERLAY::Dispose()
{
}

void CTEXTOVERLAY::EnableTextWindow(uint8_t x, uint8_t y, uint8_t width, uint8_t height) 
{
  ((volatile uint16_t *)(0x4000000 + m_screen * 0x1000))[0] |= (1 << 13) ;
  ((volatile uint16_t *)(0x4000040 + m_screen * 0x1000))[0] = (x << 8) | ((x + width) << 0) ;
  ((volatile uint16_t *)(0x4000040 + m_screen * 0x1000))[2] = (y << 8) | ((y + height) << 0) ;
  ((volatile uint16_t *)(0x4000040 + m_screen * 0x1000))[4] = 0x0001 ;
  ((volatile uint16_t *)(0x4000040 + m_screen * 0x1000))[5] = 0x3F3F ;
}

void CTEXTOVERLAY::DisableTextwindow() 
{
  ((volatile uint16_t *)(0x4000000 + m_screen * 0x1000))[0] &= ~(1 << 13) ;
  ((volatile uint16_t *)(0x4000040 + m_screen * 0x1000))[4] = 0x003F ;  
}

void CTEXTOVERLAY::Clear()
{
  if (m_screen == 1)
    memset((void *)SCREEN_BASE_BLOCK_SUB(1), 0, 32*32*2) ;
  else
    memset((void *)SCREEN_BASE_BLOCK(1), 0, 32*32*2) ;  
    
  DisableTextwindow() ;
}

void CTEXTOVERLAY::SetChar(uint8_t x, uint8_t y, char ch, uint8_t pal) 
{
  uint16_t tileA = (ch & 0x0F) + 2*16*((ch & 0xF0) >> 4) + 1 ;
  uint16_t tileB = tileA + 16  ;
  if (m_screen == 1)
  {
    ((uint16_t *)(SCREEN_BASE_BLOCK_SUB(1)))[x + y*2*32] = tileA | (pal << 12) ;
    ((uint16_t *)(SCREEN_BASE_BLOCK_SUB(1)))[x + y*2*32 + 32] = tileB | (pal << 12) ;
  } else
  {
    ((uint16_t *)(SCREEN_BASE_BLOCK(1)))[x + y*2*32] = tileA | (pal << 12)  ;
    ((uint16_t *)(SCREEN_BASE_BLOCK(1)))[x + y*2*32 + 32] = tileB | (pal << 12) ;
  }
}

void CTEXTOVERLAY::SetBiColorChar(uint8_t x, uint8_t y, char ch, uint8_t pal1, uint8_t pal2) 
{
  uint16_t tileA = (ch & 0x0F) + 2*16*((ch & 0xF0) >> 4) + 1 ;
  uint16_t tileB = tileA + 16  ;
  if (m_screen == 1)
  {
    ((uint16_t *)(SCREEN_BASE_BLOCK_SUB(1)))[x + y*2*32] = tileA | (pal1 << 12) ;
    ((uint16_t *)(SCREEN_BASE_BLOCK_SUB(1)))[x + y*2*32 + 32] = tileB | (pal2 << 12) ;
  } else
  {
    ((uint16_t *)(SCREEN_BASE_BLOCK(1)))[x + y*2*32] = tileA | (pal1 << 12)  ;
    ((uint16_t *)(SCREEN_BASE_BLOCK(1)))[x + y*2*32 + 32] = tileB | (pal2 << 12) ;
  }
}

void CTEXTOVERLAY::SetText(uint8_t x, uint8_t y, char *text, uint8_t pal) 
{
  while (*text)
  {
    SetChar(x++,y, *(text++), pal);
  }
}

void CTEXTOVERLAY::SetBiColorText(uint8_t x, uint8_t y, char *text, uint8_t pal1, uint8_t pal2) 
{
  while (*text)
  {
    SetBiColorChar(x++,y, *(text++), pal1, pal2);
  }
}

void CTEXTOVERLAY::SetBorder(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t palette) 
{
  SetChar(x,y, 0x01, palette) ;
  SetChar(x+w, y, 0x03, palette) ;
  SetChar(x,y+h, 0x04, palette) ;
  SetChar(x+w+h, y, 0x08, palette) ;
  for (int i=1;i+1<w;i++)
    SetChar(i, y, 0x02) ;
  for (int i=1;i+1<h;i++)
    SetChar(x, i, 0x09) ;
}

void CTEXTOVERLAY::RainbowVSyncHandler()
{
  static uint16_t color = 0;
  static uint32_t h = 1 << 16 ;
  h += 0x0122 * 192; // this should be ~360° / 16 lines
  vramSetBankH(VRAM_H_LCD); // for sub engine
  vramSetBankE(VRAM_E_LCD); // for main engine
  ((uint16_t *)(&VRAM_H_EXT_PALETTE[0][TEXTCOLOR_RAINBOW]))[1] = color ;
  ((uint16_t *)(&VRAM_E_EXT_PALETTE[0][TEXTCOLOR_RAINBOW]))[1] = color ;
  vramSetBankE(VRAM_E_BG_EXT_PALETTE);      
  vramSetBankH(VRAM_H_SUB_BG_EXT_PALETTE);      

  int32_t hi = (h / 60) ;
  int32_t f = hi & 0xFFFF ;
  hi >>= 16 ;
  int32_t p = 0 ;
  int32_t q = ((1 << 8) - f) >> 11;
  int32_t t = f >> 11 ;
  if (hi >= 6)
  {
    h = h - (360 << 16) ;
  }
  switch (hi % 6)
  {
    case 0:
      color = RGB15(31, t, p) ;
      break;
    case 1:
      color = RGB15(q, 31, p) ;
      break;
    case 2:
      color = RGB15(p, 31, t) ;
      break;
    case 3:
      color = RGB15(p, q, 31) ;
      break;
    case 4:
      color = RGB15(t, p, 31) ;
      break;
    case 5:
      color = RGB15(31, p, q) ;
      break;
  }
}

